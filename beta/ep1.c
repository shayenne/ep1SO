#include "ep1.h"


Link leProcessos(FILE * entrada, Link trace) {
	char  nome[20] = "\0";
	double t0, dt, deadline;
	int pr, k, linha = 1;
	Processo p;
	
	while(TRUE) {
		p = (Processo) malloc(sizeof(struct processo));
		k = fscanf(entrada, "%lf %s %lf %lf %d", &t0, nome, &dt, &deadline, &pr);
		
		if (k < 1)
			break;
		
		if (verbose)
			fprintf(stderr, "Trace: %4d   Processo: %s\n", linha, nome);
		
		linha++;
		p->t0 = t0;
		strcpy(p->nome, nome);
		p->dt = dt;
		p->deadline = deadline;
		p->p = pr;
		p->rtime = p->dt;
		trace = queuePut(p, trace);


/*		printf("ESSE E O P\n");
		printf("%f\n", p->t0);
		printf("%s\n", p->nome);
		printf("%f\n", p->dt);
		printf("%f\n", p->deadline);
		printf("%d\n", p->p);
		printf("%f\n", p->rtime);*/

	}
	return trace;
}

int main(int argc, char *argv[]) {
	FILE *entrada, *saida;
	Link trace;
	
	entrada = fopen(argv[2], "r");
	saida = fopen(argv[3], "w");
	
	if (argc == 5 && !strcmp(argv[4],"d\0"))
		verbose = TRUE;


	trace = queueInit(1);

	trace = leProcessos(entrada, trace);
	fclose(entrada);

	if (!strcmp(argv[1], "1"))
		escalonadorFCFS(trace, saida);

	if (!strcmp(argv[1], "2"))
		escalonadorSJF(trace, saida);


	if (!strcmp(argv[1], "4"))
		escalonadorRR(trace, saida);
	fclose(saida);


	return 0;
}
