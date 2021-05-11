/*
 * Client TCP - Headers
 *
 * Responsibles: 
 *      Felipe Escórcio de Sousa - RA: 171043
 *      Ricardo Ribeiro Cordeiro - RA: 186633 
 * 
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
#define COURSE_LEN 50
#define NAME_LEN 30
#define SURNAME_LEN 200
#define RESIDENCE_LEN 50
#define GRADUATION_LEN 50
#define SKILL_LEN 200
#define YEAR_LEN 5
#define EXPERIENCE_LEN 200

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
