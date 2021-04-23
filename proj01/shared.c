/*
 * Blabla
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shared.h"

int send_message(int fd, char *msg) {

    int len_sent, total_sent = 0;
    char buffer[BUFFER_LEN];
    
    memcpy(buffer, msg, strlen(msg));
    while (total_sent < BUFFER_LEN) {
        if ((len_sent = send(fd, &buffer[total_sent], (BUFFER_LEN - total_sent), 0)) > 0) {
            total_sent += len_sent;
        } else if (len_sent == 0) {
            printf("The other side shut down connection.\n");
            return 0;
        } else {
            printf("Error sending message!\n");
            exit(1);
        }
    }

    return len_sent; 
}

int receive_message(int fd, char *msg) {
    
    int len_read, buffer_filled = 0;
    
    while (buffer_filled < BUFFER_LEN) {
        if ((len_read = recv(fd, &msg[buffer_filled], (BUFFER_LEN - buffer_filled), 0)) > 0) {
            buffer_filled += len_read;
        } else if (len_read == 0) {
            printf("The other side shut down connection.\n");
        } else {
            printf("Error reading message!\n");
            exit(1);
        }
    }

    return len_read;
}
