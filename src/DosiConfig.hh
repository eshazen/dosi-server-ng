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
  CONST = 30,
  LINEAR = 31,
  LOG = 32,
  PTBYPT = 33
};

enum process_mode_t {
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
  DosiConfig();			  // constructor
  ~DosiConfig();		  // destructor

  // the server state
  uint32_t minFreq;
  uint32_t maxFreq;
  uint32_t step;
  uint32_t numFreqs;
  uint32_t numDiodes;
  uint32_t numSweeps;
  uint32_t samplesPerFreq;
  uint32_t SDsep;
  uint32_t sweepDelay;
  uint32_t ptsToAvg;
  uint32_t triggerMode;
  uint16_t DDSamp[6];
  bool saveTime;
  bool whichDiodes[6];
  bool calibrating;

  process_mode_t mode;
  pgaMode_t pgaModeA;
  pgaMode_t pgaModeB;
  uint32_t *pgaInA;
  uint32_t *pgaInB;

  bool debug = false;
};


#endif  
