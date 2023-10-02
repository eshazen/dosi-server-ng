
#include "TcpServer.hh"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

TcpServer::TcpServer( int port) {

  if( debug) printf("sock_in = 0x%lx\n", (unsigned long)&sock_in);

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
    fprintf(stderr, "Failed to create socket - %s\n", strerror(errno));
    exit(1);
  }

  /* set the "reuse" option */
  rc = setsockopt( listen_s, SOL_SOCKET, SO_REUSEADDR, &c_one, sizeof(c_one));

  /*
   * Bind an address to the socket
   */
  if (bind(listen_s, (struct sockaddr *)&sock_in, sizeof(sock_in)) < 0) {
    fprintf(stderr, "Failed to bind - %s\n", strerror(errno));
    exit(1);
  }

  /*
   * Set the length of the listen queue
   */
  if (listen(listen_s, MAX_PENDING) < 0) {
    fprintf(stderr, "Failed to set listen queue - %s\n", strerror(errno));
    exit(1);
  }

  conn = 0;
  active = true;

  if( debug) printf("TcpServer::TcpServer() socket listening...\n");
  if( debug) printf("sock_in = 0x%lx\n", (unsigned long)&sock_in);
}

//
// check for a new connection, and accept if so
// return 0 if no pending connection attempt, or connection (session) number if so
//
int TcpServer::CheckConnect() {
  unsigned int len = sizeof( sock_in);
  
  if( debug) printf("CheckConnect() sock_in = 0x%lx\n", (unsigned long)&sock_in);

  conn_s = accept(listen_s, (struct sockaddr *)&sock_in, &len);
  if( debug) printf("TcpServer::CheckConnect() accept(): %d\n", conn_s);

  if ( conn_s < 0) {
    if( errno == EAGAIN || errno == EWOULDBLOCK) {
      if( debug) printf("TcpServer::CheckConnect() no requests\n");
      return 0;
    }
    else {
      fprintf(stderr, "TcpServer::CheckConnect accept - %s\n",
	      strerror(errno));
      exit(1);
    }
  }

  ++conn;
  if( debug) printf("TcpServer::CheckConnect() connected!  conn=%d\n", conn);

  line = 0;
  return conn;
}


//
// check for incoming data on connected socket
//

int TcpServer::CheckData() {

  int len = recv(conn_s, buf, sizeof(buf), MSG_DONTWAIT);
  if( debug) printf("TcpServer::CheckData() len = %d\n", len);

  if( len > 0) {
      ++line;
      if (len >= MAX_LINE) {
	len = MAX_LINE - 1;
      }
      buf[len] = 0;
      if( debug) printf("TcpServer::CheckData() RECV: \"%s\"\n", buf);
  }

  if( len == 0) {
    printf("TcpServer::CheckData() : connection closed\n");
    active = false;
  }

  if( len < 0) {
    if( errno != EAGAIN && errno != EWOULDBLOCK)
      fprintf(stderr, "TcpServer::CheckData recv - %s\n",
	      strerror(errno));
  }

  return len;
}
