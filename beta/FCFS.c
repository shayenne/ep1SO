#include "FCFS.h"
#include "impressao.h"
#include <semaphore.h>
sem_t mutex;
sem_t inc;
sem_t cpu[CPUMAX];
sem_t manager;
cpu_set_t cpuset[CPUMAX];
pthread_attr_t attr[CPUMAX];

void *work(void * time) {
  long i = 0;
  clock_t ini = clock();
  clock_t fim = clock();
  sem_wait(&mutex);
  fprintf(stderr, "Comecei a thread %ld e estou usando a cpu: %d meu time é: %f \n",
	 pthread_self(), sched_getcpu(), *(double *) time);
  while ((double)(fim - ini)/CLOCKS_PER_SEC < *(double *)time) {
    i++;
    fim = clock();
  }
  fprintf(stderr, "Terminei a thread %ld e estou usando a cpu: %d meu time é: %f\n",
	  pthread_self(), sched_getcpu(), *(double *) time);
  sem_post(&mutex);
  return NULL;
  pthread_exit(NULL);
}

int maxCPU() {
  if (sysconf(_SC_NPROCESSORS_ONLN) > CPUMAX)
    return CPUMAX;
  return sysconf(_SC_NPROCESSORS_ONLN);
}


/* Devolve 0 se não pode receber processos e 1 se pode receber. */
void * gerente(void * proc) {
  int i, state, manstate;
  Processo p = *(Processo *) proc;
  sem_getvalue(&manager, &manstate);
  printf("Manstate de p: %d %s\n", manstate, p->nome);     

  sem_unlink("inc");
  sem_init(&inc, 0, 1);
  
  sem_wait(&manager);
  
  for (i = 0; i < maxCPU();) {
    
    sem_getvalue(&cpu[i], &state); 
    printf("State de p: %d %s CPU %d\n", state, p->nome, i);
    if (state == 1) {
      sem_wait(&cpu[i]);
      /*Colocar o processo nesta CPU*/
      pthread_create(&p->pid, &attr[i], work,(void *) &p->dt);
      pthread_join(p->pid, NULL);
      sem_post(&manager);
      sem_post(&cpu[i]);
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
void escalonadorFCFS(Link trace, FILE * saida) {
  Processo p;
  Link pronto;
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
    sem_unlink("cpu[i]");
    /* Inicialização do semáforo */
    sem_init(&cpu[i],0,1);
  }


  pronto = trace;
  trace = NULL;
  
  printf("N CPU = %d\n", maxCPU());
  i = 0;
  while (!queueEmpty(pronto)) {
    i++;
    p = queueGet(pronto);
    
    pthread_create(&thread[i], NULL, gerente, (void *) &p);
    sleep(1);
  }
   
  for (j = 0; j < i; j++)
    pthread_join(thread[j], NULL);
  
  for (i = 0; i < maxCPU(); i++) {
    pthread_attr_destroy(&attr[i]);
  }
  
  pthread_exit(NULL);
  queueFree(pronto);
  pronto = NULL;
}
