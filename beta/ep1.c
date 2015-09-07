#include <stdio.h>  /* fprintf(), fscanf() */
#include <stdlib.h> /* EXIT_FAILURE, EXIT_SUCESS, malloc(), free() */
#include <string.h> /* strcmp() */
#include "queue.h"  /* queueInit(), queueEmpty(), queuePut(), 
                       queueGet(), queueFree() */
#include "item.h"
#include "util.h"   /* mallocSafe(), pause(), Bool */
#include "ep1.h"



void leProcessos(FILE * entrada) {
	char  nome[20] = "\0";
	double t0, dt, deadline;
	int pr, k, linha = 0;
	Processo p, q;
	
	while(TRUE) {
		p = (Processo) malloc(sizeof(struct processo));
		k = fscanf(entrada, "%lf %s %lf %lf %d", &t0, nome, &dt, &deadline, &pr);

		if (k < 1)
			break;
		linha++;
		p->t0 = t0;
		strcpy(p->nome, nome);
		p->dt = dt;
		p->deadline = deadline;
		p->p = pr;
		p->rtime = p->dt;
		queuePut(p);
		q = queueGet();
		printf("ESSE E O P\n");
		printf("%f\n", p->t0);
		printf("%s\n", p->nome);
		printf("%f\n", p->dt);
		printf("%f\n", p->deadline);
		printf("%d\n", p->p);
		printf("%f\n", p->rtime);
		printf("ESSE E O Q\n");
		printf("%f\n", q->t0);
		printf("%s\n", q->nome);
		printf("%f\n", q->dt);
		printf("%f\n", q->deadline);
		printf("%d\n", q->p);
		printf("%f\n", q->rtime);

	}
	
}

int main(int argc, char *argv[]) {
	FILE *entrada, *saida;
	
	entrada = fopen(argv[2], "r");
	saida = fopen(argv[3], "w");
	
	if (argc == 5 && !strcmp(argv[4],"d\0")){
		verbose = TRUE;
		}
	if (verbose)
		fprintf(stderr, "FALEI QUE DEU ERRO!\n");
	
	queueInit(1);

	leProcessos(entrada);
	fclose(entrada);
	
	fprintf(saida, "Fiz mudancas de contexto :D\n");
	fclose(saida);
	queueFree();

	return 0;
}
