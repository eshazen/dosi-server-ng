//
// Demo/test the command parser
//

#include <cstdint>
#include <cstring>
#include <cstdio>

#include "ConfigCmdList.hh"
#include "DosiConfig.hh"
#include "ParseString.hh"
#include "DOSI_HW.hh"


int main( int argc, char *argv[]) {

  ConfigCmdList list;		// configuration command list (filled in by constructor)
  DosiConfig conf;		// DOSI server configuration
  DOSI_HW HW;			// Hardware interface (dummy for now)

  char buff[80];		// raw command line for parsing
  char *parv[10];		// parsed command as text tokens
  uint32_t pari[10];		// parsed command integer tokens

 //---- main loop to let user type commands ----

  while( 1) {
    fgets( buff, sizeof(buff), stdin);
    // parse string into text and integer tokens
    int nt = parse_string( buff, parv, pari, sizeof(pari)/sizeof(pari[0]));
    if( nt) {
      // look up the command
      ConfigCmd* it = list.Search( (const char**)parv, nt);
      if( it) {
	printf("Found.  Type=%d\n", it->Type());
	// run the lambda function bound to the item
	it->action( (const char**)parv, pari, &conf, &HW);
	// if it changed the configuration, print
	if( it->Type() == CMD_CONF) {
	  // check for errors
	  const char *msg = conf.checkErrors();
	  if( msg)
	    printf("ERROR(s): %s\n", msg);
	  else
	    printf("Config OK\n");
	  conf.Print();
	}
      } else {
	printf("Not found\n");
      }
    } else {
      // nothing typed, just dump the command list
      printf("----\n");
      list.Print();
      printf("----\n");
    }
  }
}
