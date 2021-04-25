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
#include <bson/bson.h>

#include "../shared.h"

/* constants */

/* types */

/* function defs */
void operate(int);
void print_help();
void register_profile(int);
void add_new_experiences(int);
void list_by_course(int);
void list_by_skill(int);
void list_by_graduation_year(int);
void list_all(int);
void find_by_email(int);
void delete_profile(int);
void close_connection(int);

#endif
