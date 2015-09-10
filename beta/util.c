/* 
 * MAC0122 Principios de Desenvolvimento de Algoritmos
 */

/* Implementacao de funcoes de uso geral */
#define _GNU_SOURCE
#include <sched.h>
#include "util.h" /* interface para este modulo */
#include <unistd.h>
#include <stdlib.h> /* malloc(), EXIT_FAILURE */
#include <stdio.h> /* fprintf(), stderr */

int verbose;

int maxCPU() {
  if (sysconf(_SC_NPROCESSORS_ONLN) > CPUMAX)
    return CPUMAX;
  return sysconf(_SC_NPROCESSORS_ONLN);
}


void *
mallocSafe (unsigned int n) 
{
    void *p;

    p = malloc(n);
    if (p == NULL)
    {
	fprintf(stderr,"stack: malloc de %u bytes falhou.\n", n);
	exit (EXIT_FAILURE);
    }

    return p;
}

/*void 
pause()
{
    char ch;
    printf("Tecle ENTER para continuar.\n");
    do
	scanf("%c", &ch);
    while (ch != ENTER);
}*/
