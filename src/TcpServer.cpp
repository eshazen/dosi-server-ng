
#include "TcpServer.hh"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

TcpServer::TcpServer() {
  TcpServer( SERVER_PORT);
}

TcpServer::TcpServer( int port) {

  /*
   * Initialize the addres data structure
   */
  memset((void *)&sock_in, 0, sizeof(sock_in));
  sock_in.sin_family = AF_INET;
  sock_in.sin_addr.s_addr = INADDR_ANY;
  sock_in.sin_port = htons(SERVER_PORT);

  /*
   * Create a socket
   */
  if ((s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    fprintf(stderr, "Failed to create socket - %s\n", strerror(errno));
    exit(1);
  }

  /* set the "reuse" option */
  rc = setsockopt( s, SOL_SOCKET, SO_REUSEADDR, &c_one, sizeof(c_one));

  /*
   * Bind an address to the socket
   */
  if (bind(s, (struct sockaddr *)&sock_in, sizeof(sock_in)) < 0) {
    fprintf(stderr, "Failed to bind - %s\n", strerror(errno));
    exit(1);
  }

  /*
   * Set the length of the listen queue
   */
  if (listen(s, MAX_PENDING) < 0) {
    fprintf(stderr, "Failed to set listen queue - %s\n", strerror(errno));
    exit(1);
  }

  conn = 0;
  active = true;

}
