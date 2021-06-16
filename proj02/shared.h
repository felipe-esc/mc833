/*
 * Shared Functions UDP - Headers
 *
 * Responsibles: 
 *      Felipe Escórcio de Sousa - RA: 171043
 *      Ricardo Ribeiro Cordeiro - RA: 186633 
 * 
 */

#ifndef CLIENT_SERVER_SHARED
#define CLIENT_SERVER_SHARED

/* includes */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

/* constants */
#define TIMEOUT_SEC 2
#define TIMEOUT_USEC 500000

#define BUFFER_LEN 12000
#define USERNAME_LEN 20
#define EMAIL_LEN 100

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
    EXIT                        // 8
};

/* types */

/* functions defs */
void send_message(int, char*, int, struct sockaddr*);

#endif
