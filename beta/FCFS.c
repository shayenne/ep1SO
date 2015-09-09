#include "FCFS.h"

void *work() {
    long i;

	printf("Entrei na work!\n");
    for (i = 0; i < 1E8; i++);
    
    printf("Esse é o i %ld\n", i);
	return NULL;
}

int maxCPU() {
           int s, j, rc, max;
           cpu_set_t cpuset;
           pthread_t thread;
           
           thread = pthread_self();

           /* Set affinity mask to include CPUs 0 to 7 */

           CPU_ZERO(&cpuset);
           for (j = 0; j < 8; j++)
               CPU_SET(j, &cpuset);

           s = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
           if (s != 0)
               handle_error_en(s, "pthread_setaffinity_np");

           /* Check the actual affinity mask assigned to the thread */

           s = pthread_getaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
           if (s != 0)
               handle_error_en(s, "pthread_getaffinity_np");

           printf("Set returned by pthread_getaffinity_np() contained:\n");
           for (j = 0; j < CPU_SETSIZE; j++)
               if (CPU_ISSET(j, &cpuset))
                   max = j;

		return max;
}

/*************************************************************
 * Pega os processosda fila e inicia a partir de t0. 
 * Caso não haja CPU disponível espera até liberar alguma.
 *************************************************************/
void escalonadorFCFS(Link trace, FILE * saida) {
	Processo p;
	Link pronto;
	int i = 0;
	int linha = 1;
	int mudanca = 0, rc;
	double tf;
	clock_t ini, fim, now;
	
	pronto = trace;
	trace = NULL;

	printf("N CPU = %d", maxCPU());

	ini = clock();
	while (!queueEmpty(pronto)) {

		p = queueGet(pronto);
		
		now = clock() / CLOCKS_PER_SEC;
		while (now < (clock_t) p->t0){
			
			now = clock()/ CLOCKS_PER_SEC;
		}
		printf("Sou instante! clock: %ld t0: %ld\n", clock() / CLOCKS_PER_SEC, (clock_t) p->t0);
			
		rc = pthread_create(&p->pid, NULL, &work, NULL);
		
		printf("Sou thread self: %ld pid: %ld\n", pthread_self(), p->pid);

		if (verbose)
			fprintf(stderr, "> Comecei o processo %ld  CPU %s ocupada\n", p->pid, "X");

		i++;/*BESTEIRA: Executar a thread*/

		if (verbose)
			fprintf(stderr, "> Pausei o processo %ld  CPU %s liberada\n", p->pid, "X");
		rc = pthread_join(p->pid, NULL);
		mudanca++;/*COLOCAR ISSO NO MOMENTO CERTO*/

		if (verbose)
			fprintf(stderr, "# Finalizei o processo %ld  (Saída: linha %d)\n", p->pid, linha);

		tf = 100;/*clock do fim do processo*/
		fprintf(saida, "%s %f %f\n", p->nome, tf, tf - p->t0);
		linha++;
	}

	fprintf(saida, "%d\n", mudanca);
	queueFree(pronto);
	pronto = NULL;
}
