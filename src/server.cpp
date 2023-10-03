//
// minimal example TCP server
//

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "TcpServer.hh"

int main( int argc, char *argv[]) {

  TcpServer* server = NULL;
  int conn;
  int line;

  // initialize the server and listen for connections
  server = new TcpServer();
  printf("Server open on default port\n");

  // loop forever processing connections
  while( true) {

    // wait for connection
    do {
      conn = server->CheckConnect(); // check for connection
    } while( !conn);

    printf("Got a connection (%d)...\n", conn);
    line = 0;			// zero line count

    // wait for commands and process
    while( true) {

      // check data, expect length or -1 (no data), 0 (closed)
      int rc = server->CheckData();

      if( rc == 0) break;	// connection closed
      if( rc < 0) continue;	// no data, loop again

      printf("Got %d bytes\n", rc);

      ++line;			// increment line count
      char *data = (char *)server->GetData();

      // format a message to send back
      char message[100];
      snprintf( message, sizeof(message), "Connection %d Line %d\n", conn, line);

      // send return data... should return length in bytes sent (-1 on error)
      rc = server->SendData( message, strlen(message), 0);

      if( rc <= 0) {
	printf("Socket send error\n");
	exit(1);
      }

    }

    printf("Connection %d closed\n", conn);
  }



}
