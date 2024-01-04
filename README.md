# TCP_Chat_Communication_C_Program
This repository hosts a comprehensive C program for establishing chat communication using the TCP protocol between client and server. The program facilitates real-time text-based interaction over a network, presenting a foundational platform for building robust communication systems.

Note:
Both the server and the client should be on the same network.
For the proper functioning of this code TOPOLOGY: SERVER(LAN CLIENT-1)------ONT------CLIENT(LAN CLIENT-2)
You need to modify the client-side code to work properly. At line number 27: If (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) = 0), configure a server-side LAN IP here."
