
| Class         | Description                 |
|---------------|-----------------------------|
| `TcpServer`   | TCP/IP server               |
| `ParseString` | String parsing utilities    |
| `DosiConfig`  | Manage server configuration |
|               |                             |

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
            
