//
// sim_dosi.c
//
// simple simulator for the DOSI server
// for now, just receive and print to stdout all commands
// on 'g' send a fixed string
//

#define FIXED_RESPONSE_STRING "HELLO FROM DOSI"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include <math.h>

#define SERVER_PORT	27015
#define MAX_PENDING     1
#define MAX_LINE	256


typedef enum { false = 0, true = 1 } bool;

int
main(int argc, char *argv[])
{
  struct sockaddr_in sock_in;
  char buf[MAX_LINE];
  char reply[MAX_LINE];

  uint32_t fakeWF[MAX_LINE];

  int conn, line;
  int len;
  int s, new_s;
  int c_one = 1;
  int rc;

  char mode;

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
    fprintf(stderr, "%s: socket - %s\n", argv[0], strerror(errno));
    exit(1);
  }

  /* set the "reuse" option */
  rc = setsockopt( s, SOL_SOCKET, SO_REUSEADDR, &c_one, sizeof(c_one));

  /*
   * Bind an address to the socket
   */
  if (bind(s, (struct sockaddr *)&sock_in, sizeof(sock_in)) < 0) {
    fprintf(stderr, "%s: bind - %s\n", argv[0], strerror(errno));
    exit(1);
  }

  /*
   * Set the length of the listen queue
   */
  if (listen(s, MAX_PENDING) < 0) {
    fprintf(stderr, "%s: listen - %s\n", argv[0], strerror(errno));
    exit(1);
  }

  conn = 0;
	
  /*
   * Loop accepting new connections and servicing them
   */
  while (true) {
    len = sizeof(sock_in);
    if ((new_s = accept(s, (struct sockaddr *)&sock_in, &len)) < 0) {
      fprintf(stderr, "%s: accept - %s\n",
	      argv[0], strerror(errno));
      exit(1);
    }
    ++conn;
    line = 0;
    while ((len = recv(new_s, buf, sizeof(buf), 0)) > 0) {
      ++line;
      if (len >= MAX_LINE) {
	len = MAX_LINE - 1;
      }
      buf[len] = 0;
      printf("RECV: \"%s\"\n", buf);

      if( toupper( *buf) == 'M') {
	mode = toupper( buf[1]);
	printf("MODE = '%c'\n", mode);
      }

      if( toupper( *buf) == 'G') {
	printf("Go!\n");
	
	if( mode == 'T') {
	  // send fake waveform data
	  for( int i=0; i<MAX_LINE; i++) {
	    double x = ((double)i/MAX_LINE)*6.28*10;
	    double ya = 3000.0*sin(x)+8000.;
	    double yb = 1200.0*sin(x*3.2)+8024.;
	    fakeWF[i] = (uint16_t)ya | (((uint16_t)yb) << 16);
	  }

	  send( new_s, fakeWF, sizeof(fakeWF), 0);
	} else {
	  // send other random stuff in the array
	  send( new_s, fakeWF, 16, 0);
	}
	  

	len = strlen( reply);
	send( new_s, reply, len, 0);

	puts(reply);
      }
    }
    close(new_s);
  }
}
/*
 * End server.c
 */
