/*
 * Blabla
 */

#ifndef DB_H
#define DB_H

/* includes */
#include <mongoc/mongoc.h>

/* constants */ 
#define MONGO_LOCAL_URI "mongodb://localhost:27017"
#define USER_COLLECTION 'alumni'

/* types */

/* functions */
mongoc_client_t* connect_db();
mongoc_client_t* disconnect_db(mongoc_client_t*);

// >> db.database.find({A: /abc def/i });
// >> db.users.findOne({"username" : {$regex : ".*son.*"}});

int db_register_profile(char *, mongoc_client_t*); 
char* db_add_new_experiences(char*, char *, mongoc_client_t*); //bson_t?
char* db_list_by_course(char *, mongoc_client_t*);
char* db_list_by_skill(char *, mongoc_client_t*);
char* db_list_by_graduation_year(int, mongoc_client_t*);
char* db_list_all(mongoc_client_t*);
char* db_find_by_email(char *, mongoc_client_t*);
int db_delete_profile(char *, mongoc_client_t*);

#endif
