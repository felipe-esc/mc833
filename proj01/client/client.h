/*
 * blabla
 */

#ifndef CLIENT_H
#define CLIENT_H

/* includes */
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../shared.h"

/* constants */

/* types */

/* function defs */
void operate(int);
void print_help();
// int register_profile(char *);
// int add_new_experiences(char *);
// void list_by_course(char *);
// void list_by_skill(char *);
// void list_by_graduation_year(char *);
// void list_all();
// void find_by_email(char *);
// int delete_user(char *);
void close_connection(int);

#endif
