/* interface: bollettino_neve    server    porta */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unitypes.h>
#include <unistr.h>
#include <unistdio.h>
#include <sys/socket.h>
#include "../include/utils.h"
#include "../include/csexp.h"
#include "../include/extraio.h"

int main (int argc, char** argv){
    int sd, err;
    struct addrinfo hints, *res;
    char *usrIn[2]={'\0'};
    uint8_t *netIn;
    uint8_t *netOut;
    uint8_t *message[1]={0};

    usrIn[0]=malloc(MAX_LEN);
    usrIn[1]=malloc(MAX_LEN);

    /* Controllo parametri */
    if (argc != 3){
        perror ("Usage: bollettino_neve server porta\n");
        exit(EXIT_FAILURE);
    }
    if(is_port(argv[2]) < 0){
        perror ("port have to be number of a service port\n");
        exit (EXIT_FAILURE);
    }

    /* Direttive per getaddrinfo */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    /* Risoluzione nome */
    if((err = getaddrinfo (argv[1],argv[2], &hints, &res)) != 0){
        fprintf(stderr,"Naming error: %s\n", gai_strerror(err));
        exit(EXIT_FAILURE);
    }

    /* Connessione con fallback */
    if ((sd=socket_fallback(res))<0){
        perror("Socket creation or connection error\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(res);

    /*** CONNESSIONE AVVENUTA CON SUCCESSO ***/

    /*** Interfaccia utente ***/
    printf("Inserire la regione di interesse\n");
    scanf("%s", usrIn[0]);
    while (strcmp(usrIn[0],"fine")){
        printf("Inserire il numero di località da visualizzare\n");
        scanf("%s", usrIn[1]);

        /* creazione messaggio */
        netOut = createmex_c(usrIn, 2);

        /* invio messaggio */
        if(write_all(sd, netOut, u8_strlen(netOut))<0){
            perror("write_all");
            exit (EXIT_FAILURE);
        }

        /* lettura messaggio dal server */
        if((err=readmex(sd, &netIn))<0){
            fprintf (stderr, "readmex error: %s\n", rm_getmessage(err));
            exit (EXIT_FAILURE);
        }
        if(getmexinfo(message, 1, netIn)<0){
            fprintf(stderr, "getmexinfo error\n");
            exit (EXIT_FAILURE);
        }

        ulc_fprintf (stdout, "%U\n", message[0]);

        /* new user request */
        printf("Inserire la regione di interesse\n");
        scanf("%s", usrIn[0]);
    }


    if (close(sd)<0){
        perror ("close");
        exit (EXIT_FAILURE);
    }
}