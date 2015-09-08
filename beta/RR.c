#include "RR.h"

/* Tempo para cada processo executar */
#define quantum 100
/*************************************************************
 * Pega os processosda fila e inicia a partir de t0. 
 * Caso não haja CPU disponível espera até liberar alguma.
 *************************************************************/
void escalonadorRR(Link trace, FILE * saida) {
	Processo p;
	Link prontos;
	int i = 0;
	int linha = 1;
	int mudanca = 0;
	double tf;


	prontos = trace;
	trace = NULL;


	while (!queueEmpty(prontos)) {

		p = queueGet(prontos);
		p->pid = i;/*BESTEIRA*/

		if (verbose)
			fprintf(stderr, "> Comecei o processo %5d  CPU %s ocupada\n", p->pid, "X");

		i++;/*BESTEIRA: Executar a thread*/

		if (verbose)
			fprintf(stderr, "> Pausei o processo %5d  CPU %s liberada\n", p->pid, "X");
		
		/* Atualiza o tr do processo pausado*/
		/* Se o processo não acabou */
		if (p->rtime > 0) {
			p->rtime = 0;
			prontos = queuePut(p, prontos);
		}
		else {
			if (verbose)
				fprintf(stderr, "# Finalizei o processo %5d  (Saída: linha %d)\n", p->pid, linha);
			free(p);
			
			tf = 100;/*clock do fim do processo*/
			fprintf(saida, "%s %f %f\n", p->nome, tf, tf - p->t0);
			linha++;
		}
		mudanca++;/*COLOCAR ISSO NO MOMENTO CERTO*/




	}

	fprintf(saida, "%d\n", mudanca);
	queueFree(prontos);

}
