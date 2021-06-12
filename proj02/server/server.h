/*
 * Server UDP - Headers
 *
 * Responsibles: 
 *      Felipe Escórcio de Sousa - RA: 171043
 *      Ricardo Ribeiro Cordeiro - RA: 186633
 * 
 */

#ifndef SERVER_H
#define SERVER_H

/* includes */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#include <mongoc/mongoc.h>

#include "../shared.h"

/* constants */
#define SERVER_MAX_CONNECTIONS 5
#define SERVER_ADMIN_USERNAME "admin"

/* types */


/* functions */
void handle_messages(int, mongoc_client_t*);
bool check_admin(char *);
void register_profile(int, char *, mongoc_client_t*);
void add_new_experiences(int, char *, mongoc_client_t*);
void list_by_course(int, char *, mongoc_client_t*);
void list_by_skill(int, char *, mongoc_client_t*);
void list_by_graduation_year(int, char *, mongoc_client_t*);
void list_all(int, mongoc_client_t*);
void find_by_email(int, char *, mongoc_client_t*);
void delete_profile(int, char *, mongoc_client_t*);

#endif