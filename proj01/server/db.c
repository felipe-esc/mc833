/*
 * Blabla
 */

#include <string.h>

#include "db.h"

mongoc_client_t* connect_db() {
    
    mongoc_client_t *client;
    mongoc_uri_t *uri;
    char *uri_string = MONGO_LOCAL_URI;
    //char *config_uri;

    mongoc_init();

    /*if (!config_uri) {
        memcpy(uri_string, config_uri, strlen(config_uri));
    }*/

    // parses mongo uri
    if (!(uri = mongoc_uri_new(uri_string))) {
        printf("Failed to parse MongoDB URI.. exiting\n", uri_string);
        exit(1);
    }

    // gets client
    if (!(client = mongoc_client_new_from_uri(uri))) {
        printf("Failed to get MongoDB.. exiting\n");
        exit(1);
    }
    printf("DB connected\n");

    // we don't need the uri anymore
    mongoc_uri_destroy(uri);

    return client;    
}

mongoc_client_t* disconnect_db(mongoc_client_t* client) {
    
    mongoc_client_destroy(client);
    mongoc_cleanup();

    return NULL;
}

int db_register_profile(char *msg, mongoc_client_t *client) {

    // salvar

    // retornar se sucesso ou não

    return 0;
}

char* db_add_new_experiences(char *msg, mongoc_client_t *client) {

    return NULL;
}

char* db_list_by_course(char *msg, mongoc_client_t *client) {

    return NULL;
}

char* db_list_by_skill(char *msg, mongoc_client_t *client) {

    return NULL;
}

char* db_list_by_graduation_year(char *msg, mongoc_client_t *client) {

    return NULL;
}

char* db_list_all(mongoc_client_t *client) {

    return NULL;
}

char* db_find_by_email(char *msg, mongoc_client_t *client) {

    return NULL;
}

int db_delete_user(char *msg, mongoc_client_t *client) {

    return 0;
}
