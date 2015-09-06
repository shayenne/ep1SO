#include <stdio.h>  /* fprintf(), fscanf() */
#include <stdlib.h> /* EXIT_FAILURE, EXIT_SUCESS, malloc(), free() */
#include <string.h> /* strcmp() */
#include "queue.h"  /* queueInit(), queueEmpty(), queuePut(), 
                       queueGet(), queueFree() */
#include "item.h"
#include "util.h"   /* mallocSafe(), pause(), Bool */

void leProcessos() {
	char buffer[1000], nome[20] = "\0";
	double t0, dt, deadline;
	int pr;
	Processo p, q;
	
	
	while(fgets(buffer, sizeof(buffer), stdin) != NULL) {
		p = (Processo) malloc(sizeof(struct processo));
		sscanf(buffer, "%lf %s %lf %lf %d", &t0, nome, &dt, &deadline, &pr);
		
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

int main() {
	queueInit(1);

	leProcessos();

	queueFree();

	return 0;
}
