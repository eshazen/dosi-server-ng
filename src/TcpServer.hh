// TcpServer.hh
//
// Header file for simple TCP server class
//


#ifndef TCP_SERVER_HH
#define TCP_SERVER_HH

#include <stdint.h>
#include <stdbool.h>
#include <netinet/in.h>

#define SERVER_PORT	27015

#define MAX_PENDING 1		   // only one connection request at a time
#define TCP_RECV_CHUNK 1024	   // maximum length of a command


class TcpServer {

public:
  TcpServer();			  // constructor for default port
  TcpServer( int server_port);	  // constructor for specified port
  int CheckConnect();		  // check for new connection, return non-zero if success
  int CheckData();		  // recv data, return: 0 if closed, -1 if none, else length
  const void *GetData();	  // return pointer to recv data or NULL
  int GetDataSize();		  // return bytes in buffer or 0
  int SendData(void *, int, int); // send data, text or binary, return count or -1 on error

private:
  void InitializeListen( int port); // initialize, bind, listen
  bool active = false;		    // listening for connections
  bool connected = false;	    // connection active
  char buf[TCP_RECV_CHUNK];	    // default buffer for input
  int buf_count = 0;		    // number of bytes in buf[]

  struct sockaddr_in sock_in;

  int conn = 0;	                   // connection number
  int line = 0;	                   // line (command) number
  int listen_s = 0;	           // TCP socket to listen for connections
  int conn_s = 0;		   // TCP socket when connected
  int rc = 0;

  bool debug = false;
};


#endif  
