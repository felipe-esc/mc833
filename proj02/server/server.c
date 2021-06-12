/*
 * Server UDP
 *
 * Responsibles: 
 *      Felipe Escórcio de Sousa - RA: 171043
 *      Ricardo Ribeiro Cordeiro - RA: 186633 
 * 
 * Refs:
 *  -   http://beej.us/guide/bgnet/translations/bgnet_ptbr.pdf
 */

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
    if ((sock_fd = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
        printf("Socket creation failed! :(\n");
        exit(1);
    }
    printf("Socket created!\n");

    // configure
    memset(&server_addr, 0, sizeof server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind socket
    if ((bind(socket_fd, (sockaddr*) &server_addr, sizeof(server_addr))) != 0) {
        printf("Couldn't bind socket! :(\n");
        exit(1);
    }
    printf("Socket binded!\n");

    // listens for datagrams
    handle_messages(sock_fd, db_client);

    // it never gets here(!) - it would be nice if it could get :(
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

        // receives datagram
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
            default:
                printf("Unknown operation: %d\n", operation);
                printf("Skipping...\n");
        }
    }
}

void register_profile(int curr_fd, char *msg, mongoc_client_t *client) {
    
    char username[USERNAME_LEN];
    int shift;

    // checks admin
    memset(username, 0, sizeof username);
    memcpy(username, msg, sizeof(username));    
    if (strcmp(username, SERVER_ADMIN_USERNAME) != 0) {
        send_message(curr_fd, "[SERVER] Ops! You must be admin to do this :(\n\0", -1);
        printf("User couldn't add profile, permission denied!\n");
        return;
    }
    printf("Adding new profile.\n");

    // adds new profile
    shift = sizeof(char) * USERNAME_LEN;

    if (db_register_profile(&msg[shift], client) < 0) {
        send_message(curr_fd, "[SERVER] An unexpected error ocurred! Could not save profile :/\n\0", -1);
        printf("Failed saving new profile.\n");
        return;
    }
    
    // sends a feedback on complete
    send_message(curr_fd, "[SERVER] Profile successfully inserted! ;)\n", -1);

    return;
}

void add_new_experiences(int curr_fd, char *msg, mongoc_client_t *db_client) {

    int shift;
    char username[USERNAME_LEN], email[EMAIL_LEN];

    // checks admin
    memset(username, 0, sizeof username);
    memcpy(username, msg, sizeof(username));
    if (strcmp(username, SERVER_ADMIN_USERNAME) != 0) {
        send_message(curr_fd, "[SERVER] Ops! You must be admin to do this :(\n", -1);
        printf("User couldn't add a new experience, permission denied!\n");
        return;
    }
    shift = sizeof(username);
    printf("Editing a profile: %s\n", &msg[shift]);

    // save new experiences
    memset(email, 0, sizeof(email));
    memcpy(email, &msg[shift], sizeof(email));
    shift += sizeof(email);

    if (!db_add_new_experiences(email, &msg[shift], db_client)){
        printf("Could not save new experiences, an unexpected error ocurred!\n");
        send_message(curr_fd, "[SERVER] Error inserting new experiences! sorry :(\n", -1);
        return;
    }

    // sends a feedback
    send_message(curr_fd, "[SERVER] Profile successfully edited!\n", -1);

    return;
}

void list_by_course(int curr_fd, char *msg, mongoc_client_t *db_client) {
    
    char list[BUFFER_LEN];

    memset(list, 0, sizeof list);

    // gets list on db
    db_list_by_course(msg, list, db_client);

    // sends data to client
    send_message(curr_fd, list, sizeof(list));

    return;
}

void list_by_skill(int curr_fd, char *msg, mongoc_client_t *db_client) {
    
    char list[BUFFER_LEN];

    memset(list, 0, sizeof list);

    // gets list on db
    db_list_by_skill(msg, list, db_client);

    // sends data to client
    send_message(curr_fd, list, sizeof(list));

    return;
}

void list_by_graduation_year(int curr_fd, char *msg, mongoc_client_t *db_client) {
    
    char list[BUFFER_LEN];

    memset(list, 0, sizeof list);

    // gets list on db
    db_list_by_graduation_year(msg, list, db_client);

    // sends data to client
    send_message(curr_fd, list, sizeof(list));

    return;
}

void list_all(int curr_fd, mongoc_client_t *db_client) {

    char list[BUFFER_LEN];

    memset(list, 0, sizeof list);

    // gets list on db
    db_list_all(list, db_client);

    // sends data to client
    send_message(curr_fd, list, sizeof(list));

    return;
}

void find_by_email(int curr_fd, char *msg, mongoc_client_t *db_client) {
    
    char buffer[BUFFER_LEN];

    memset(buffer, 0, sizeof buffer);

    // gets profile on db
    db_find_by_email(msg, buffer, db_client);

    // sends data to client
    send_message(curr_fd, buffer, sizeof(buffer));

    return;
}

void delete_profile(int curr_fd, char *msg, mongoc_client_t *db_client) {
    
    int shift;
    char username[USERNAME_LEN];

    memset(username, 0, sizeof(username));
    memcpy(username, msg, sizeof(username));

    // checks admin
    if (strcmp(username, SERVER_ADMIN_USERNAME) != 0) {
        send_message(curr_fd, "[SERVER] Ops! You must be admin to do this :(\n\0", -1);
        printf("User couldn't delete a profile, permission denied!\n");
        return;
    }
    shift = sizeof(username);
    printf("Deleting a profile: %s\n", &msg[shift]);

    // tries to delete a profile
    if (db_delete_profile(&msg[shift], db_client) < 0) {
        send_message(curr_fd, "[SERVER] An unexpected error ocurred! Could not delete profile :/\n\0", -1);
        printf("Failed deleting profile.\n");
        return;
    }
    
    // sends a feedback to client if it could delete profile
    send_message(curr_fd, "[SERVER] Profile successfully deleted! ;)\n", -1);

    return;
}

