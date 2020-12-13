#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <limits.h>
#include <unitypes.h>
#include <unistr.h>
#include <unistdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_LEN 50
/* #define _POSIX_C_SOURCE 200809L */

/**
 *    Prova a connettersi a tutti gli indirizzi in lista, se non riesce ritorna -1
 **/
int socket_fallback (struct addrinfo *res);

/*
	is_port verifica se la stringa passata a parametro corrisponde ad un numero accettabile perchè sia una porta.
	restituisce:
	- -1 se la stringa non è puramente numerica
	- -2 se il numero non è accettabile
	- numero di porta se il parametro è accettabile
*/
int is_port (char* port_string);
