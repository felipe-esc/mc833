/*
 * Blabla
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shared.h"

void send_message(int fd, char *msg, int msg_len) {

    int len_sent, total_sent = 0, str_len;
    char buffer[BUFFER_LEN];

    if (msg_len == -1) {
        str_len = strlen(msg);
    } else {
        str_len = msg_len;
    }
    
    memset(buffer, 0, sizeof buffer);
    memcpy(buffer, msg, str_len);

    while (total_sent < BUFFER_LEN) {
        if ((len_sent = send(fd, &buffer[total_sent], (BUFFER_LEN - total_sent), 0)) > 0) {
            total_sent += len_sent;
        } else if (len_sent == 0) {
            printf("The other side shut down connection.\n");
            exit(0);
        } else {
            printf("Error sending message!\n");
            exit(1);
        }
    }
}

void receive_message(int fd, char *msg) {
    
    int len_read, buffer_filled = 0;
    
    while (buffer_filled < BUFFER_LEN) {
        if ((len_read = recv(fd, &msg[buffer_filled], (BUFFER_LEN - buffer_filled), 0)) > 0) {
            buffer_filled += len_read;
        } else if (len_read == 0) {
            printf("The other side shut down connection.\n");
            exit(0);
        } else {
            printf("Error reading message!\n");
            exit(1);
        }
    }
}
