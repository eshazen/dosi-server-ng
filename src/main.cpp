//
// test TCP server
//

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "TcpServer.hh"

int main( int argc, char *argv[]) {

  char buff[80];
  TcpServer* server = NULL;
  int conn;
  int rc;
  char *s;

  while( true) {

    printf("\n> ");
    fgets( buff, sizeof(buff), stdin);

    switch( toupper( *buff)) {
    case 'H':
      printf("Server test\n\n");
      printf("I - initialize\n");
      printf("X - destruct\n");

      printf("C - poll for connection\n");
      printf("D - poll for data\n");
      break;

    case 'I':
      server = new TcpServer();
      printf("Server open on default port\n");
      break;

    case 'X':
      if( server == NULL)
	printf("No server\n");
      else
	delete server;
      break;

    case 'C':
      if( server == NULL) {
	printf("No server\n");
	break;
      }

      if( (conn = server->CheckConnect()))
	printf("Got a connection (%d)...\n", conn);
      else
	printf("No connection waiting\n");
      break;

    case 'D':
      // check for data
      if( server == NULL) {
	printf("No server\n");
	break;
      }
      rc = server->CheckData();
      if( rc == 0) {
	printf("Connection closed\n");
      }
      
      if( rc > 0) {
	printf("got %d bytes\n", rc);
	s = (char *)server->GetData();
	if( s != NULL) {
	  for( int i=0; i<rc; i++) {
	    if( isprint(*s))
	      putchar(*s);
	    else
	      printf("[0x%x]", *s);
	    ++s;
	  }
	  printf("\n");
	}
      } else {
	printf("no data\n");
      }

    default:
      ;
    }

  }
}
