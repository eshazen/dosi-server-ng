#ifndef TCP_SERVER_HH
#define TCP_SERVER_HH

#include <stdint.h>
#include <stdbool.h>
#include <netinet/in.h>

#define SERVER_PORT	27015
#define MAX_PENDING     1
#define MAX_LINE	256


class TcpServer {
public:
  TcpServer();			// constructor for default port
  TcpServer( int server_port);	// constructor for specified port
  int CheckConnect();		// check for new connection, return non-zero if success
  int CheckData();		// check for incoming data, return: 0 if closed, -1 if none, else length

private:
  bool active = false;

  struct sockaddr_in sock_in;

  char buf[MAX_LINE];
  char reply[MAX_LINE];

  int conn, line;
  int listen_s;
  int conn_s;
  int c_one = 1;
  int rc;

  char mode;

  bool debug = false;
};


#endif  
