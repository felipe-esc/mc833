/*
 * Blabla
 * Refs:
 *  -   http://beej.us/guide/bgnet/translations/bgnet_ptbr.pdf
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"

int main(int argc, char *argv[]) {

    int server_port = SERVER_DEFAULT_PORT, sock_fd;
    char *server_ip = LOCALHOST;
    struct sockaddr_in server_addr;

    // receives ip
    if (argc > 1) {
        server_ip = argv[1];
    }
    // receives port
    if (argc == 3) {
       server_port = atoi(argv[2]);
    }

    // create socket
    if ((sock_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        printf("Socket creation failed!\n");
        exit(1);
    }
    printf("Socket created.\n");

    // config server address
    memset(&server_addr, 0, sizeof server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    
    // convert IPv4 or IPv6 ip
    if(inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        printf("Failed to convert server IP\n");
        exit(1);
    }

    // connect
    if (connect(sock_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        printf("Failed to connect with server!\n");
        exit(1);
    }
    printf("Connected with server!\n\n");

    operate(sock_fd);

    printf("Finishing...\n");
    close(sock_fd);
    return 0;
}

void operate(int curr_fd) {
    int option;

    while (1) {
        printf("Choose an option[0-9]: (Choose 9 for help)\n");
        scanf("%d", &option);

        switch (option) {
            case REGISTER_PROFILE:
                register_profile(curr_fd);
                break;
            case ADD_EXPERIENCES:
                add_new_experiences(curr_fd);
                break;
            case LIST_BY_COURSE:
                list_by_course(curr_fd);
                break;
            case LIST_BY_SKILL:
                list_by_skill(curr_fd);
                break;
            case LIST_BY_GRADUATION_YEAR:
                list_by_graduation_year(curr_fd);
                break;
            case LIST_ALL:
                list_all(curr_fd);
                break;
            case FIND_BY_EMAIL:
                find_by_email(curr_fd);
                break;
            case DELETE_PROFILE:
                delete_profile(curr_fd);
                break;
            case CLOSE_CONNECTION:
                close_connection(curr_fd);
                return;
            default:
                print_help();
        }
        printf("\nDo you need anything else? ");
    }
}

void close_connection(int curr_fd) {
    
    int op = CLOSE_CONNECTION;
    char send_buffer[BUFFER_LEN], receive_buffer[BUFFER_LEN];

    memcpy(send_buffer, &op, sizeof(int));

    send_message(curr_fd, send_buffer, sizeof send_buffer);

    receive_message(curr_fd, receive_buffer);
    printf("%s\n", receive_buffer);

    return;
}

void print_help() {
    
    printf("\nHELP! I NEED SOMEBODY'S HELP!\n\n");

    const char *help = "Available options:\n"
                        "\t\t0 - Register new profile\n"
                        "\t\t1 - Add new experience\n"
                        "\t\t2 - List by course\n"
                        "\t\t3 - List by skill\n"
                        "\t\t4 - List by graduation year\n"
                        "\t\t5 - List all\n"
                        "\t\t6 - Find by Email\n"
                        "\t\t7 - Delete profile\n"
                        "\t\t8 - Close connection\n"
                        "\t\t9 - Help\n\n";
    
    printf("%s", help);
}

void register_profile(int curr_fd) {

    char username[USERNAME_LEN], email[EMAIL_LEN], name[NAME_LEN], surname[SURNAME_LEN], 
        residence[RESIDENCE_LEN], graduation[GRADUATION_LEN], skills[SKILL_LEN], graduation_year[YEAR_LEN], 
        **experiences, *profile, add_xp = 'y', buf[200], msg[BUFFER_LEN], feedback[BUFFER_LEN];
    const char *key;
    int xp_size = 0, op = REGISTER_PROFILE, shift;
    bson_t *document, child;
    size_t keylen;

    // ask for data
    printf("\nPlease, identify yourself:\n");
    scanf(" %[^\n]s", username);
    
    printf("\nNow insert the user data you want to save.\n");
    printf("Email:\n");
    scanf(" %[^\n]s", email);
    printf("Name:\n");
    scanf(" %[^\n]s", name);
    printf("Surname:\n");
    scanf(" %[^\n]s", surname);
    printf("Residence:\n");
    scanf(" %[^\n]s", residence);
    printf("Graduation:\n");
    scanf(" %[^\n]s", graduation);
    printf("Graduation year:\n");
    scanf(" %[^\n]s", graduation_year);
    printf("Skills:\n");
    scanf(" %[^\n]s", skills);
    
    printf("Previous experiences: \n");
    experiences = malloc(sizeof(char*));
    while (add_xp == 'y') {
        experiences[xp_size] = malloc(sizeof(char) * EXPERIENCE_LEN);
        scanf(" %[^\n]s", experiences[xp_size]);
        xp_size++;
        printf("Add another experience? (y/n)\n");
        scanf(" %c", &add_xp);
        if (add_xp == 'y') {
            printf("Please, insert your additional experiece\n");
            experiences = realloc(experiences, (xp_size + 1) * sizeof(char*));
        }
    }

    // create bson
    document = bson_new();
    bson_append_utf8(document, "email", strlen("email\0"), email, -1);
    bson_append_utf8(document, "name", strlen("name\0"), name, -1);
    bson_append_utf8(document, "surname", strlen("surname\0"), surname, -1);
    bson_append_utf8(document, "residence", strlen("residence\0"), residence, -1);
    bson_append_utf8(document, "graduation", strlen("graduation\0"), graduation, -1);
    bson_append_utf8(document, "graduationYear", strlen("graduationYear\0"), graduation_year, -1);
    bson_append_utf8(document, "skills", strlen("skills\0"), skills, -1);

    BSON_APPEND_ARRAY_BEGIN(document, "experiences", &child);
    for (int i = 0; i < xp_size; ++i) {
        keylen = bson_uint32_to_string(i, &key, buf, sizeof buf);
        bson_append_utf8(&child, key, (int) keylen, experiences[i], -1);
    }
    bson_append_array_end(document, &child);

    // convert data
    profile = bson_as_canonical_extended_json(document, NULL);
    
    memset(msg, 0, sizeof msg);

    memcpy(msg, &op, sizeof(int));
    shift = sizeof(int);
    memcpy(&msg[shift], username, sizeof(username));
    shift += sizeof(username);
    strcpy(&msg[shift], profile);

    // send message
    send_message(curr_fd, msg, sizeof msg);

    // receive feedback
    receive_message(curr_fd, feedback);
    printf("%s\n",feedback);


    bson_free(profile);
    bson_destroy(document);

    for (int i = 0; i < xp_size; i++) {
        free(experiences[i]);
    }
    free(experiences);
}

void add_new_experiences(int curr_fd) {
    int op = ADD_EXPERIENCES;
    char experience[EXPERIENCE_LEN], feedback[BUFFER_LEN], username[USERNAME_LEN], 
        send_buffer[BUFFER_LEN], receive_buffer[BUFFER_LEN];
    memcpy(send_buffer, &op, sizeof(int));
  
    // op tem a operation, mas falta pegar os dados recebido do scanf
    // pedir dados
    printf("Insert the username that you want to add a new experience\n");

    printf("Now, insert the experience that you want to be added\n");

    // converter

    // enviar

    // feedback
}

void list_by_course(int curr_fd) {

    int op = LIST_BY_COURSE, shift;
    char graduation[GRADUATION_LEN], send_buffer[BUFFER_LEN], feedback[BUFFER_LEN];
    
    memcpy(send_buffer, &op, sizeof(int));
    shift = sizeof(int);

    printf("Insert the graduation course that you want consult\n");
    scanf(" %[^\n]s", graduation);
    memcpy(&send_buffer[shift], graduation, sizeof(graduation));      
    
    send_message(curr_fd, send_buffer, sizeof(send_buffer));

    receive_message(curr_fd, feedback);
    printf("Here is the list of users with the chosen course\n");
    printf("%s\n",feedback);

}

void list_by_skill(int curr_fd) {

    int op = LIST_BY_SKILL, shift;
    char skill[SKILL_LEN], send_buffer[BUFFER_LEN], feedback[BUFFER_LEN];

    memcpy(send_buffer, &op, sizeof(int));
    shift = sizeof(int);

    printf("Insert the skill that you want to consult\n");
    scanf(" %[^\n]s", skill);
    memcpy(&send_buffer[shift], skill, sizeof(skill));
    
    send_message(curr_fd, send_buffer, sizeof(send_buffer));

    receive_message(curr_fd, feedback);
    printf("Here is the list of users with the chosen skill\n");
    printf("%s\n",feedback);


}

void list_by_graduation_year(int curr_fd) {
    
    int op = LIST_BY_GRADUATION_YEAR, shift;
    char year[YEAR_LEN], send_buffer[BUFFER_LEN], feedback[BUFFER_LEN];
    
    memcpy(send_buffer, &op, sizeof(int));
    shift = sizeof(int);
    
    printf("Insert the graduation year that you want to consult\n");
    scanf(" %[^\n]s", year);
    memcpy(&send_buffer[shift], year, sizeof(year));
    
    send_message(curr_fd, send_buffer, sizeof(send_buffer));

    receive_message(curr_fd, feedback);
    printf("Here is the list of users with the chosen graduation year\n");
    printf("%s\n",feedback);

}

void list_all(int curr_fd) {
    
    int op = LIST_ALL;
    char send_buffer[BUFFER_LEN], receive_buffer[BUFFER_LEN];

    memcpy(send_buffer, &op, sizeof(int));
    
    send_message(curr_fd, send_buffer, sizeof send_buffer);

    receive_message(curr_fd, receive_buffer);
    printf("Here is the list of all users\n");
    printf("%s\n", receive_buffer);
}

void find_by_email(int curr_fd) {
    
    int op = FIND_BY_EMAIL, shift;
    char send_buffer[BUFFER_LEN], feedback[BUFFER_LEN];
    char email[EMAIL_LEN];

    memcpy(send_buffer, &op, sizeof(int));
    shift = sizeof(int);

    printf("Insert the email that you want to consult:\n");
    scanf(" %[^\n]s", &email);
    printf("EMAIL:%s\n", email);
    memcpy(&send_buffer[shift], email, sizeof(send_buffer));
    printf("sendbuffer:%s\n", send_buffer);
    send_message(curr_fd, send_buffer, sizeof(send_buffer));

    receive_message(curr_fd, feedback);
    printf("Here is the list of users with the chosen email\n");
    
    printf("%s\n",feedback);
}

void delete_profile(int curr_fd) {
    int op = DELETE_PROFILE, shift;
    char send_buffer[BUFFER_LEN], feedback[BUFFER_LEN], email[EMAIL_LEN];

    memcpy(send_buffer, &op, sizeof(int));
    shift = sizeof(int);

    // char feedback[BUFFER_LEN];
    printf("Insert the email that you want to delete\n");

    scanf("%[^\n]s", &email);
    memcpy(&send_buffer[shift], email, sizeof(email));

    // enviar
    send_message(curr_fd, send_buffer, sizeof(send_buffer));

    // receive feedback
    receive_message(curr_fd, feedback);
    printf(feedback);
}
