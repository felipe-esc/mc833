/*
 * Blabla
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
    printf("Connected with server!\n");

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
        printf("Do you need anything else? ");
    }
}

void close_connection(int curr_fd) {
    
    int op = CLOSE_CONNECTION;
    char send_buffer[BUFFER_LEN], receive_buffer[BUFFER_LEN];

    memcpy(send_buffer, &op, sizeof(int));
    
    send_message(curr_fd, send_buffer);
    receive_message(curr_fd, receive_buffer);

    printf("%s\n", receive_buffer);

    return;
}

void print_help() {
    
    printf("HELP! I NEED SOMEBODY'S HELP!\n\n");

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
    
    printf("%s\n", help);
}

void register_profile(int curr_fd) {

    char username[20], email[50], name[30], surname[200], residence[50], graduation[50], skills[500], add_xp = 'y';
    char **experiences;
    int graduation_year, xp_size = 0;
    bson_t *document;
    // pedir dados
    printf("Please, identify yourself:\n");
    scanf(" %[^\n]s", username);
    
    printf("\nNow insert the user data you want to save\n");
    printf("Email: \n");
    scanf(" %[^\n]s", email);
    printf("Name: \n");
    scanf(" %[^\n]s", name);
    printf("Surname: \n");
    scanf(" %[^\n]s", surname);
    printf("Residence: \n");
    scanf(" %[^\n]s", residence);
    printf("Graduation: \n");
    scanf(" %[^\n]s", graduation);
    printf("Graduation year: \n");
    scanf("%d", graduation_year);
    printf("Skills: \n");
    scanf(" %[^\n]s", skills);
    // fazer um array de strings
    /*
    printf("Previous experiences: \n");
    experiences = malloc(sizeof (char) * 500);
    while (add_xp == "y") {
        xp_size++;
        experiences = realloc((xp_size) * sizeof (char) * 500);
        scanf(" %[^\n]s", experiences[xp_size]);
        printf("Add another experience? (y/n)\n");
        scanf("%c", add_xp);
    }
    */

    // document = bson_new();
    // BSON_APPEND_UTF8(&document, "name", name);

    // converter dados
    // user = bson_as_canonical_extended_json(document, NULL);

    // enviar mensagem

    // receber feedback

}

void add_new_experiences(int curr_fd) {
    printf("TODO: Implementar - %s\n", __func__);
    // pedir dados

    // converter

    // enviar

    // feedback
}

void list_by_course(int curr_fd) {
    printf("TODO: Implementar - %s\n", __func__);
    // pedir curso

    // enviar

    // esperar resposta ou feedback

}

void list_by_skill(int curr_fd) {
    printf("TODO: Implementar - %s\n", __func__);
    // pedir habilidade

    // enviar

    // esperar resposta ou feedback

}

void list_by_graduation_year(int curr_fd) {
    printf("TODO: Implementar - %s\n", __func__);
    // pedir ano

    // enviar

    // esperar resposta ou feedback

}

void list_all(int curr_fd) {
    printf("TODO: Implementar - %s\n", __func__);
    // enviar request

    // esperar resposta ou feedback

}

void find_by_email(int curr_fd) {
    printf("TODO: Implementar - %s\n", __func__);
    // pedir email

    // enviar

    // esperar resposta ou feedback

}

void delete_profile(int curr_fd) {
    printf("TODO: Implementar - %s\n", __func__);
    // pedir email

    // enviar

    // esperar resposta ou feedback

}
