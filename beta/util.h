/* 
 * MAC0122 Principios de Desenvolvimento de Algoritmos
 */

/* Interface para macros e funcoes de uso geral */

#ifndef _UTIL_H
#define _UTIL_H

#define ENTER '\n'

typedef enum 
{
    FALSE,
    TRUE
} 
Bool;

/*extern Bool verbose;*/

void *
mallocSafe (unsigned int n); 

void 
pause();

#endif

