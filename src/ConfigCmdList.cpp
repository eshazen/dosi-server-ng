#include <cstdio>
#include "ConfigCmdList.hh"
#include "ParseString.hh"
#include "DOSI_HW.hh"

ConfigCmdList::ConfigCmdList() { InitializeList(); };
ConfigCmdList::~ConfigCmdList() { };


void ConfigCmdList::Print() {
  if( items.size()) {
    for( int i=0; i<items.size(); i++)
      items[i]->Print();
  } else {
    printf("Empty list\n");
  }
}

void ConfigCmdList::AddItem( ConfigCmd *it) {
  items.push_back( it);
}

int ConfigCmdList::Size() {
  return items.size();
}


ConfigCmd* ConfigCmdList::Get( int i)
{
  return items[i];
}


ConfigCmd* ConfigCmdList::Search( const char *pv[], int np) {
  int it;
  for( int i=0; i<Size(); i++)
    if( it = items[i]->Match( pv, np))
      return Get(i);
  return NULL;
}


// define come convenience macros to build the command list
// N.B. ConfFunc_args is defined in ConfigCmd.hh
#define LAMBDA(cod) []ConfFunc_args cod    // create a lambda expression given a code block
#define LIST_ADD1(t,s,cod)     AddItem( new ConfigCmd( t, s, LAMBDA(cod)))      // add 1-string command
#define LIST_ADD2(t,s1,s2,cod) AddItem( new ConfigCmd( t, s1, s2, LAMBDA(cod))) // add 2-string command

void ConfigCmdList::InitializeList() {

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

  LIST_ADD1( CMD_GO, "g", {hw->runDOSI( NULL);});
}
