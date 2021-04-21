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

#include "../shared.h"

/* constants */
#define SERVER_MAX_CONNECTIONS 5

/* types */


/* functions */
void handle_messages(int);
// int register_profile(char *);
// int add_new_experiences(char *);
// user** list_by_course(char *);
// user** list_by_skill(char *);
// user** list_by_graduation_year(char *);
// user** list_all();
// user* find_by_email(char *);
// int delete_user(char *); // retornar 0 se ok 

#endif

/* 
 *  Usuário:
 *      Email <string[50]>: maria_silva@gmail.com 
 *      Nome <string[30]>: Maria 
 *      Sobrenome <string[170]: Silva
 *      Residência <string[50]>: Campinas
 *      Formação Acadêmica <string[50]>: Ciência da Computação
 *      Ano de Formatura <int>: 2015
 *      Habilidades <array<string[100]>>: Análise de Dados, Internet das Coisas, Computação em Nuvem
 *      Experiência <array<string[250]>>: (1) Estágio de 1 ano na Empresa X, onde trabalhei como analista de dados
 *      (2) Trabalhei com IoT e Computação em Nuvem por 5 anos na Empresa Y
*/
