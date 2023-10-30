#include <cstdio>
#include "DosiConfig.hh"

DosiConfig::DosiConfig() {
  
}

DosiConfig::~DosiConfig() {

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
