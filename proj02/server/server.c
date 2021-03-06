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
    struct sockaddr_in server_addr;
    struct sockaddr client_addr;
    socklen_t addr_size;
    char *mongo_uri = NULL;
    mongoc_client_t *db_client;
    struct timeval tv;

    char data[BUFFER_LEN];

    if (argc >= 2) {
        server_port = atoi(argv[1]);
    }
    if (argc == 3) {
        mongo_uri = malloc(sizeof(char) * 300);
        memcpy(mongo_uri, argv[2], strlen(argv[2]));
    }

    db_client = connect_db(mongo_uri);

    // create socket
    if ((sock_fd = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
        printf("Socket creation failed! :(\n");
        exit(1);
    }
    printf("Socket created!\n");

    // configures socket options
    tv.tv_sec = TIMEOUT_SEC;
    tv.tv_usec = TIMEOUT_USEC;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("Error setting socket opts");
    }

    // configure
    memset(&server_addr, 0, sizeof server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind socket
    if ((bind(sock_fd, (struct sockaddr*) &server_addr, sizeof(server_addr))) != 0) {
        printf("Couldn't bind socket! :(\n");
        exit(1);
    }
    printf("Socket binded!\n");

    // listens for datagrams
    handle_messages(sock_fd, db_client, &client_addr);

    // it never gets here(!) - it would be nice if it could get :(
    disconnect_db(db_client);
    close(sock_fd);

    return 0;
}

/*
 * Waits and handle received datagrams
 */
void handle_messages(int curr_fd, mongoc_client_t *db_client, struct sockaddr *client_addr) {
    
    char buffer[BUFFER_LEN];
    int shift = 0, buffer_filled, len_read;
    enum operations operation;

    while (1) {
        memset(buffer, 0, sizeof buffer);

        // waits for datagram
        while (!receive_message(curr_fd, buffer, client_addr)) { 
            continue;
        }

        // get operation
        shift = sizeof operation;
        memcpy(&operation, buffer, sizeof operation);

        // handles it
        switch (operation) {
            case REGISTER_PROFILE:
                register_profile(curr_fd, &buffer[shift], db_client, client_addr);
                break;
            case ADD_EXPERIENCES:
                add_new_experiences(curr_fd, &buffer[shift], db_client, client_addr);
                break;
            case LIST_BY_COURSE:
                list_by_course(curr_fd, &buffer[shift], db_client, client_addr);
                break;
            case LIST_BY_SKILL:
                list_by_skill(curr_fd, &buffer[shift], db_client, client_addr);
                break;
            case LIST_BY_GRADUATION_YEAR:
                list_by_graduation_year(curr_fd, &buffer[shift], db_client, client_addr);
                break;
            case LIST_ALL:
                list_all(curr_fd, db_client, client_addr);
                break;
            case FIND_BY_EMAIL:
                find_by_email(curr_fd, &buffer[shift], db_client, client_addr);
                break;
            case DELETE_PROFILE:
                delete_profile(curr_fd, &buffer[shift], db_client, client_addr);
                break;
            default:
                printf("Unknown operation: %d\n", operation);
                printf("Skipping...\n");
        }
    }
}

/*
 * Function that registers profiles on the database
 */
void register_profile(int curr_fd, char *msg, mongoc_client_t *client, struct sockaddr *client_addr) {
    
    char username[USERNAME_LEN];
    int shift;

    // checks admin
    memset(username, 0, sizeof username);
    memcpy(username, msg, sizeof(username));    
    if (strcmp(username, SERVER_ADMIN_USERNAME) != 0) {
        send_message(curr_fd, "[SERVER] Ops! You must be admin to do this :(\n\0", -1, client_addr);
        printf("User couldn't add profile, permission denied!\n");
        return;
    }
    printf("Adding new profile.\n");

    // adds new profile
    shift = sizeof(char) * USERNAME_LEN;

    if (db_register_profile(&msg[shift], client) < 0) {
        send_message(curr_fd, "[SERVER] An unexpected error ocurred! Could not save profile :/\n\0", -1, client_addr);
        printf("Failed saving new profile.\n");
        return;
    }
    
    // sends a feedback on complete
    send_message(curr_fd, "[SERVER] Profile successfully inserted! ;)\n", -1, client_addr);

    return;
}

/*
 * Function that adds new experiences to a profile
 */
void add_new_experiences(int curr_fd, char *msg, mongoc_client_t *db_client, struct sockaddr *client_addr) {

    int shift;
    char username[USERNAME_LEN], email[EMAIL_LEN];

    // checks admin
    memset(username, 0, sizeof username);
    memcpy(username, msg, sizeof(username));
    if (strcmp(username, SERVER_ADMIN_USERNAME) != 0) {
        send_message(curr_fd, "[SERVER] Ops! You must be admin to do this :(\n", -1, client_addr);
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
        send_message(curr_fd, "[SERVER] Error inserting new experiences! sorry :(\n", -1, client_addr);
        return;
    }

    // sends a feedback
    send_message(curr_fd, "[SERVER] Profile successfully edited!\n", -1, client_addr);

    return;
}

/*
 * Function that lists courses and sends to client
 */
void list_by_course(int curr_fd, char *msg, mongoc_client_t *db_client, struct sockaddr *client_addr) {
    
    char list[BUFFER_LEN];

    printf("Listing by course: %s\n", msg);

    memset(list, 0, sizeof list);

    // gets list on db
    db_list_by_course(msg, list, db_client);

    // sends data to client
    send_message(curr_fd, list, sizeof(list), client_addr);

    return;
}

/*
 * Function that lists profiles by skill and sends to client
 */
void list_by_skill(int curr_fd, char *msg, mongoc_client_t *db_client, struct sockaddr *client_addr) {
    
    char list[BUFFER_LEN];

    printf("Listing by skill: %s\n", msg);

    memset(list, 0, sizeof list);

    // gets list on db
    db_list_by_skill(msg, list, db_client);

    // sends data to client
    send_message(curr_fd, list, sizeof(list), client_addr);

    return;
}

/*
 * Function that lists profiles by graduation year and sends to client
 */
void list_by_graduation_year(int curr_fd, char *msg, mongoc_client_t *db_client, struct sockaddr *client_addr) {
    
    char list[BUFFER_LEN];

    printf("Listing by graduation year: %s\n", msg);

    memset(list, 0, sizeof list);

    // gets list on db
    db_list_by_graduation_year(msg, list, db_client);

    // sends data to client
    send_message(curr_fd, list, sizeof(list), client_addr);

    return;
}

/*
 * Function that lists all profiles and sends to client
 */
void list_all(int curr_fd, mongoc_client_t *db_client, struct sockaddr *client_addr) {

    char list[BUFFER_LEN];

    printf("Listing all\n");

    memset(list, 0, sizeof list);

    // gets list on db
    db_list_all(list, db_client);

    // sends data to client
    send_message(curr_fd, list, sizeof(list), client_addr);

    return;
}

/*
 * Function that searches a profile by email
 */
void find_by_email(int curr_fd, char *msg, mongoc_client_t *db_client, struct sockaddr *client_addr) {
    
    char buffer[BUFFER_LEN];

    printf("Finding by email: %s\n", msg);

    memset(buffer, 0, sizeof buffer);

    // gets profile on db
    db_find_by_email(msg, buffer, db_client);

    // sends data to client
    send_message(curr_fd, buffer, sizeof(buffer), client_addr);

    return;
}

/*
 * Functions that tries to delete profile
 */
void delete_profile(int curr_fd, char *msg, mongoc_client_t *db_client, struct sockaddr *client_addr) {
    
    int shift;
    char username[USERNAME_LEN];

    memset(username, 0, sizeof(username));
    memcpy(username, msg, sizeof(username));

    // checks admin
    if (strcmp(username, SERVER_ADMIN_USERNAME) != 0) {
        send_message(curr_fd, "[SERVER] Ops! You must be admin to do this :(\n\0", -1, client_addr);
        printf("User couldn't delete a profile, permission denied!\n");
        return;
    }
    shift = sizeof(username);
    printf("Deleting a profile: %s\n", &msg[shift]);

    // tries to delete a profile
    if (db_delete_profile(&msg[shift], db_client) < 0) {
        send_message(curr_fd, "[SERVER] An unexpected error ocurred! Could not delete profile :/\n\0", -1, client_addr);
        printf("Failed deleting profile.\n");
        return;
    }
    
    // sends a feedback to client if it could delete profile
    send_message(curr_fd, "[SERVER] Profile successfully deleted! ;)\n", -1, client_addr);

    return;
}

/*
 *  Handles incoming datagrams (Server side)
 */
bool receive_message(int fd, char *msg, struct sockaddr *addr) {
    int len_read, buffer_filled = 0, fromlen;

    fromlen = sizeof(addr);
    
    while (buffer_filled < BUFFER_LEN) {
        if ((len_read = recvfrom(fd, &msg[buffer_filled], (BUFFER_LEN - buffer_filled), 0, addr, &fromlen)) > 0) {
            buffer_filled += len_read;
        } else {
            return false;
        }
    }

    return true;
}

