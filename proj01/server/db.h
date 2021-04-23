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

// int register_profile(char *);
// int add_new_experiences(char *);
// char* list_by_course(char *);
// char* list_by_skill(char *);
// char* list_by_graduation_year(char *);
// char* list_all();
// char* find_by_email(char *);
// int delete_user(char *);

#endif
