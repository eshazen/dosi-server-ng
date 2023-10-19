
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
  
  

