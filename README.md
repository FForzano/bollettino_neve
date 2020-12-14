# bollettino_neve

This project is an example of Client-Server application.
The server is in Posix-C, the Client both in Posix-C and Java.
I tried to implement Canonical S-Expression protcol for comunication.

Code example in this project:
- Socket stream C and Java
- Passive socket for server
- use of getaddrinfo
- Concurrent Server
- use of sigaction for signal gestion
- use of libunistring library for uint8_t string manipulation
- use of strtol
- Implementation of Canonical S-Expression (csexp)

___

The program simulate the request of information about ski location in Italy.
User insert the region and the number of ski resort and the server have to send the information.

(I supposed the info are in /usr/local/bollettino_neve/<region>.txt)

___
Applicative protocol:

Client---------------------------Server

    (<len>:<region><len>:<number>)
    ----------------------------->

            result_info
    <-----------------------------
