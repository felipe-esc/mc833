/*
 * Blabla
 * Refs:
 *  -   http://mongoc.org/libmongoc/current/tutorial.html#include-and-link-libmongoc-in-your-c-program
 */

#include <string.h>

#include "db.h"

mongoc_client_t* connect_db(char *config_uri) {
    
    mongoc_client_t *client;
    mongoc_uri_t *uri;
    char uri_string[100];

    mongoc_init();

    if (config_uri) {
        memcpy(uri_string, config_uri, strlen(config_uri));
    } else {
        memcpy(uri_string, MONGO_LOCAL_URI, strlen(MONGO_LOCAL_URI));
    }

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
    
    mongoc_collection_t *collection;
    bson_error_t error;
    bson_oid_t oid;
    bson_t *doc = NULL;

    collection = mongoc_client_get_collection(client, DB_NAME, PROFILE_COLLECTION);
    doc = bson_new_from_json((const uint8_t *)msg, -1, &error);
    
    if (doc == NULL) {
        printf("DB: Error creating document: %s\n", error.message);
        return -1;
    }

    bson_oid_init(&oid, NULL);
    BSON_APPEND_OID(doc, "_id", &oid);
    
    if (!mongoc_collection_insert_one(collection, doc, NULL, NULL, &error)) {
        printf("DB: Error inserting profile: %s\n", error.message);
        return -1;
    }

    bson_destroy(doc);
    mongoc_collection_destroy(collection);
}

int db_add_new_experiences(char *email, char *xp, mongoc_client_t *client) {
    printf("TODO: Implementar - %s\n", __func__);

    // tentar inserir skill

    // retornar se sucesso ou não

    return 0;
}

void db_list_by_course(char *course, char* buffer, mongoc_client_t *client) {
    
    mongoc_collection_t *collection;
    mongoc_cursor_t *cursor;
    bson_error_t error;
    const bson_t *doc;
    bson_t *query;
    char *str;
    int buffer_filled = 0;

    collection = mongoc_client_get_collection(client, DB_NAME, PROFILE_COLLECTION);
    query = bson_new();
    printf("recived course %s\n", course);
    BSON_APPEND_UTF8(query, "graduation",course); // talvez seja BSON_APPEND_UTF8(query, "graduation", course, strlen(course));
    cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);
    while (mongoc_cursor_next (cursor, &doc)) {
        str = bson_as_canonical_extended_json (doc, NULL);
        memcpy(buffer + buffer_filled, str, strlen(str));
        buffer_filled += strlen(str);
        bson_free (str);
    }

    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);
}

void db_list_by_skill(char *skill, char* buffer, mongoc_client_t *client) {

    mongoc_collection_t *collection;
    mongoc_cursor_t *cursor;
    bson_error_t error;
    const bson_t *doc;
    bson_t *query;
    char *str;
    int buffer_filled = 0;


    collection = mongoc_client_get_collection(client, DB_NAME, PROFILE_COLLECTION);
    query = bson_new();
    printf("recived skill %s\n", skill);
    BSON_APPEND_UTF8(query, "skills",skill); // talvez seja BSON_APPEND_UTF8(query, "graduation", course, strlen(course));
    cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);
    while (mongoc_cursor_next (cursor, &doc)) {
        str = bson_as_canonical_extended_json (doc, NULL);
        memcpy(buffer + buffer_filled, str, strlen(str));
        buffer_filled += strlen(str);
        bson_free (str);
    }

    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);
}

void db_list_by_graduation_year(char* year, char* buffer, mongoc_client_t *client) {
    
    mongoc_collection_t *collection;
    mongoc_cursor_t *cursor;
    bson_error_t error;
    const bson_t *doc;
    bson_t *query;
    char *str;
    int buffer_filled = 0;


    collection = mongoc_client_get_collection(client, DB_NAME, PROFILE_COLLECTION);
    query = bson_new();
    printf("recived graduationYear %s\n", year);
    BSON_APPEND_UTF8(query, "graduationYear",year); // talvez seja BSON_APPEND_UTF8(query, "graduation", course, strlen(course));
    cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);
    while (mongoc_cursor_next (cursor, &doc)) {
        str = bson_as_canonical_extended_json (doc, NULL);
        memcpy(buffer + buffer_filled, str, strlen(str));
        buffer_filled += strlen(str);
        bson_free (str);
    }

    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);
}

void db_list_all(char* buffer, mongoc_client_t *client) {
    printf("TODO: teste - %s\n", __func__);

    char *str;
    int buffer_filled = 0;
    mongoc_collection_t *collection;
    bson_t *query;
    mongoc_cursor_t *cursor;
    const bson_t *doc;

    collection = mongoc_client_get_collection(client, DB_NAME, PROFILE_COLLECTION);
    query = bson_new();
    cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);

    doc = bson_new();
    while (mongoc_cursor_next(cursor, &doc)) {
        str = bson_as_canonical_extended_json (doc, NULL);
        memcpy(buffer + buffer_filled, str, strlen(str));
        buffer_filled += strlen(str);
        bson_free(str);
    }

    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);
}

void db_find_by_email(char *email, char *buffer, mongoc_client_t *client) {
    
    mongoc_collection_t *collection;
    mongoc_cursor_t *cursor;
    bson_error_t error;
    const bson_t *doc;
    bson_t *query;
    char *str;
    int buffer_filled = 0;

    collection = mongoc_client_get_collection(client, DB_NAME, PROFILE_COLLECTION);
    query = bson_new();
    printf("recived email %s\n", email);
    BSON_APPEND_UTF8(query, "email",email);
    cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);
    while (mongoc_cursor_next (cursor, &doc)) {
        str = bson_as_canonical_extended_json (doc, NULL);
        memcpy(buffer + buffer_filled, str, strlen(str));
        buffer_filled += strlen(str);
        bson_free (str);
    }

    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);
}

int db_delete_profile(char *email, mongoc_client_t *client) {
    
    mongoc_collection_t *collection;
    bson_error_t error;
    bson_oid_t oid;
    bson_t *doc;

    collection = mongoc_client_get_collection(client, DB_NAME, PROFILE_COLLECTION);

    doc = bson_new();
    bson_oid_init(&oid, NULL);
    BSON_APPEND_OID(doc, "_id", &oid);
    printf("email to be deleted %s", email);
    BSON_APPEND_UTF8(doc, "email", email);

    if (!mongoc_collection_delete_one(collection, doc, NULL, NULL, &error)) {
        printf("Delete failed: %s\n", error.message);
    }

    bson_destroy(doc);
    mongoc_collection_destroy(collection);
}
