#include "RR.h"
#include "impressao.h"
#include <semaphore.h>

/*Numero em milisegundos*/
#define quantum 0.100

sem_t mutex;
sem_t inc;
pthread_mutex_t cpu[CPUMAX];
sem_t manager;
cpu_set_t cpuset[CPUMAX];
pthread_attr_t attr[CPUMAX];
pthread_cond_t cond[CPUMAX];

Link pronto;

void *workRR(void * proc[]) {
  Processo p = *(Processo *) proc[0];
  int x = *(int *) proc[1];
  long i = 0;
  clock_t ini = clock();
  clock_t fim = clock();



  sem_wait(&mutex);

  fprintf(stderr, "Recebi no p: t0 %f\n", p->t0);
  fprintf(stderr, "Comecei a thread %ld e estou usando a cpu: %d meu time é: %f \n",
	 pthread_self(), sched_getcpu(), p->dt);
  while (p->rtime > 0) {

    while ((double)(fim - ini)/CLOCKS_PER_SEC < quantum) {
      i = (i + 1)%1000;
      fim = clock();
    }
    
    pthread_cond_wait(&cond[x], &cpu[x]);
    printf("São e salvo!\n");
    ini = clock();
  }
  fprintf(stderr, "Terminei a thread %ld e estou usando a cpu: %d meu time é: %f\n",
	  pthread_self(), sched_getcpu(), p->dt);
  sem_post(&mutex);
 
  pthread_exit(NULL);
}


/* Devolve 0 se não pode receber processos e 1 se pode receber. */
void * gerenteRR(void * proc) {

  int i, state, manstate;
  Processo p = *(Processo *) proc;
  clock_t ini, now;
  void * vet[2];



  sem_getvalue(&manager, &manstate);
  printf("Manstate de p: %d %s\n", manstate, p->nome);     

  sem_unlink("inc");
  sem_init(&inc, 0, 1);
  printf(" Tentei entrar no semaforo principal %s\n", p->nome);
  sem_wait(&manager);
  printf("Entrei no semaforo principal %s\n", p->nome);
  for (i = 0; i < maxCPU();) {
    
    sem_getvalue(&cpu[i], &state); 
    printf("State de p: %d %s CPU %d\n", state, p->nome, i);
    if (!pthread_mutex_trylock(&cpu[i])) {
      ini = clock();
      now = clock();
      /*Colocar o processo nesta CPU*/
      printf("Resultado do cond\n");
      if (p->rtime == p->dt) {
	vet[0] = (void *) &p;
	vet[1] = (void *) &i;
	pthread_create(&p->pid, &attr[i], workRR,(void *) &vet);
      }
      else
	pthread_cond_signal(&cond[i]);
      while (p->rtime > 0 && (now - ini)/CLOCKS_PER_SEC < quantum)
	now = clock();
      if ((now - ini)/CLOCKS_PER_SEC >= quantum) {
	printf("\n PASSEI DO QUANTUM\n");
	printf("Esse é meu antigo rtime %f\n", p->rtime);
	p->rtime -= (now - ini)/CLOCKS_PER_SEC;
	printf("Esse é meu novo rtime %f\n", p->rtime);
	if (p->rtime > 0) {
	  
	  pronto = queuePut(p, pronto);
	  printf("Coloquei o %s na fila?\n",p->nome );
	 
	}
	else
	  pthread_join(p->pid, NULL);
      }
      pthread_mutex_unlock(&cpu[i]);
      sem_post(&manager);
      break;
    }
    sem_wait(&inc);
    i++;
    sem_post(&inc);
  }
        
  printf("O gerente recebeu o processo %s\n", p->nome);
  pthread_exit(NULL);
  return NULL;
}

/*************************************************************
 * Pega os processosda fila e inicia a partir de t0. 
 * Caso não haja CPU disponível espera até liberar alguma.
 *************************************************************/
void escalonadorRR(Link trace, FILE * saida) {
  Processo p;

  pthread_t thread[100];
  
  int i, j;
  

  
  /*thread = pthread_self();*/
  
  /* Inicializa conjunto de CPU's disjuntos */
  for (i = 0; i < maxCPU(); i++) {
    CPU_ZERO(&cpuset[i]);
    CPU_SET(i, &cpuset[i]);
    
    pthread_attr_init(&attr[i]);
    pthread_attr_setaffinity_np(&attr[i], sizeof(cpu_set_t), &cpuset[i]);
  }

  sem_unlink("mutex");
  /* Inicialização do semáforo */
  sem_init(&mutex,0,1);
  
  sem_unlink("manager");
  /* Inicialização do semáforo */
  sem_init(&manager,0,maxCPU());
  
  for (i = 0; i < maxCPU(); i++) {
    pthread_mutex_init(&cpu[i], NULL);
    pthread_cond_init(&cond[i], NULL);
  }


  pronto = trace;
  trace = NULL;
  
  printf("N CPU = %d\n", maxCPU());
  printf("Quantum é %ld\n", quantum/CLOCKS_PER_SEC);
  i = 0;
  while (!queueEmpty(pronto)) {
    i++;
    p = queueGet(pronto);
    
    pthread_create(&thread[i], NULL, gerenteRR, (void *) &p);
    sleep(1);
  }
   


  
  for (j = 0; j < i; j++){
    printf("%d \n",j);
    pthread_join(thread[j], NULL);
    }

  
 
  for (i = 0; i < maxCPU(); i++) {
    pthread_attr_destroy(&attr[i]);
    pthread_mutex_destroy(&cpu[i]);
    pthread_cond_destroy(&cond[i]);
  }
  
  pthread_exit(NULL);
  queueFree(pronto);
  pronto = NULL;
}
