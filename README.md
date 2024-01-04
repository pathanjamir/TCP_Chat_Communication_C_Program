# TCP_Chat_Communication_C_Program

Welcome to the TCP Chat Communication C Program repository! This project provides a client-server solution for real-time text-based communication using the TCP protocol. The client and server programs enable users to engage in dynamic chat conversations over a network.

## Features
- **Chat Communication:** Seamless communication between clients and the server using the TCP protocol.
- **Server-Side:** The server handles incoming connections, facilitates chat interactions, and manages client communication.
- **Client-Side:** Clients can establish connections with the server and engage in chat conversations.
- **Utilities:** Shared utility functions streamline TCP socket operations and manage chat messages.

## Future Implementations
This repository also outlines planned enhancements for the project:
- **Resumable File Transfer:** Support for resuming interrupted file transfers.
- **Encryption:** Implementation of encryption mechanisms to secure messages and file transfers.
- **Compression:** Integration of compression algorithms for optimized file transfers.
- **Progress Bar:** Addition of a progress bar to visualize ongoing file transfers.
- **Timeouts:** Incorporation of timeout mechanisms to handle unresponsive clients.
- **User Authentication:** Implementation of user authentication for enhanced security.
- **Multi-threading:** Exploration of multi-threading to improve program concurrency.

## Getting Started
### Prerequisites
- GCC Compiler
- CMake (for building)
- (Add any other prerequisites here)

### Building
1. Clone this repository:
   ```bash
   git clone https://github.com/your-username/TCP_Chat_Communication_C_Program.git
   cd TCP_Chat_Communication_C_Program
###Note
-Both the server and the client should be on the same network.
-For the proper functioning of this code TOPOLOGY: SERVER(LAN CLIENT-1)------ONT------CLIENT(LAN CLIENT-2)
-You need to modify the client-side code to work properly. At line number 27: If (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) = 0), configure a server-side LAN IP here.
