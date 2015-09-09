#include "SRTN.h"


int cmpfuncSRTN (const void * a, const void * b)
{
   const Processo * ia = (const Processo *)a; 
    const Processo * ib = (const Processo *)b;
    return ((*ia)->dt  - (*ib)->dt); 
   
}


void escalonadorSRTN(Link trace, FILE * saida) {
	Link prontos, juntos;
	Processo * array;
	Processo atual, new, p;
	double tf;
	int tam, i = 0, mudanca = 0, linha = 1;

	prontos = queueInit(1);

	atual = queueGet(trace);
	while (atual != NULL) {
		juntos = queueInit(1);
		new = atual;
		tam = 0;
		while (new != NULL && new->t0 == atual->t0) {
			juntos = queuePut(new, juntos);
			tam++;
			if (!queueEmpty(trace))
				new = queueGet(trace);
			else
				new = NULL;
		}
		atual = new;

		if (tam > 1) {
			array = (Processo *) malloc (tam * sizeof(Processo));

			for (i = 0; i < tam; i++)
				array[i] = queueGet(juntos);

			free(juntos);

			qsort(array, tam, sizeof(Processo), cmpfuncSRTN);

			printf("\nOrdenação\n");
			for (i = 0; i < tam; i++){
				printf("=== %s %f\n", array[i]->nome, array[i]->dt);
				prontos = queuePut(array[i], prontos);
			}
		}
		else
			if (tam == 1)
				prontos = queuePut(queueGet(juntos), prontos);
	}



	while (!queueEmpty(prontos)) {

		p = queueGet(prontos);
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

	fprintf(saida, "%d\n", mudanca);
	queueFree(prontos);

}
