//
// DosiConfig.cpp
//
// manage the DOSI box configuration (replaces the old DOSI_config struct)
// for now the members are public
//


#include <cstdio>
#include "DosiConfig.hh"
#include "ParseString.hh"

DosiConfig::DosiConfig() {
  
}

DosiConfig::~DosiConfig() {

}

// local function and variables for range checking
bool OutOfRange( int v, int v_min, int v_max) {   return ( v < v_min || v > v_max); }
#define CheckRange( v, min, max, msg) if(OutOfRange(v,min,max)) return msg;

const char* DosiConfig::checkErrors() {
  static char errmsg[30];

  //
  // NOTE FIXME:  needs a lot more error checking!
  //
  CheckRange( minFreq, 1, 1000, "minFreq (sn) out of range");
  CheckRange( maxFreq, minFreq, 1000, "maxFreq (sx) out of range");
  CheckRange( step, 1, 1000, "step (ss) out of range");

  if( parse_laser_string( raw_laser_config, whichDiodes)) {
    snprintf( errmsg, sizeof(errmsg), "Bad laser config: \"%s\"", raw_laser_config);
    return errmsg;
  }

  return NULL;
}

#define QUOTE(s) "\"" #s "\""
#define PRIT(s) printf("%20s %d\n", QUOTE(s), s);

void DosiConfig::Print() {
  PRIT(minFreq);
  PRIT(maxFreq);
  PRIT(step);
  PRIT(numFreqs);
  PRIT(numDiodes);
  PRIT(numSweeps);
  PRIT(samplesPerFreq);
  PRIT(SDsep);
  PRIT(sweepDelay);
  PRIT(ptsToAvg);
  PRIT(triggerMode);
  for( int i=0; i<sizeof(DDSamp)/sizeof(DDSamp[0]); i++)
    PRIT(DDSamp[i]);
  PRIT(saveTime);
  PRIT(calibrating);
  for( int i=0; i<sizeof(whichDiodes)/sizeof(whichDiodes[0]); i++)
    PRIT(whichDiodes[i]);
  PRIT(mode);
  PRIT(pgaModeA);
  PRIT(pgaModeB);
}



//
// set reasonable defaults for debug
//
void DosiConfig::DefaultValues() {
  
  //DEFAULT CONFIGURATION. YOU CAN CHANGE IT IF YOU WANT
  //All the run scripts will parse this configuration to set parameters
  minFreq = 50;
  maxFreq = 450;
  step = 5;
  numFreqs = 10;
  numSweeps = 1;
  for(uint16_t iter = 0; iter < 6; iter++){
    DDSamp[iter] = 0x3FFF;
  }  
  saveTime = false;
  samplesPerFreq = 8192;
  calibrating = false;
  SDsep = 10;
  sweepDelay = 0;
  ptsToAvg = 1; //1 means take 1 measurement, 0 would be no measurements
  //Set all diodes to true
  for (uint32_t j = 0; j<NUM_LASERS; j++) {
    whichDiodes[j] = true;
  }
  numDiodes = NUM_LASERS;
  mode = REIM;
  debug = false;
  pgaModeA = CONST;
  pgaModeB = CONST;
  //Set PGA to 20 for both channels
  //  pgaInA = pgaAtop;
  //  pgaInB = pgaBtop;
  triggerMode = 0;
}
