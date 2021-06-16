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
            perror("sendto");
            return; // don't know if it's right
        }
    }
}
