/*
 * BlaBla
 */

// #include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"
#include "db.h"

int main(int argc, char *argv[]) {

    int sock_fd, new_fd;
    int server_port = SERVER_DEFAULT_PORT;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    mongoc_client_t *db_client;

    char data[BUFFER_LEN];

    if (argc == 2) {
        server_port = atoi(argv[1]);
    }

    db_client = connect_db();

    // create socket
    if ((sock_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        printf("Socket creation failed!\n");
        exit(1);
    }
    printf("Socket created.\n");

    // configure
    memset(&server_addr, 0, sizeof server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
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
    printf("Server listening on port: %d :)\n", server_port);
        
    while (1) {
        addr_size = sizeof(client_addr);
        if ((new_fd = accept(sock_fd, (struct sockaddr *) &client_addr, &addr_size)) != -1) {
            printf("Connection accepted!\n");

            if (!fork()) { // 0 -> child proccess
                close(sock_fd);
                
                handle_messages(new_fd, db_client);
                
                printf("Closing connection..\n");
                close(new_fd);
                exit(0);
            }
            close(new_fd);
        }
    }

    // criar um fluxo pra sair do while
    disconnect_db(db_client);

    return 0;
}

void handle_messages(int curr_fd, mongoc_client_t *db_client) {
    
    char buffer[BUFFER_LEN];
    int shift = 0, buffer_filled, len_read;
    enum operations operation;

    while (1) {
        memset(buffer, 0, sizeof buffer);

        if (receive_message(curr_fd, buffer) == 0) {
            return;
        }
        
        // get operation
        shift = sizeof operation;
        memcpy(&operation, buffer, sizeof operation);

        // handle it
        switch (operation) {
            // todo: enviar msg de feedback ao final
            case REGISTER_PROFILE:
                printf("Adding new profile\n");
                // register_profile(&buffer[shift]);
                break;
            case ADD_EXPERIENCES:
                // add_experiences();
                break;
            case LIST_BY_COURSE:
                // list_by_course();
                break;
            case LIST_BY_SKILL:
                // list_by_skill():
                break;
            case LIST_BY_GRADUATION_YEAR:
                // list_by_graduation_year();
                break;
            case LIST_ALL:
                // list_all():
                break;
            case FIND_BY_EMAIL:
                // find_by_email();
                break;
            case DELETE_PROFILE:
                // delete_profile();
                break;
            case CLOSE_CONNECTION:
                send_message(curr_fd, "[Server] Closing connection. ;) See ya!\n");
                return;
            default:
                printf("Unknown operation: %d\n", operation);
                printf("Skipping...\n");
        }
    }
}

bool check_admin(char *username, char *password) {
    return strcmp(username, SERVER_ADMIN_USERNAME) == 0 && strcmp(password, SERVER_ADMIN_PASSWORD) == 0;
}
