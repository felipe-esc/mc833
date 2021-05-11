/*
 * Server TCP - Headers
 *
 * Responsibles: 
 *      Felipe Escórcio de Sousa - RA: 171043
 *      Ricardo Ribeiro Cordeiro - RA: 186633
 * 
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

/* types */


/* functions */
void handle_messages(int, mongoc_client_t*);
bool check_admin(char *);
void register_profile(int, char *, mongoc_client_t*);
void add_new_experiences(int, char *, mongoc_client_t*);
void list_by_course(int, char *, mongoc_client_t*);
void list_by_skill(int, char *, mongoc_client_t*);
void list_by_graduation_year(int, char *, mongoc_client_t*);
void list_all(int, mongoc_client_t*);
void find_by_email(int, char *, mongoc_client_t*);
void delete_profile(int, char *, mongoc_client_t*);


#endif

/* 
 *  Usuário:
 *      Email <string[50]>: maria_silva@gmail.com 
 *      Nome <string[30]>: Maria 
 *      Sobrenome <string[200]: Silva
 *      Residência <string[50]>: Campinas
 *      Formação Acadêmica <string[50]>: Ciência da Computação
 *      Ano de Formatura <int>: 2015
 *      Habilidades <string[500]>: Análise de Dados, Internet das Coisas, Computação em Nuvem
 *      Experiência <array<string[250]>>: (1) Estágio de 1 ano na Empresa X, onde trabalhei como analista de dados
 *      (2) Trabalhei com IoT e Computação em Nuvem por 5 anos na Empresa Y
*/
