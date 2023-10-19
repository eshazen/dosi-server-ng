## Classes in new server

| Class         | Description                 |
|---------------|-----------------------------|
| `TcpServer`   | TCP/IP server               |
| `ParseString` | String parsing utilities    |
| `DosiConfig`  | Manage server configuration |

Thoughts on structure of new code.

Create a new class `DOSI` which acts as a container for all the HW
classes `DDSH_hl`, `ADC` and `GPIO`.  The constructor can take care of
all the initialization of the hardware (also providing a method like
`hardwareInit()` to force re-initialization).  It should also have
generic methods like `selectAPD()` and `enableLasers()`.

`DosiConfig` class takes the place of the old struct.  No public
members.  Constructor sets all values to invalid settings so
everything must be set from a configuration file.  Provide a
`checkValid()` method to ensure this is so.  Also provide print
methods (text and maybe HTML) to display the configuration.

### Command Parsing

This is tricky, as the command structure is not very logical.
There are 3 categories of commands:  those which modify the
configuration, those which act immediately on the hardware, and the
"GO" command to start a run.

At the very least, we need a parser which can check a command for
validity and identify which category the command falls in.
Additionally

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
            
