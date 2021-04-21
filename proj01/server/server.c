/* 
 * BlaBla
 */ 

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"
#include "../shared.h"

int main(int argc, char *argv[]) {

    int sock_fd, new_fd;
    int port = 3490; // todo: get from run
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    char data[BUFFER_LEN];

    // checar se argv == 2, todo: adicionar PORT
    /* if (argc != 2) {
        fprintf(stderr,"");
        exit(1);
    } */

    // create socket
    if ((sock_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        printf("Socket creation failed!\n");
        exit(1);
    }
    printf("Socket created.\n");

    // configure
    memset(&server_addr, 0, sizeof server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind
    if (bind(sock_fd, (struct sockaddr *) &server_addr, sizeof server_addr) != 0) {
        printf("Socket binding failed!\n");
        exit(1);
    }
    printf("Socket binded..\n");

    // listen
    if (listen(sock_fd, SERVER_MAX_CONNECTIONS) != 0){
        printf("Listening failed!\n");
        exit(1);
    }
    printf("Server listening...\n");
        
    while (1) {
        addr_size = sizeof(client_addr);
        if ((new_fd = accept(sock_fd, (struct sockaddr *) &client_addr, &addr_size)) != -1) {
            printf("Connection accepted!");

            if (!fork()) { // 0 -> child proccess
                close(sock_fd);
                
                handle_messages(new_fd);

                close(new_fd);
                exit(0);
            }
            close(new_fd);
        }
    }

    return 0;
}

void handle_messages(int curr_fd) {
    
    char data[BUFFER_LEN];
    int shift;
    enum operations operation;


}