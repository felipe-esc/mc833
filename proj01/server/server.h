/*
 * BlaBla
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
#define SERVER_ADMIN_PASSWORD "1234"

/* types */


/* functions */
void handle_messages(int, mongoc_client_t*);
// int register_profile(char *, mongoc_client_t*);
// int add_new_experiences(char *, mongoc_client_t*);
// char* list_by_course(char *, mongoc_client_t*); // NULL se não achar
// char* list_by_skill(char *, mongoc_client_t*);
// char* list_by_graduation_year(char *, mongoc_client_t*);
// char* list_all(mongoc_client_t*);
// char* find_by_email(char *, mongoc_client_t*);
// int delete_user(char *, mongoc_client_t*); // retornar 0 se ok 
bool check_admin(char *, char *);

#endif

/* 
 *  Usuário:
 *      Email <string[50]>: maria_silva@gmail.com 
 *      Nome <string[30]>: Maria 
 *      Sobrenome <string[170]: Silva
 *      Residência <string[50]>: Campinas
 *      Formação Acadêmica <string[50]>: Ciência da Computação
 *      Ano de Formatura <int>: 2015
 *      Habilidades <string[500]>: Análise de Dados, Internet das Coisas, Computação em Nuvem
 *      Experiência <array<string[250]>>: (1) Estágio de 1 ano na Empresa X, onde trabalhei como analista de dados
 *      (2) Trabalhei com IoT e Computação em Nuvem por 5 anos na Empresa Y
*/
