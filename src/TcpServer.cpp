// TcpServer.cc - simple TCP server for DOSI
//
// User must call polling functions (CheckConnect and CheckData) to check for activity
//
// TcpServer( int port)            - create socket, bind, listen
//
// int CheckConnect()              - check for connection
//                                   return 0 if none, else connection number
//
// int CheckData()                 - check for incoming data, store in local buffer
//                                   return:  >0 - got data, returns size
//                                            =0 - connection closed
//                                            <0 - no data seen, try again
//
// char *GetData()                 - returns a pointer to data or NULL if none
//
// int GetDataSize()               - returns size of data in buffer
//
// int SendData( void *data, int size, bool bin)
//                                 - send <size> bytes of data from <data>
//                                 - <bin> means send binary data with header
// ------------------------------------------------------------------------

#include "TcpServer.hh"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// constructor with specified port
TcpServer::TcpServer( int port) {
  InitializeListen( port);
}

// constructor with defauolt port
TcpServer::TcpServer() {
  InitializeListen( SERVER_PORT);
}


// create a socket and initialize to listen for connections
//
void TcpServer::InitializeListen( int port) {

  int c_one = 1;		// constant one

  if( debug) printf("TcpServer::InitializeListen() sock_in = 0x%lx\n", (unsigned long)&sock_in);

  /*
   * Initialize the address data structure
   */
  memset((void *)&sock_in, 0, sizeof(sock_in));
  sock_in.sin_family = AF_INET;
  sock_in.sin_addr.s_addr = INADDR_ANY;
  sock_in.sin_port = htons(SERVER_PORT);

  /*
   * Create a socket
   * set SOCK_NONBLOCK so we can poll for connections
   */
  if ((listen_s = socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP)) < 0) {
    fprintf(stderr, "TcpServer::InitializeListen() - Failed to create socket - %s\n", strerror(errno));
    exit(1);
  }

  /* set the "reuse" option */
  rc = setsockopt( listen_s, SOL_SOCKET, SO_REUSEADDR, &c_one, sizeof(c_one));

  /*
   * Bind an address to the socket
   */
  if (bind(listen_s, (struct sockaddr *)&sock_in, sizeof(sock_in)) < 0) {
    fprintf(stderr, "TcpServer::InitializeListen() - Failed to bind - %s\n", strerror(errno));
    exit(1);
  }

  /*
   * Set the length of the listen queue
   */
  if (listen(listen_s, MAX_PENDING) < 0) {
    fprintf(stderr, "TcpServer::InitializeListen() - Failed to set listen queue - %s\n", strerror(errno));
    exit(1);
  }

  conn = 0;
  active = true;
  connected = false;

  if( debug) printf("TcpServer::TcpServer() socket listening...\n");
  if( debug) printf("sock_in = 0x%lx\n", (unsigned long)&sock_in);
}

//
// check for a new connection, and accept if so
// return 0 if no pending connection attempt, or connection (session) number if so
// abort with message to stderr on error
//
int TcpServer::CheckConnect() {

  if( !active) {
    fprintf( stderr, "TcpServer::CheckConnect() no server active\n");
    exit(1);
  }
  
  if( connected) {
    fprintf( stderr, "TcpServer::CheckConnect() already connected\n");
    exit(1);
  }

  unsigned int len = sizeof( sock_in);
  
  if( debug) printf("CheckConnect() sock_in = 0x%lx\n", (unsigned long)&sock_in);

  conn_s = accept(listen_s, (struct sockaddr *)&sock_in, &len);
  if( debug) printf("TcpServer::CheckConnect() accept(): %d\n", conn_s);

  if ( conn_s < 0) {		// error return
    // if there isn't a connection request, just return 0
    if( errno == EAGAIN || errno == EWOULDBLOCK) {
      if( debug) printf("TcpServer::CheckConnect() no requests\n");
      return 0;
    } else {
      // some other error - die
      fprintf(stderr, "TcpServer::CheckConnect accept - %s\n",
	      strerror(errno));
      exit(1);
    }
  }

  // success!
  ++conn;
  if( debug) printf("TcpServer::CheckConnect() connected!  conn=%d\n", conn);
  connected = true;

  line = 0;
  return conn;
}


//
// check for incoming data on connected socket
// receive if so.  Max TCP_RECV_CHUNK bytes
//
// returns: =0  connection closed
//          <0  no data
//          >0  data received
//
// abort program on error, with message to stderr
//

int TcpServer::CheckData() {

  if( !active || !connected) {
    fprintf( stderr, "TcpServer::CheckData(): Not connected\n");
    return -1;
  }

  // try to receive data
  int len = recv(conn_s, buf, sizeof(buf), MSG_DONTWAIT);

  // return 0 - connection closed
  if( len == 0) {
    printf("TcpServer::CheckData() : connection closed\n");
    connected = false;
    buf_count = 0;
  }

  // return -1, it's an error
  if( len < 0) {
    // these mean no data, try again
    if( errno == EAGAIN || errno == EWOULDBLOCK) {
      buf_count = 0;
    } else {
      // anything else is fatal
      fprintf(stderr, "TcpServer::CheckData recv - %s\n",
	      strerror(errno));
      exit( 1);
    }
  }

  // positive value - got data
  if( len > 0) {
      ++line;
      if (len >= TCP_RECV_CHUNK) {
	len = TCP_RECV_CHUNK - 1;
      }
      buf[len] = 0;
      buf_count = len;
      if( debug) printf("TcpServer::CheckData() RECV: \"%s\"\n", buf);
  }

  return len;
}

//
// return buffer address
//
const void *TcpServer::GetData() {
  if( buf_count)
    return (void *)buf;
  else
    return NULL;
}

//
// return buffer size
//
int TcpServer::GetDataSize() {
  return buf_count;
}

//
// send data
// count is the number of bytes
// if binary is true, prefix with "definite length block response" header
// as "#dnnnn" where 3 is the number of digits in the decimal length nnnn
//
// return bytes sent, -1 on error or 0 if a re-try is needed for some reason
//
// NOTE: this function doesn't check for errors related to large sizes;
// probably should check for EMSGSIZE error and break up longer data
//
int TcpServer::SendData( void *data, int count, int binary) {
  int rc;
  char headr[10];
  ssize_t total = 0;

  // check for silly mistakes
  if( !active) {
    fprintf( stderr, "TcpServer::SendData() no server active\n");
    exit(1);
  }
  
  if( !connected) {
    fprintf( stderr, "TcpServer::SendData() not connected\n");
    exit(1);
  }
    
  // binary is true - include the #dnnnn header
  if( binary) {

    // format the header
    snprintf( headr, sizeof(headr), "%d", count);
    int s = strlen( headr);
    snprintf( headr, sizeof(headr), "#%d%d", s, count);
    if( debug) printf("SendData() binary header for %d is \"%s\"\n", count, headr);

    // send the header
    rc = send( conn_s, headr, strlen(headr), MSG_MORE);

    // deal with errors
    if( rc == -1) {
      if( errno == EAGAIN || errno == EWOULDBLOCK) {
	return 0;
      } else {
	fprintf(stderr, "TcpServer::SendData() header - %s\n", strerror(errno));
	return -1;
      }
    }

    total += rc;
  }

  // now send the data itself
  rc = send( conn_s, data, count, 0);
  
  if( rc == -1) {
    if( errno == EAGAIN || errno == EWOULDBLOCK) {
      return 0;
    } else {
      fprintf(stderr, "TcpServer::SendData() data - %s\n", strerror(errno));
      return -1;
    }
  }

  total += rc;

  return (int)total;
}
