#include "../include/main_func.h"

void execute (int *pd_return, uint8_t *region_u8, uint8_t *N_u8){
    char region[MAX_LEN], N[MAX_LEN];
    int pd_n1_n2[2];
    pid_t pid;

    ulc_sprintf(region, "%U", region_u8);
    ulc_sprintf(N, "%U", N_u8);

    /* creazione pipe di comunicazione tra processi nipoti */
    if (pipe(pd_n1_n2)<0){
        perror ("pipe n1 to n2");
        exit (EXIT_FAILURE);
    }

    /* figlio per sort */
    if ((pid=fork())<0){
        perror ("sort process error");
        exit (EXIT_FAILURE);
    }
    if(pid == 0){
        char path[1024];

        /* chiusura terminale di lettura da pipe */
        if (close (pd_return[0])<0){
            perror ("close pipe");
            exit(EXIT_FAILURE);
        }
        /* ridirezioni opportune */
        if (close(pd_return[1])<0){
            perror ("close");
            exit(EXIT_FAILURE);
        }
        if (close(pd_n1_n2[0])<0){
            perror ("close");
            exit(EXIT_FAILURE);
        }
        if (close(1)<0){
            perror ("close");
            exit(EXIT_FAILURE);
        }
        if (dup(pd_n1_n2[1])<0){
            perror ("close");
            exit(EXIT_FAILURE);
        }

        /* sprintf (path, "/var/local/bollettino_neve/%s.txt", region); */
        /* execlp ("sort", "sort", "-r", , path, NULL); */
        sprintf (path, "/home/federico/bollettino_neve/%s.txt", region);
        execlp ("sort", "sort", "-r", "-k3", "-n", path, NULL); /* use this for test */
        perror("execlp");
        exit(EXIT_FAILURE);
    }

    /* figlio per head */
    if ((pid=fork())<0){
        perror ("head process error");
        exit (EXIT_FAILURE);
    }
    if(pid == 0){
        char parameter[1024];

        /* chiusura terminale di lettura da pipe */
        if (close (pd_return[0])<0){
            perror ("close pipe");
            exit(EXIT_FAILURE);
        }
        /* ridirezioni opportune */
        if (close(pd_n1_n2[1])<0){
            perror ("close");
            exit(EXIT_FAILURE);
        }
        if (close(0)<0){
            perror ("close");
            exit(EXIT_FAILURE);
        }
        if (dup(pd_n1_n2[0])<0){
            perror ("close");
            exit(EXIT_FAILURE);
        }
        if (close(1)<0){
            perror ("close");
            exit(EXIT_FAILURE);
        }
        if (dup(pd_return[1])<0){
            perror ("close");
            exit(EXIT_FAILURE);
        }

        sprintf (parameter, "-n%s", N);
        execlp ("head", "head", parameter, NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }

    /* processo figlio non necessita della pipe di comunicazione dei nipoti */
    if (close(pd_n1_n2[0])<0){
        perror ("close");
        exit (EXIT_FAILURE);
    }
    if (close(pd_n1_n2[1])<0){
        perror ("close");
        exit (EXIT_FAILURE);
    }
}