/*
 * BlaBla
 * Refs:
 *  -   http://beej.us/guide/bgnet/translations/bgnet_ptbr.pdf
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
    char *mongo_uri = NULL;
    mongoc_client_t *db_client;

    char data[BUFFER_LEN];

    if (argc >= 2) {
        server_port = atoi(argv[1]);
    }
    if (argc == 3) {
        mongo_uri = malloc(sizeof(char) * 100);
        memcpy(mongo_uri, argv[2], strlen(argv[2]));
    }

    db_client = connect_db(mongo_uri);

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
    close(sock_fd);

    return 0;
}

void handle_messages(int curr_fd, mongoc_client_t *db_client) {
    
    char buffer[BUFFER_LEN];
    int shift = 0, buffer_filled, len_read;
    enum operations operation;

    while (1) {
        memset(buffer, 0, sizeof buffer);

        receive_message(curr_fd, buffer);
        
        // get operation
        shift = sizeof operation;
        memcpy(&operation, buffer, sizeof operation);

        // handles it
        switch (operation) {
            case REGISTER_PROFILE:
                register_profile(curr_fd, &buffer[shift], db_client);
                break;
            case ADD_EXPERIENCES:
                add_new_experiences(curr_fd, &buffer[shift], db_client);
                break;
            case LIST_BY_COURSE:
                list_by_course(curr_fd, &buffer[shift], db_client);
                break;
            case LIST_BY_SKILL:
                list_by_skill(curr_fd, &buffer[shift], db_client);
                break;
            case LIST_BY_GRADUATION_YEAR:
                list_by_graduation_year(curr_fd, &buffer[shift], db_client);
                break;
            case LIST_ALL:
                list_all(curr_fd, db_client);
                break;
            case FIND_BY_EMAIL:
                find_by_email(curr_fd, &buffer[shift], db_client);
                break;
            case DELETE_PROFILE:
                delete_profile(curr_fd, &buffer[shift], db_client);
                break;
            case CLOSE_CONNECTION:
                send_message(curr_fd, "[SERVER] Closing connection. ;) See ya!\n\0", -1);
                return;
            default:
                printf("Unknown operation: %d\n", operation);
                printf("Skipping...\n");
        }
    }
}

bool check_admin(char *username) {
    // not working right
    return strcmp(username, SERVER_ADMIN_USERNAME) == 0;
}

void register_profile(int curr_fd, char *msg, mongoc_client_t *client) {
    
    char username[USERNAME_LEN];
    int shift;

    memset(username, 0, sizeof username);
    memcpy(username, msg, sizeof(username));
    if (!check_admin(username)) {
        send_message(curr_fd, "[SERVER] Ops! You must be admin to do this :(\n\0", -1);
        printf("User couldn't add profile, permission denied!\n");
        return;
    }
    printf("Adding new profile\n");

    shift = sizeof(char) * USERNAME_LEN;
    
    if (db_register_profile(&msg[shift], client) < 0) {
        send_message(curr_fd, "[SERVER] An unexpected error ocurred! Could not save profile :/\n\0", -1);
        printf("Failed saving new profile.\n");
        return;
    }
    
    send_message(curr_fd, "[SERVER] Profile successfully inserted! ;)\n\0", -1);
}

void add_new_experiences(int curr_fd, char *msg, mongoc_client_t *db_client) {
    printf("TODO: Implementar - %s\n", __func__);
    char username[USERNAME_LEN];

    memset(username, 0, sizeof username);
    memcpy(username, msg, sizeof(username));
    if (!check_admin(username)) {
        send_message(curr_fd, "[SERVER] Ops! You must be admin to do this :(\n\0", -1);
        printf("User couldn't add a new experience, permission denied!\n");
        return;
    }
    
    // separar dados

    // salvar

    // feedback ao client
}

void list_by_course(int curr_fd, char *msg, mongoc_client_t *db_client) {
    printf("TODO: Implementar - %s\n", __func__);
    // separar string

    // fazer a busca no db

    // enviar resposta ou feedback ao client

}

void list_by_skill(int curr_fd, char *msg, mongoc_client_t *db_client) {
    printf("TODO: Implementar - %s\n", __func__);
    // separar string

    // fazer a busca no db

    // enviar resposta ou feedback ao client

}

void list_by_graduation_year(int curr_fd, char *msg, mongoc_client_t *db_client) {
    printf("TODO: Implementar - %s\n", __func__);
    // separar string

    // fazer a busca no db

    // enviar resposta ou feedback ao client

}

void list_all(int curr_fd, mongoc_client_t *db_client) {

    char list[BUFFER_LEN];

    db_list_all(list, db_client);

    send_message(curr_fd, list, sizeof(list));
}

void find_by_email(int curr_fd, char *msg, mongoc_client_t *db_client) {
    printf("TODO: Implementar - %s\n", __func__);
    // separar string

    // fazer a busca no db

    // enviar resposta ou feedback ao client

}

void delete_profile(int curr_fd, char *msg, mongoc_client_t *db_client) {
    char username[USERNAME_LEN];

    memset(username, 0, sizeof username);
    memcpy(username, msg, sizeof(username));

    if (!check_admin(username)) {
        send_message(curr_fd, "[SERVER] Ops! You must be admin to do this :(\n\0", -1);
        printf("User couldn't delete a profile, permission denied!\n");
        return;
    }
    
    // separar dados

    // salvar

    // feedback ao client
}

