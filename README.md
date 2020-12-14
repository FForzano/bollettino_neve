# bollettino_neve

This project is an example of Client-Server application.
The server is in Posix-C, the Client both in Posix-C and Java.
I tried to implement Canonical S-Expression protcol for comunication.

Please send me code reviews and suggestion, I'm a student.

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
## The program:

The program simulate the request of information about ski location in Italy.
User insert the region and the number of ski resort and the server have to send the request informations.
(I supposed the info are in /usr/local/bollettino_neve/<region>.txt)

Client end when user write "fine".

For more info about the program see ./doc/Testo_Esercizio_11.txt

(NB: complete text of exercise is in italian, i'll translate it).

___
## Applicative protocol:

Client --------------------------- Server

    (<len>:<region><len>:<number>)
    ----------------------------->

            result_info
    <-----------------------------
