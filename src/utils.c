#define _POSIX_C_SOURCE 200809L
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>

/**
 *    Prova a connettersi a tutti gli indirizzi in lista, se non riesce ritorna -1
 **/
int socket_fallback (struct addrinfo *res){
    struct addrinfo *ptr;
    int sd;

    /* tentativi di conessione */
    for (ptr=res; ptr != NULL; ptr=ptr->ai_next){
        /* se sd non è < 0 provo a connettere */
        if((sd = socket (ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol))>=0){
            if (connect(sd, ptr->ai_addr, ptr->ai_addrlen)==0){
                return sd; /* se la connessione va a buon fine ritorno il socket descriptor */
            }
            close (sd); /* se la connessione fallisce tento col successivo indirizzo */
        }
    }
    /* se nessun indirizzo è valido o, se valido, non riesce a connettersi ritorno -1 */
    return (-1);
}

/*
	is_port verifica se la stringa passata a parametro corrisponde ad un numero accettabile perchè sia una porta.
	restituisce:
	- -1 se la stringa non è puramente numerica
	- -2 se il numero non è accettabile
	- numero di porta se il parametro è accettabile
*/
int is_port (char* port_string){
    char *endptr;
	long port;

	port = strtol (port_string, &endptr, 10);

	if (port == 0 && errno == EINVAL){
		/* port_string is not numerical string */
		return (-1);
	}
	if ((port == LONG_MIN || port == LONG_MAX) && (errno == ERANGE)){
		/* overflow or underflow in conversion */
		return (-2);
	}
	if(port<0 || port > 65535){
		/* port_string is a number but it isn't a port */
		return (-2);
	}
	if (*endptr != '\0'){
		fprintf (stderr, "strtol terminated but string isn't terminated\n");
	}

	return (int) port;
}
