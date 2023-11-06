//
// Initial skeleton for new DOSI server
//
// Create a server on the default port defined in TcpServer.hh
// Listen for connections
// Look up commands and process them
//

#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <cstdint>

#include "ConfigCmdList.hh"
#include "DosiConfig.hh"
#include "ParseString.hh"
#include "DOSI_HW.hh"
#include "TcpServer.hh"
#include "DebugLog.hh"

int main( int argc, char *argv[]) {

  TcpServer* server = NULL;
  int conn;

  ConfigCmdList list;		// configuration command list (filled in by constructor)
  DosiConfig conf;		// DOSI server configuration
  DOSI_HW HW;			// Hardware interface (dummy for now)

  char buff[256];		// raw command line for parsing
  char *parv[10];		// parsed command as text tokens
  uint32_t pari[10];		// parsed command integer tokens

  char response[256];		// text response string

  // turn off non-error debugging messages
  DebugLog::setlogmask( LOG_UPTO( LOG_ERR));

  // initialize the server and listen for connections
  server = new TcpServer();
  DebugLog::log(LOG_INFO, "Server open on default port\n");

  // loop forever processing connections
  while( true) {

    // wait for connection
    do { conn = server->CheckConnect(); } while( !conn);

    DebugLog::log(LOG_INFO, "Got a connection (%d)...\n", conn);

    // wait for commands and process
    while( true) {

      // default response
      snprintf( response, sizeof(response), "OK\n");

      // check data, expect length or -1 (no data), 0 (closed)
      int rc = server->CheckData();

      if( rc == 0) break;	// connection closed
      if( rc < 0) continue;	// no data, try again

      char *data = (char *)server->GetData();
      DebugLog::log(LOG_DEBUG, "Got command: %s", data);

      // parse string into text and integer tokens
      int nt = parse_string( data, parv, pari, sizeof(pari)/sizeof(pari[0]));

      // no valid tokens (empty command?)
      if( !nt) continue;

      DebugLog::log(LOG_INFO, "Got %d tokens (%s)\n", nt, data);
      
      // look up the command in the list, return NULL if not found
      ConfigCmd* it = list.Search( (const char**)parv, nt);

      if( !it) {
	// command invalid
	DebugLog::log(LOG_INFO, "NOT FOUND\n");
	snprintf( response, sizeof(response), "ERR: invalid cmd \"%s\"\n", data);

      } else {

	// command is valid
	DebugLog::log(LOG_INFO, "FOUND\n");

	// switch based on command type
	switch( it->Type()) {
	case CMD_ERROR:
	  DebugLog::log(LOG_INFO, "Got ERROR type\n");
	  snprintf( response, sizeof(response), "ERR: bad command\n");
	  break;

	case CMD_CONF:		// edit configuration
	  DebugLog::log(LOG_INFO, "Got CONF type\n");
	  // run the lambda function bound to the item
	  it->action( (const char**)parv, pari, &conf, &HW);
	  break;

	case CMD_HW:		// immediate hardware action
	  DebugLog::log(LOG_INFO, "Got HW type\n");
	  // run the lambda function bound to the item
	  it->action( (const char**)parv, pari, &conf, &HW);
	  break;

	case CMD_GO:		// start a run
	  DebugLog::log(LOG_INFO, "Got GO type\n");
	  // check for configuration errors first
	  const char *msg = conf.checkErrors();
	  if( msg)
	    // if there is an error, send back a message
	    snprintf( response, sizeof(response), "ERR: %s\n", msg);
	  else {
	    // no error, run the lambda function bound to the item
	    it->action( (const char**)parv, pari, &conf, &HW);
	    snprintf( response, sizeof(response), "OK\n");
	  }
	  break;
	}
      }


      DebugLog::log(LOG_INFO, "RESP: %s", response);

      // send return data... should return length in bytes sent (-1 on error)
      rc = server->SendData( response, strlen(response), 0);

      if( rc <= 0) {
	DebugLog::log(LOG_INFO, "Socket send error\n");
	exit(1);
      }


    } // while(true) - commands

    DebugLog::log(LOG_INFO, "Connection %d closed\n", conn);
  } // while(true) - connections
}
