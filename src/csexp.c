#include "../include/csexp.h"

int Ndigit (int number);

/**
 * createmex_u8 receive an array of UTF-8 string and the length (N_string) of array; return the 
 * address pointer of the uint8_t string with the message.
 * the message use a Canonical S-Expression protocol.
 * 
 * NB : this message is in C format string ('\0' terminated), send without it
 **/
uint8_t* createmex_u8 (uint8_t **string, int N_string){
    uint8_t *output_temp, *output;
    int i, len;
    
    /* Null set & '(' to start message */
    output = malloc(2);
    u8_sprintf(output, "(");

    /* add all data to message */
    for (i=0; i<N_string; i++){
        /* Data length */
        len=u8_strlen(string[i]);

        output_temp = malloc(u8_strlen(output)+len+Ndigit(len)+2);
        /* new output_prov */
        u8_sprintf (output_temp, "%U%d:%U", output, len, string[i]);

        /* output_temp -> output */ 
        free (output);
        output = output_temp;
    }

    /* terminator char insert */ 
    output_temp = malloc(u8_strlen(output)+2);
    u8_sprintf (output_temp, "%U)", output);
    free(output);
    output = output_temp;
    return output;
}

/**
 * createmex_c receive an array of string and the length (N_string) of array; return the address 
 * pointer of the uint8_t string with the message.
 * the message use a Canonical S-Expression protocol.
 * 
 * NB : this message is in C format string ('\0' terminated), send without it
 **/
uint8_t* createmex_c (char **string, int N_string){
    uint8_t *output_temp, *output;
    int i, len;
    
    /* Null set & '(' to start message */
    output = malloc(2);
    u8_sprintf(output, "(");

    /* add all data to message */
    for (i=0; i<N_string; i++){
        /* Data length */
        len=strlen(string[i]);

        output_temp = malloc(u8_strlen(output)+len+Ndigit(len)+2);
        /* new output_prov */
        u8_sprintf (output_temp, "%U%d:%s", output, len, string[i]);

        /* output_prov -> output */ 
        free (output);
        output = output_temp;
    }

    /* terminator char insert */ 
    output_temp = malloc(u8_strlen(output)+2);
    u8_sprintf (output_temp, "%U)", output);
    free(output);
    output = output_temp;
    return output;
}

/**
 * readmex read from fd one message
 * 
 * Return length of message.
 * Return -1 if fd is empty.
 * Return -2 if message is invalid message.
 **/
int readmex (int fd, uint8_t **buff){
    int i=0;
    int end=0;
    uint8_t *buff_temp = NULL;

    *buff = malloc(1);

    /* starter char reading */
    if (read(fd, *buff, 1) <= 0){
        return (-1);
    }
    if ((char) (*buff)[0] != '('){
        return (-2);
    }
    i++;

    /* all data reading (end=1 if the first char of data is ')') */
    while (!end){
        int j=0;
        char len_c[15], *endptr;
        long len;
        uint8_t len_u8[15];

        /* read char by char data length (stop reading when the read char is ':') */
        do{
            if (read(fd, len_u8+j, 1) <= 0){
                return (-1);
            }
            /* if it reads ')' the message is finished */
            if(((char)len_u8[j]) == ')'){
                end=1;
            }
            j++;
        }while (((char) len_u8[j-1]) != ':' && !end);

        /* copy to buffer */
        buff_temp = malloc (i+j);
        u8_cpy(buff_temp, *buff, i);
        u8_cpy(buff_temp+i, len_u8, j);
        free (*buff);
        *buff = buff_temp;
        i += j;

        /* read data (data length: len) */
        if (!end){
            int Nread;

            len_u8[j]=0;
            ulc_sprintf (len_c, "%U", len_u8);

            j--;
            len = strtol(len_c, &endptr, 10);
            if (len == 0 && errno == EINVAL){
                fprintf (stderr, "No number for conversion\n");
                return (-2);
            }
            if (errno == ERANGE){
                len == LONG_MIN ? fprintf (stderr, "Conversion underflow\n") : fprintf (stderr, "Conversion overflow\n");
                return (-2);
            }
            if (*endptr != ':'){
                fprintf (stderr, "Error in numeric conversion\n");
                return (-2);
            }

            buff_temp = malloc (i+len);
            u8_cpy (buff_temp, *buff, i);
            free (*buff);

            if ((Nread=read_all(fd, buff_temp+i, len)) < 0){
                return (-1);
            }

            *buff=buff_temp;
            i += Nread;
        }
    }
    return i;
}

/**
 * getmexinfo put in output array the data of message in C format string ('\0' terminated).
 * N_output is the max number of element in output array.
 * 
 * Return -1 if output array buffer is too small.
 * Return -2 for conversion error.
 **/ 
int getmexinfo (uint8_t **output, int N_output, uint8_t *message){
    int i=1, j, len, current_el=0;
    long lenL;
    uint8_t datalen[5];
    char datalen_c[5];


    while((char)message[i] != ')'){
        char *endptr; /* end pointer fo strtol */

        /* length of output array control */
        if (current_el >= N_output){
            return (-1);
        }

        j=0;
        /* copy length of data in datalen */
        while((char)message[i] != ':'){
            datalen[j]=message[i];
            i++;
            j++;
        }
        datalen[j]=0;
        i++; /* now message[i] is the start char of data */

        ulc_sprintf (datalen_c, "%U", datalen);

        lenL = strtol (datalen_c, &endptr, 10);
        if (lenL == 0 && errno == EINVAL){
            fprintf (stderr, "No number for conversion\n");
            return (-2);
        }
        if (errno == ERANGE){
            lenL == LONG_MIN ? fprintf (stderr, "Conversion underflow\n") : fprintf (stderr, "Conversion overflow\n");
            return (-2);
        }
        if (*endptr != '\0'){
            fprintf (stderr, "WARNING: Conversion terminate in non NULL char\n");
            return (-2);
        }
        len=(int)lenL;

        /* data copy */
        output[current_el] = malloc(len+1);
        for (j=0; j<len; j++){
            output[current_el][j]=message[i];
            i++;
        }
        output[current_el][j]=0; /* '\0' insert for C format string */
        current_el++;
    }
    return 0;
}

/**
 * return an error string.
 **/
char* rm_getmessage (int err){
    char *err1;

    if (err == -1){
        err1 = malloc(sizeof(": fd is empty\n"));
        sprintf (err1, ": fd is empty\n");
    }

    if (err == -2){
        err1 = malloc(sizeof(": Message is invalid message\n"));
        sprintf (err1, ": Message is invalid message\n");
    }
    return err1;
}

int Ndigit (int number){
    int digit = 1;

    while ((number = number/10) > 0) digit++;

    return digit;
}