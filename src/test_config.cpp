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

// define come convenience macros to build the command list
// N.B. ConfFunc_args is defined in ConfigCmd.hh
#define LAMBDA(cod) []ConfFunc_args cod    // create a lambda expression given a code block
#define LIST_ADD1(t,s,cod)     list.AddItem( new ConfigCmd( t, s, LAMBDA(cod)))      // add 1-string command
#define LIST_ADD2(t,s1,s2,cod) list.AddItem( new ConfigCmd( t, s1, s2, LAMBDA(cod))) // add 2-string command

int main( int argc, char *argv[]) {

  ConfigCmdList list;		// configuration command list
  DosiConfig conf;		// DOSI server configuration
  DOSI_HW HW;			// Hardware interface (dummy for now)

  char buff[80];		// raw command line for parsing
  char *parv[10];		// parsed command as text tokens
  uint32_t pari[10];		// parsed command integer tokens

  // construct the list of server commands.
  // Use LIST_ADD1() for single-token commands, LIST_ADD2() for two-token commands
  // Provide a code block, 
  //   refer to pi[] (integer tokens), pv[] (string tokens), hw (hardware), c (configuration)

  LIST_ADD1( CMD_CONF, "sn", {c->minFreq =  pi[1];});
  LIST_ADD1( CMD_CONF, "sx", {c->maxFreq =  pi[1];});
  LIST_ADD1( CMD_CONF, "ss", {c->step    =  pi[1];});
  LIST_ADD1( CMD_CONF, "sf", {c->numFreqs = pi[1];});

  LIST_ADD1( CMD_CONF, "sn", {c->minFreq = pi[1];});
  LIST_ADD1( CMD_CONF, "sx", {c->maxFreq = pi[1];});
  LIST_ADD1( CMD_CONF, "ss", {c->step = pi[1];});
  LIST_ADD1( CMD_CONF, "sf", {c->numFreqs = pi[1];});
  LIST_ADD1( CMD_CONF, "sw", {c->numSweeps = pi[1];});
  LIST_ADD1( CMD_CONF, "sa", {c->samplesPerFreq = pi[1];});
  LIST_ADD1( CMD_CONF, "sp", {c->SDsep = pi[1];});
  LIST_ADD1( CMD_CONF, "sy", {c->sweepDelay = pi[1];});
  LIST_ADD1( CMD_CONF, "sv", {c->ptsToAvg = pi[1];});
  LIST_ADD1( CMD_CONF, "st", {c->saveTime = pi[1];});
  LIST_ADD1( CMD_CONF, "sc", {c->calibrating = pi[1];});

  LIST_ADD1( CMD_CONF, "ms", {c->mode = SINGLE_TONE ;});
  LIST_ADD1( CMD_CONF, "mt", {c->mode = TIME;});
  LIST_ADD1( CMD_CONF, "mr", {c->mode = REIM;});
  LIST_ADD1( CMD_CONF, "ma", {c->mode = AMPPHASE;});
  LIST_ADD1( CMD_CONF, "mc", {c->mode = CAL_REIM;});
  LIST_ADD1( CMD_CONF, "mp", {c->mode = CAL_AMPPHASE;});
  LIST_ADD1( CMD_CONF, "mo", {c->mode = OP;});
  LIST_ADD1( CMD_CONF, "mh", {c->mode = CHROME;});
  LIST_ADD1( CMD_CONF, "mu", {c->mode = DUAL_APD;});
  LIST_ADD1( CMD_CONF, "mv", {c->mode = DUAL_APD_TRIGGER;});

 LIST_ADD1( CMD_CONF, "t", {c->triggerMode = pi[1];});

 LIST_ADD1( CMD_CONF, "sd", {
     if( parse_laser_string( pv[1], c->whichDiodes))
       { fprintf(stderr,"Bad laser config string: %s\n", pv[1]);   exit(1); }  });
  LIST_ADD2( CMD_CONF, "pam", "c", {c->pgaModeA = CONST;});
  LIST_ADD2( CMD_CONF, "pam", "l", {c->pgaModeA = LINEAR;});
  LIST_ADD2( CMD_CONF, "pam", "g", {c->pgaModeA = LOG;});
  LIST_ADD2( CMD_CONF, "pam", "p", {c->pgaModeA = PTBYPT;});
  LIST_ADD2( CMD_CONF, "pbm", "c", {c->pgaModeB = CONST;});
  LIST_ADD2( CMD_CONF, "pbm", "l", {c->pgaModeB = LINEAR;});
  LIST_ADD2( CMD_CONF, "pbm", "g", {c->pgaModeB = LOG;});
  LIST_ADD2( CMD_CONF, "pbm", "p", {c->pgaModeB = PTBYPT;});

  LIST_ADD1( CMD_GO, "g", {printf("START A RUN!\n");});

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
	if( it->Type() == CMD_CONF)
	  conf.Print();
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
