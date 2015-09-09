#include "FCFS.h"
#include "impressao.h"
#include <semaphore.h>
sem_t mutex;
sem_t cpu;

void *work(void * time) {
    long i = 0;
	clock_t ini = clock();
	clock_t fim = clock();
	sem_wait(&mutex);
	printf("Entrei na work! Sou a thread %ld e Estou usando a cpu: %d o meu time é :%f, ini:%f, fim:%f\n", pthread_self(), sched_getcpu(), *(double *) time, ini, fim);
	while ((fim - ini)/ CLOCKS_PER_SEC < *(clock_t *)time) {
		fim = clock() ;
		i++;
    }
    printf("Esse é o i %ld sou thread %ld, cpu %d, meu fim: %f \n", i, pthread_self(), sched_getcpu(), fim - ini);
    sem_post(&mutex);
	return NULL;
}

int maxCPU() {
	return sysconf(_SC_NPROCESSORS_ONLN);
}

/*************************************************************
 * Pega os processosda fila e inicia a partir de t0. 
 * Caso não haja CPU disponível espera até liberar alguma.
 *************************************************************/
void escalonadorFCFS(Link trace, FILE * saida) {
	Processo p;
	Link pronto;
	int i = 0, j;
	int linha = 1;
	int mudanca = 0, rc;
	double tf;
	int s, nt =0;
	clock_t ini, fim, now;
	int cpus[2] = {0, 0};
	pthread_t threads[20];
	 pthread_attr_t attr;
	
	sem_unlink("mutex");
	/* Inicialização do semáforo */
	sem_init(&mutex,0,1);
	

	sem_unlink("cpus");
	/* Inicialização do semáforo */
	sem_init(&cpu, 0, 1);
	
	
	pthread_attr_init(&attr);
	
	cpu_set_t cpuset;

	CPU_ZERO(&cpuset);
	CPU_SET(0, &cpuset);
	
	pronto = trace;
	trace = NULL;

	printf("N CPU = %d\n", maxCPU());

	ini = clock();
	while (!queueEmpty(pronto)) {

		p = queueGet(pronto);
		threads[nt++] = p->pid;
		
		now = clock() / CLOCKS_PER_SEC;
		while (now < (clock_t) p->t0){
			
			now = clock()/ CLOCKS_PER_SEC;
		}
		printf("Sou instante! clock: %ld t0: %ld\n", clock() / CLOCKS_PER_SEC, (clock_t) p->t0);

		s = pthread_attr_init(&attr);
		
		
		sem_wait(&cpu);
		pthread_attr_setaffinity_np(&attr,sizeof(cpu_set_t),&cpuset);
		rc = pthread_create(&p->pid, &attr, &work, (void *) &p->dt);
		sem_post(&cpu);


		printf("Esse é o s do set: %d\n", s);
		
		

		/*s = pthread_getaffinity_np(p->pid, sizeof(cpu_set_t), &cpuset);*/
		printf("Esse é o s do get: %d\n", s);
		printf("Sou thread self: %ld pid: %ld\n", pthread_self(), p->pid);

		if (verbose)
			fprintf(stderr, "> Comecei o processo %ld  CPU %d ocupada\n", p->pid, sched_getcpu());

		i++;/*BESTEIRA: Executar a thread*/

		if (verbose)
			fprintf(stderr, "> Pausei o processo %ld  CPU %s liberada\n", p->pid, "X");
		
		mudanca++;/*COLOCAR ISSO NO MOMENTO CERTO*/

		if (verbose)
			fprintf(stderr, "# Finalizei o processo %ld  (Saída: linha %d)\n", p->pid, linha);

		tf = 100;/*clock do fim do processo*/
		fprintf(saida, "%s %f %f\n", p->nome, tf, tf - p->t0);
		linha++;
		
		
	}

	pthread_exit(NULL);
	
		
	fprintf(saida, "%d\n", mudanca);
	queueFree(pronto);
	pronto = NULL;
}
