#include <cstdio>
#include "DosiConfig.hh"

DosiConfig::DosiConfig() {
  
}

DosiConfig::~DosiConfig() {

}

// local function and variables for range checking
bool OutOfRange( int v, int v_min, int v_max) {   return ( v < v_min || v > v_max); }
#define CheckRange( v, min, max, msg) if(OutOfRange(v,min,max)) return msg;

const char* DosiConfig::checkErrors() {
  //
  // NOTE FIXME:  needs a lot more error checking!
  //
  CheckRange( minFreq, 1, 1000, "minFreq (sn) out of range");
  CheckRange( maxFreq, minFreq, 1000, "maxFreq (sx) out of range");
  CheckRange( step, 1, 1000, "step (ss) out of range");

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
