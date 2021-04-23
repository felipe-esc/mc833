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
