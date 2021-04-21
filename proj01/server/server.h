/*
 * BlaBla
 */

#ifndef SERVER_H
#define SERVER_H

// includes
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

/* constants */
#define SERVER_MAX_CONNECTIONS 5

/* structs */


/* functions */
void handle_messages(int);
// void register_profile();
// void add_new_experiences();
// user** list_by_course();
// user** list_by_skill();
// user** list_by_graduation_year();
// user** list_all();
// user* find_by_email();
// int delete_user(); // retornar 0 se ok 

#endif

/* 
Email: maria_silva@gmail.com
Nome: Maria Sobrenome: Silva
Residência: Campinas
Formação Acadêmica: Ciência da Computação
Ano de Formatura: 2015
Habilidades: Análise de Dados, Internet das Coisas, Computação em Nuvem
Experiência: (1) Estágio de 1 ano na Empresa X, onde trabalhei como analista de dados
(2) Trabalhei com IoT e Computação em Nuvem por 5 anos na Empresa Y
*/