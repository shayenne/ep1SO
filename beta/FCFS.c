#include "FCFS.h"


/*************************************************************
 * Pega os processosda fila e inicia a partir de t0. 
 * Caso não haja CPU disponível espera até liberar alguma.
 *************************************************************/
void escalonadorFCFS(Link trace, FILE * saida) {
	Processo p;
	Link pronto;
	int i = 0;
	int linha = 1;
	int mudanca = 0;
	double tf;

	pronto = trace;
	trace = NULL;


	while (!queueEmpty(pronto)) {

		p = queueGet(pronto);
		p->pid = i;/*BESTEIRA*/

		if (verbose)
			fprintf(stderr, "> Comecei o processo %5d  CPU %s ocupada\n", p->pid, "X");

		i++;/*BESTEIRA: Executar a thread*/

		if (verbose)
			fprintf(stderr, "> Pausei o processo %5d  CPU %s liberada\n", p->pid, "X");

		mudanca++;/*COLOCAR ISSO NO MOMENTO CERTO*/

		if (verbose)
			fprintf(stderr, "# Finalizei o processo %5d  (Saída: linha %d)\n", p->pid, linha);

		tf = 100;/*clock do fim do processo*/
		fprintf(saida, "%s %f %f\n", p->nome, tf, tf - p->t0);
		linha++;
	}

	fprintf(saida, "%d", mudanca);
	queueFree(pronto);
	pronto = NULL;
}
