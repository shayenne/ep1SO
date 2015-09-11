/* 
 * MAC0122 Principios de Desenvolvimento de Algoritmos
 */

/* Interface para macros e funcoes de uso geral */

#ifndef _UTIL_H
#define _UTIL_H

#define _GNU_SOURCE
#define ENTER '\n'
#define CPUMAX 10
#include <stdio.h>
extern FILE * saida;

int maxCPU();

typedef enum 
{
    FALSE,
    TRUE
} 
Bool;

/*extern Bool verbose;*/

void *
mallocSafe (unsigned int n); 

/*void 
pause();*/

extern int verbose;


#endif

