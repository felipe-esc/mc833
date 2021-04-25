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
    printf("TODO: Implementar - %s\n", __func__);

    // salvar

    // retornar se sucesso ou não

    return 0;
}

char* db_add_new_experiences(char *email, char *xp, mongoc_client_t *client) {
    printf("TODO: Implementar - %s\n", __func__);

    // tentar inserir skill

    // retornar se sucesso ou não

    return NULL;
}

char* db_list_by_course(char *course, mongoc_client_t *client) {
    printf("TODO: Implementar - %s\n", __func__);

    // busca por curso

    return NULL;
}

char* db_list_by_skill(char *skill, mongoc_client_t *client) {
    printf("TODO: Implementar - %s\n", __func__);

    // busca por skill

    return NULL;
}

char* db_list_by_graduation_year(int year, mongoc_client_t *client) {
    printf("TODO: Implementar - %s\n", __func__);

    // busca por ano de graduação

    return NULL;
}

char* db_list_all(mongoc_client_t *client) {
    printf("TODO: Implementar - %s\n", __func__);

    // lista todos

    return NULL;
}

char* db_find_by_email(char *email, mongoc_client_t *client) {
    printf("TODO: Implementar - %s\n", __func__);

    // busca por email

    return NULL;
}

int db_delete_profile(char *email, mongoc_client_t *client) {
    printf("TODO: Implementar - %s\n", __func__);

    // tenta deletar

    // retorno sucesso ou não

    return 0;
}
