//
// test TCP server
//

#include <stdio.h>
#include "TcpServer.hh"

int main( int argc, char *argv[]) {

  int conn, line;
  int rc;

  TcpServer server(SERVER_PORT);

  while( true) {
    if( (conn = server.CheckConnect())) {
      printf("Got a connection...\n");

      // check for data
      while( (rc = server.CheckData())) {
	if( rc > 0)
	  printf("got %d bytes\n", rc);
      }
      printf("Connection closed...\n");

    }
  }
}
