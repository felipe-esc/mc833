/*
 * DataBase Server UDP
 *
 * Responsibles: 
 *      Felipe Escórcio de Sousa - RA: 171043
 *      Ricardo Ribeiro Cordeiro - RA: 186633 
 * 
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

    // configs db uri
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
    
    // tries to disconnect
    mongoc_client_destroy(client);
    mongoc_cleanup();

    return NULL;
}

int db_register_profile(char *msg, mongoc_client_t *client) {
    
    mongoc_collection_t *collection;
    bson_error_t error;
    bson_oid_t oid;
    bson_t *doc = NULL;
    int success = 0;

    // gets collection
    collection = mongoc_client_get_collection(client, DB_NAME, PROFILE_COLLECTION);
    
    // creates doc
    doc = bson_new_from_json((const uint8_t *)msg, -1, &error);
    if (doc == NULL) {
        printf("DB: Error creating document: %s\n", error.message);
        return -1;
    }

    // creates an objectId
    bson_oid_init(&oid, NULL);
    BSON_APPEND_OID(doc, "_id", &oid);
    
    // tries to insert one
    if (!mongoc_collection_insert_one(collection, doc, NULL, NULL, &error)) {
        printf("DB: Error inserting profile: %s\n", error.message);
        success = -1;
    }

    // frees doc
    bson_destroy(doc);
    mongoc_collection_destroy(collection);
    
    return success;
}

int db_add_new_experiences(char *email, char *xp, mongoc_client_t *client) {

    mongoc_collection_t *collection;
    bson_error_t error;
    bson_oid_t oid;
    bson_t *doc = NULL;
    bson_t *update = NULL;
    bson_t *query = NULL;
    int success = 0;

    // gets collection
    collection = mongoc_client_get_collection(client, DB_NAME, PROFILE_COLLECTION);

    // creates query: db.collection.updateOne({ "email": email }, { $push: { "experiences": { $each: [ exp1, exp2, ..., expN ] }}})
    query = bson_new();
    BSON_APPEND_UTF8(query, "email", email);
    update = BCON_NEW("$push", "{", "experiences", "{", "$each", xp, "}", "}");

    // tries to update
    if (!mongoc_collection_update_one(collection, query, update, NULL, NULL, &error)) {
        printf("DB: Error editing profile: %s\n", error.message);
        success = -1;
    }

    // frees docs
    bson_destroy(query);
    bson_destroy(update);
    mongoc_collection_destroy(collection);

    return success;
}

void db_list_by_course(char *course, char* buffer, mongoc_client_t *client) {
    
    mongoc_collection_t *collection;
    mongoc_cursor_t *cursor;
    bson_error_t error;
    const bson_t *doc;
    bson_t *query;
    char *str;
    int buffer_filled = 0;

    // gets collection
    collection = mongoc_client_get_collection(client, DB_NAME, PROFILE_COLLECTION);

    // creates query: db.collection.find({ "graduation": graduation })
    query = bson_new();
    BSON_APPEND_UTF8(query, "graduation", course);
    cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);

    // concats returned documents to a string
    while (mongoc_cursor_next(cursor, &doc)) {
        str = bson_as_canonical_extended_json(doc, NULL);
        memcpy(buffer + buffer_filled, str, strlen(str));
        buffer_filled += strlen(str);
        bson_free (str);
    }

    // frees docs
    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);

    return;
}

void db_list_by_skill(char *skill, char* buffer, mongoc_client_t *client) {

    mongoc_collection_t *collection;
    mongoc_cursor_t *cursor;
    bson_error_t error;
    const bson_t *doc;
    bson_t *query;
    char *str, aux[200] = "/";
    int buffer_filled = 0;

    // gets collection
    collection = mongoc_client_get_collection(client, DB_NAME, PROFILE_COLLECTION);

    // creates query: db.collection.find({ "skill": "/skill/i" })    
    strcat(aux, skill);
    strcat(aux, "/i");

    query = bson_new();
    BSON_APPEND_UTF8(query, "skills", aux);
    cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);

    // concats returned documents to a string
    while (mongoc_cursor_next(cursor, &doc)) {
        str = bson_as_canonical_extended_json(doc, NULL);
        memcpy(buffer + buffer_filled, str, strlen(str));
        buffer_filled += strlen(str);
        bson_free(str);
    }

    // frees docs
    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);

    return;
}

void db_list_by_graduation_year(char* year, char* buffer, mongoc_client_t *client) {
    
    mongoc_collection_t *collection;
    mongoc_cursor_t *cursor;
    bson_error_t error;
    const bson_t *doc;
    bson_t *query;
    char *str;
    int buffer_filled = 0;

    // gets collections
    collection = mongoc_client_get_collection(client, DB_NAME, PROFILE_COLLECTION);

    // creates query: db.collection.find({ "graduationYear": graduation_year })
    query = bson_new();
    BSON_APPEND_UTF8(query, "graduationYear", year);
    cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);

    // concats returned documents to a string
    while (mongoc_cursor_next(cursor, &doc)) {
        str = bson_as_canonical_extended_json(doc, NULL);
        memcpy(buffer + buffer_filled, str, strlen(str));
        buffer_filled += strlen(str);
        bson_free(str);
    }

    // frees docs
    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);

    return;
}

void db_list_all(char* buffer, mongoc_client_t *client) {

    char *str;
    int buffer_filled = 0;
    mongoc_collection_t *collection;
    bson_t *query;
    mongoc_cursor_t *cursor;
    const bson_t *doc;

    // gets colletion
    collection = mongoc_client_get_collection(client, DB_NAME, PROFILE_COLLECTION);

    // creates query: db.collection.find({})
    query = bson_new();
    cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);
    doc = bson_new();

    // concats returned docs to a string
    while (mongoc_cursor_next(cursor, &doc)) {
        str = bson_as_canonical_extended_json(doc, NULL);
        memcpy(buffer + buffer_filled, str, strlen(str));
        buffer_filled += strlen(str);
        bson_free(str);
    }

    // frees docs
    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);

    return;
}

void db_find_by_email(char *email, char *buffer, mongoc_client_t *client) {
    
    mongoc_collection_t *collection;
    mongoc_cursor_t *cursor;
    bson_error_t error;
    const bson_t *doc;
    bson_t *query;
    char *str;
    int buffer_filled = 0;

    // gets collection
    collection = mongoc_client_get_collection(client, DB_NAME, PROFILE_COLLECTION);

    // creates query: db.collection.find({ "email": email })
    query = bson_new();
    BSON_APPEND_UTF8(query, "email", email);
    cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);

    // concats returned docs to a string
    while (mongoc_cursor_next(cursor, &doc)) {
        str = bson_as_canonical_extended_json(doc, NULL);
        memcpy(buffer + buffer_filled, str, strlen(str));
        buffer_filled += strlen(str);
        bson_free(str);
    }

    // frees docs
    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);

    return;
}

int db_delete_profile(char *email, mongoc_client_t *client) {

    mongoc_collection_t *collection;
    bson_error_t error;
    bson_oid_t oid;
    bson_t *doc;
    int success = 0;

    // gets collection
    collection = mongoc_client_get_collection(client, DB_NAME, PROFILE_COLLECTION);

    // creates query: db.collection.deleteOne({ "email": email })
    doc = bson_new();
    bson_oid_init(&oid, NULL);
    BSON_APPEND_OID(doc, "_id", &oid);
    BSON_APPEND_UTF8(doc, "email", email);

    // tries to delete profile
    if (!mongoc_collection_delete_one(collection, doc, NULL, NULL, &error)) {
        printf("Delete failed: %s\n", error.message);
        success = -1;
    }

    // frees docs
    bson_destroy(doc);
    mongoc_collection_destroy(collection);

    return success;
}
