/* Interface: controllo_conto_corrente_server porta */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unitypes.h>
#include <unistr.h>
#include <unistdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include "../include/utils.h"
#include "../include/csexp.h"
#include "../include/main_func.h"
#include "../include/extraio.h"

void CHLD_handler (int signo);

int main (int argc, char** argv){
    int sd, err, on;
    struct addrinfo hints, *res;
    struct sigaction sa;

    /* Controllo parametri */
    if (argc != 2){
        perror ("Usage: bollettino_neve_server porta\n");
        exit(EXIT_FAILURE);
    }
    if(is_port(argv[1]) < 0){
        perror ("port have to be number of a service port\n");
        exit (EXIT_FAILURE);
    }

    /* Direttive per getaddrinfo */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    /* Risoluzione nome */
    if((err = getaddrinfo (NULL,argv[1], &hints, &res)) != 0){
        fprintf(stderr,"Naming error: %s\n", gai_strerror(err));
        exit(EXIT_FAILURE);
    }

    /* socket creation */
    if ((sd=socket(res->ai_family, res->ai_socktype, res->ai_protocol))<0){
        perror ("Socket creation error\n");
        exit(EXIT_FAILURE);
    }

    /* set socket option */
    on=1;
    if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    /* binding */
    if (bind(sd,res->ai_addr, res->ai_addrlen)<0){
        perror ("binding error\n");
        exit (EXIT_FAILURE);
    }

    freeaddrinfo(res);

    /* Passive socket create */
    if (listen(sd,SOMAXCONN)<0){
        perror("listening error\n");
        exit (EXIT_FAILURE);
    }

    /* signal child gestion */
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; /* per fare ripartire accept in caso di arrivo SIGCHLD */
    sa.sa_handler = CHLD_handler;

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    /*** connessione socket passiva avvenuta con successo ***/

    /*** inizio elaborazione richieste dei client ***/
    while(1){
        int ns;
        pid_t pid;

        /* creazione socket passiva */
        if((ns=accept(sd, NULL, NULL))<0){
            perror("accept");
            exit(EXIT_FAILURE);
        }

        /* Processo figlio di gestione della richiesta */
        if ((pid=fork())<0){
            perror ("fork");
            exit (EXIT_FAILURE);
        }
        if (pid==0){
            int pd_result[2];
            uint8_t *netIn, *netOut, *message[2];
            char *pipe_buff[1];

            /* socket passiva non gestita da questo processo */
            if (close(sd)<0){
                perror ("close");
                exit (EXIT_FAILURE);
            }

            /* lettura da socket di categoria fintanto che il server tiene aperta la socket */
            while(readmex(ns, &netIn)>0){
                int Nread, err;
                struct stat sb;
                char file_name[128];
                
                if ((err=getmexinfo(message, 2, netIn))<0){
                    fprintf (stderr, "getmexinfo error: number %d\n", err);
                    exit(EXIT_FAILURE);
                }
                
                /* creazione pipe */
                if (pipe(pd_result)<0){
                    perror ("pipe");
                    exit (EXIT_FAILURE);
                }
                
                /* eseguzione dei comandi specifici */
                execute (pd_result, message[0], message[1]);
                
                /* chiusura canale di scrittura su pipe */
                if (close (pd_result[1])<0){
                    perror ("close");
                    exit (EXIT_FAILURE);
                }

                /*** lettura risultato da pipe ***/
                /* allocazione memoria pari alla dimensione intera del file (+1 per '\0') */
                ulc_sprintf (file_name, "/home/federico/bollettino_neve/%U.txt", message[0]);
                /* ulc_sprintf (file_name, "/var/local/bollettino_neve/%U.txt", message[0]); */
                if (stat(file_name, &sb)<0){
                    perror ("stat error");
                    exit (EXIT_FAILURE);
                }
                pipe_buff[0] = malloc (sb.st_size +1);
                /* lettura vera e propria dalla pipe */
                if ((Nread=(int)read_all(pd_result[0],pipe_buff[0], (size_t)sb.st_size))<0){
                    perror ("read_all from pipe error");
                    exit (EXIT_FAILURE);
                }
                /* inserisco NULL come terminatore */
                pipe_buff[0][Nread]=0;

                /* chiusura canale di lettura da pipe */
                if (close (pd_result[0])<0){
                    perror ("close");
                    exit (EXIT_FAILURE);
                }

                /** CREAZIONE E INVIO MESSAGGIO **/
                netOut = createmex_c(pipe_buff,1);

                /* free pipe_buff */
                free (pipe_buff[0]);

                /* Scrittura su socket del risultato */
                if (write_all(ns, netOut, u8_strlen(netOut))<0){
                    perror ("error in write_all");
                    exit (EXIT_FAILURE);
                }
            }

            /* chiusura socket attiva processo figlio */
            if (close (ns)<0){
                perror ("close active socket");
                exit (EXIT_FAILURE);
            }
            printf ("End service\n");
            exit(EXIT_SUCCESS);
        }

        /* chiusura socket attiva processo padre */
        if (close(ns)<0){
            perror ("close active socket");
            exit (EXIT_FAILURE);
        }
        printf("New request can be accept\n");
    }
}

void CHLD_handler (int signo){
    int status;
    
    (void) signo;
    while (waitpid(-1,&status,WNOHANG)>0);
}