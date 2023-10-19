// DosiConfig.hh
//
// manage server configuration
//

#ifndef DOSI_CONFIG_HH
#define DOSI_CONFIG_HH

// code types of configuration commands
typedef enum {
  CMD_TYPE_CONFIG,		// configuration commands, set internal states
  CMD_TYPE_GO,			// uniquely the "G" command to start a run
  CMD_TYPE_LASER,		// laser enable/disable functions
  CMD_TYPE_GPIO,		// misc GPIO (APD select, spectrometer, etc)
  CMD_TYPE_ERROR		// error in parsed command
} cmd_type_t;

typedef enum {
  SINGLE_TONE=0,
  TIME=1, 
  REIM=2, 
  AMPPHASE = 3, 
  CAL_REIM = 4, 
  CAL_AMPPHASE = 5, 
  OP = 6, 
  CHROME = 7,
  DUAL_APD = 8,
  DUAL_APD_TRIGGER = 9,
  DELAY_TRIGGER = 10,
} process_mode_t;


class DosiConfig {

public:
  DosiConfig();			  // constructor
  ~DosiConfig();		  // destructor
  int parseCommandPacket( char* buffer);

private:
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


cmd_type_t checkCommandType( char *buffer);



#endif  
