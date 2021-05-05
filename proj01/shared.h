/*
 * Blabla
 */

#ifndef CLIENT_SERVER_SHARED
#define CLIENT_SERVER_SHARED

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define TIMEOUT 15000
#define BUFFER_LEN 4096
#define USERNAME_LEN 20

#define SERVER_DEFAULT_PORT 3490
#define LOCALHOST "127.0.0.1"

enum operations {
    REGISTER_PROFILE,           // 0   
    ADD_EXPERIENCES,            // 1
    LIST_BY_COURSE,             // 2
    LIST_BY_SKILL,              // 3
    LIST_BY_GRADUATION_YEAR,    // 4
    LIST_ALL,                   // 5
    FIND_BY_EMAIL,              // 6
    DELETE_PROFILE,             // 7
    CLOSE_CONNECTION            // 8
};

void send_message(int, char *, int);
void receive_message(int, char *);

#endif
