## ToDo

This will be a long list, no doubt!

* Combine `server.cpp` and `test_config.cpp` to make a demo server
  which can parse commands and return errors as appropriate.
* Check all `DosiConfig` members for correct range with reasonable
error messages.  Print errors to `stdout` and also (eventually) return
to the client.

## Files

    ConfigCmd.cpp            class: one configuration command
    ConfigCmdList.cpp        class: list of configuration commands
    DebugLog.cpp             class: simple debug message logger
    DosiConfig.cpp           class: DOSI configuration 
    DOSI_HW.cpp              class: manage DOSI hardware I/F
    interactive_test.cpp     app: interactive TCP server for debug
    ParseString.cpp          class: string parsing utilities
    server.cpp               app: simple stand-alone TCP server
    sim_dosi.c               app: standalone test server
    TcpServer.cpp            class: TCP server
    test_config.cpp          app: test ConfigCmdList class
    test_DebugLog.cpp        app: test DebugLog class


## Classes in new server

| Class           | Description                               |
|-----------------|-------------------------------------------|
| `DebugLog`      | Generic debug log facility like syslog    |
| `TcpServer`     | TCP/IP server                             |
| `ParseString`   | String parsing utilities                  |
| `DosiConfig`    | Server configuration (replace old struct) |
| `ConfigCmd`     | One configuration command                 |
| `ConfigCmdList` | List of all configuration commands        |
| `DOSI_HW`       | Control hardware (dummy for now)          |

Thoughts on structure of new code.

Create a new class `DOSI_HW` which acts as a container for all the HW
classes `DDSH_hl`, `ADC` and `GPIO`.  The constructor can take care of
all the initialization of the hardware (also providing a method like
`hardwareInit()` to force re-initialization).  It should also have
generic methods like `selectAPD()` and `enableLasers()`.

### DOSI_HW methods

* selectAPD()
* enableActiveLasers()
* disableAllLasers()
* setSpectrometer()
* runDOSI() ? or in another class?
* writeGPIO() ?

`DosiConfig` class takes the place of the old struct.  For now, all
public members.  Constructor sets all values to invalid settings so
everything must be set from a configuration file.  Provide a
`checkValid()` method to ensure this is so.  Also provide print
methods (text and maybe HTML) to display the configuration.

### Command Parsing

There are 3 categories of commands:  those which modify the
configuration, those which act immediately on the hardware, and the
"GO" command to start a run.

A list of all configuration commands is kept in `ConfigCmdList`.
The constructor calls `Initialize()` which builds the list for DOSI.
Commands are added with `AddItem()`.  Search for a command using
`Search()`.  See `test_config.cpp` for a complete example of how to
use this.

## Code / Class structure in old server

`main()`

Set up socket, listen, bind

Create instances of `DDSH_hl`, `ADC` and `GPIO`.  Call some setup functions on each.

`while(1):`

        Listen for connections
        `handleConnection( socket, DDS, ADC, GPIO)

`handleConnection()`

        Initialize DOSI_config with default settings
		Allocate some memory with new
		Read commands from socket until close
		   (read characters one at a time, look for \n)
	       call parseCommandPacket( DOSI_config, cmd, DDS, ADC, GPIO, socket) for each
		Free some memory

`parseCommandPacket()`

        parse commands and take action.  three types:
          configuration:  modify DOSI_config and return
          hardware (immediate action using GPIO)
            lasers on/off
            select APD
			spectrometer on/off
          go:  
            allocat some memory for calibration
            call runDOSI( config, DDS, ADC, GPIO, socket...)
            
