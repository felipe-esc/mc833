/*
 * Shared Functions UDP
 *
 * Responsibles: 
 *      Felipe Escórcio de Sousa - RA: 171043
 *      Ricardo Ribeiro Cordeiro - RA: 186633 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "shared.h"

/*
 *  Handles datagrams sendings
 */
void send_message(int fd, char *msg, int msg_len, struct sockaddr *addr) {

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
        if ((len_sent = sendto(fd, &buffer[total_sent], (BUFFER_LEN - total_sent), MSG_CONFIRM, addr, sizeof(struct sockaddr))) > 0) {
            total_sent += len_sent;
        } else {    
            printf("Error sending message! Probably message was lost or corrupted!\n");
            return;
        }
    }
}

/*
 *  Handles datagrams receivings
 */
void receive_message(int fd, char *msg, struct sockaddr *addr) {
    int len_read, buffer_filled = 0, fromlen;

    fromlen = sizeof(addr);
    
    while (buffer_filled < BUFFER_LEN) {
        if ((len_read = recvfrom(fd, &msg[buffer_filled], (BUFFER_LEN - buffer_filled), 0, addr, &fromlen)) > 0) {
            buffer_filled += len_read;
        } else {
            printf("Error reading message! Probably message was lost or corrupted!\n"); // don't know if it's right
            return;
        }
    }
}
