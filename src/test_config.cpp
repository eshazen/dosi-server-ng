#include <cstdint>

#include "ConfigParser.hh"
#include "DosiConfig.hh"

int main( int argc, char *argv[]) {

  ConfigItemList list;
  DosiConfig conf;
  ConfigItem* it;

  list.AddItem( new ConfigItem( "sn", Int1, &conf.minFreq, 1));
  list.AddItem( new ConfigItem( "sx", Int1, &conf.maxFreq, 1));
  list.AddItem( new ConfigItem( "ss", Int1, &conf.step, 1));
  list.AddItem( new ConfigItem( "sf", Int1, &conf.numFreqs, 1));

  list.AddItem( new ConfigItem( "sw", Int1, &conf.numSweeps, 1));
  list.AddItem( new ConfigItem( "sa", Int1, &conf.samplesPerFreq, 1));
  list.AddItem( new ConfigItem( "sp", Int1, &conf.SDsep, 1));
  list.AddItem( new ConfigItem( "sy", Int1, &conf.sweepDelay, 1));
  list.AddItem( new ConfigItem( "sv", Int1, &conf.ptsToAvg, 1));
  list.AddItem( new ConfigItem( "st", Int1, &conf.saveTime, 1));
  list.AddItem( new ConfigItem( "sc", Int1, &conf.calibrating, 1));

  list.AddItem( new ConfigItem( "ms", Constant, &conf.mode, SINGLE_TONE ));
  list.AddItem( new ConfigItem( "mt", Constant, &conf.mode, TIME));
  list.AddItem( new ConfigItem( "mr", Constant, &conf.mode, REIM));
  list.AddItem( new ConfigItem( "ma", Constant, &conf.mode, AMPPHASE));
  list.AddItem( new ConfigItem( "mc", Constant, &conf.mode, CAL_REIM));
  list.AddItem( new ConfigItem( "mp", Constant, &conf.mode, CAL_AMPPHASE));
  list.AddItem( new ConfigItem( "mo", Constant, &conf.mode, OP));
  list.AddItem( new ConfigItem( "mh", Constant, &conf.mode, CHROME));
  list.AddItem( new ConfigItem( "mu", Constant, &conf.mode, DUAL_APD));
  list.AddItem( new ConfigItem( "mv", Constant, &conf.mode, DUAL_APD_TRIGGER));

  list.AddItem( new ConfigItem( "t", Int1, &conf.triggerMode, 1));

  list.AddItem( new ConfigItem( "sd", Laser, &conf.whichDiodes, 1));  
  
  list.AddItem( new ConfigItem( "pam", "c", PGAMode, &conf.pgaModeA, CONST));
  list.AddItem( new ConfigItem( "pam", "l", PGAMode, &conf.pgaModeA, LINEAR));
  list.AddItem( new ConfigItem( "pam", "g", PGAMode, &conf.pgaModeA, LOG));
  list.AddItem( new ConfigItem( "pam", "p", PGAMode, &conf.pgaModeA, PTBYPT));

  list.AddItem( new ConfigItem( "pbm", "c", PGAMode, &conf.pgaModeB, CONST));
  list.AddItem( new ConfigItem( "pbm", "l", PGAMode, &conf.pgaModeB, LINEAR));
  list.AddItem( new ConfigItem( "pbm", "g", PGAMode, &conf.pgaModeB, LOG));
  list.AddItem( new ConfigItem( "pbm", "p", PGAMode, &conf.pgaModeB, PTBYPT));

  list.Print();

}
