#ifndef TCP_SERVER_HH
#define TCP_SERVER_HH

#include <stdint.h>
#include <stdbool.h>
#include <netinet/in.h>

#define SERVER_PORT	27015

#define MAX_PENDING 1
#define TCP_RECV_CHUNK 1024


class TcpServer {
public:
  TcpServer();			  // constructor for default port
  TcpServer( int server_port);	  // constructor for specified port
  int CheckConnect();		  // check for new connection, return non-zero if success
  int CheckData();		  // recv data, return: 0 if closed, -1 if none, else length
  void *GetData();		  // return pointer to recv data or NULL
  int GetDataSize();		  // return bytes in buffer or 0
  int SendData(void *, int, int); // send data, text or binary, return count or -1 on error

private:
  bool active = false;
  char buf[TCP_RECV_CHUNK];	// default buffer for input
  int buf_count;

  struct sockaddr_in sock_in;

  int conn, line;
  int listen_s;
  int conn_s;
  int c_one = 1;
  int rc;

  char mode;

  bool debug = false;
};


#endif  
