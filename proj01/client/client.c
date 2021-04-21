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
        printf("Choose an option[0-9]: (9 for help)\n");
        scanf("%d", &option);

        switch (option) {
            case REGISTER_PROFILE:
                // register_profile();
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

    printf("%s", receive_buffer);

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
