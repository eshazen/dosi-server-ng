// DosiConfig.hh
//
// manage server configuration
//

#ifndef DOSI_CONFIG_HH
#define DOSI_CONFIG_HH

#include <cstdint>

// code types of configuration commands
enum cmd_type_t {
  CMD_TYPE_CONFIG,		// configuration commands, set internal states
  CMD_TYPE_GO,			// uniquely the "G" command to start a run
  CMD_TYPE_LASER,		// laser enable/disable functions
  CMD_TYPE_GPIO,		// misc GPIO (APD select, spectrometer, etc)
  CMD_TYPE_ERROR		// error in parsed command
};

enum pgaMode_t {
  PG_UNDEF = -1,
  CONST = 30,
  LINEAR = 31,
  LOG = 32,
  PTBYPT = 33
};

enum process_mode_t {
  PM_UNDEF=-1,
  SINGLE_TONE=10,
  TIME=11, 
  REIM=12, 
  AMPPHASE =13, 
  CAL_REIM =14, 
  CAL_AMPPHASE =15, 
  OP =16, 
  CHROME =17,
  DUAL_APD =18,
  DUAL_APD_TRIGGER =19,
  DELAY_TRIGGER = 20,
};


class DosiConfig {

public:
  DosiConfig();		       // constructor
  ~DosiConfig();	       // destructor
  void Print();		       // print to std output
  const char* checkErrors();   // NULL if OK, else ptr to error string

  // the server state, all invalid to begin
  uint32_t minFreq = -1;
  uint32_t maxFreq = -1;
  uint32_t step = -1;
  uint32_t numFreqs = -1;
  uint32_t numDiodes = -1;
  uint32_t numSweeps = -1;
  uint32_t samplesPerFreq = -1;
  uint32_t SDsep = -1;
  uint32_t sweepDelay = -1;
  uint32_t ptsToAvg = -1;
  uint32_t triggerMode = -1;
  uint16_t DDSamp[6];
  bool saveTime = -1;
  bool whichDiodes[6];
  bool calibrating = -1;

  process_mode_t mode = PM_UNDEF;
  pgaMode_t pgaModeA = PG_UNDEF;
  pgaMode_t pgaModeB = PG_UNDEF;
  uint32_t *pgaInA = NULL;
  uint32_t *pgaInB = NULL;

  char* raw_laser_config;

  bool debug = false;
};


#endif  
