#include <stdio.h>
#include "impressao.h"

void printInicioProcesso(void * proc, void* cpu) {
	fprintf(stderr, "> Comecei o processo %ld  CPU %ld ocupada\n", (long) proc, (long) cpu);
}
