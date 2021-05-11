/*
 * DataBase Server TCP - Headers
 *
 * Responsibles: 
 *      Felipe Escórcio de Sousa - RA: 171043
 *      Ricardo Ribeiro Cordeiro - RA: 186633 
 * 
 */

#ifndef DB_H
#define DB_H

/* includes */
#include <mongoc/mongoc.h>

/* constants */ 
#define MONGO_LOCAL_URI "mongodb://localhost:27017"
#define DB_NAME "alumni"
#define PROFILE_COLLECTION "profiles"

/* types */

/* functions */
mongoc_client_t* connect_db(char*);
mongoc_client_t* disconnect_db(mongoc_client_t*);

int db_register_profile(char*, mongoc_client_t*); 
int db_add_new_experiences(char*, char *, mongoc_client_t*); //bson_t?
void db_list_by_course(char*, char*, mongoc_client_t*);
void db_list_by_skill(char*, char*, mongoc_client_t*);
void db_list_by_graduation_year(char*, char*, mongoc_client_t*);
void db_list_all(char*,mongoc_client_t*);
void db_find_by_email(char*, char*, mongoc_client_t*);
int db_delete_profile(char*, mongoc_client_t*);

#endif
