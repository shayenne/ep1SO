#include "FCFS.h"
#include "impressao.h"
#include <semaphore.h>
sem_t mutex;
sem_t inc;
sem_t cpu[CPUMAX];
sem_t manager;
cpu_set_t cpuset[CPUMAX];
pthread_attr_t attr[CPUMAX];

void *work(void * proc) {
  Processo p = *(Processo *) proc;
  long i = 0;
  clock_t ini = clock();
  clock_t fim = clock();
  sem_wait(&mutex);
  
  if (verbose)
    fprintf(stderr, "Comecei a thread %ld e estou usando a cpu: %d\n", pthread_self(), sched_getcpu());
  while ((double)(fim - ini)/CLOCKS_PER_SEC < (double) p->dt) {
    i = (i + 1)%1000;
    fim = clock();
  }
  if (verbose)
    fprintf(stderr, "Terminei a thread %ld e estou usando a cpu: %d\n", pthread_self(), sched_getcpu());
  
  fprintf(saida, "%s %f %f\n", p->nome, (double)fim/CLOCKS_PER_SEC,(double) (fim-ini)/CLOCKS_PER_SEC);
  sem_post(&mutex);
  return NULL;
  pthread_exit(NULL);
}



void * gerente(void * proc) {
  int i, state, manstate;
  Processo p = *(Processo *) proc;
  sem_getvalue(&manager, &manstate);
  
  
  sem_unlink("inc");
  sem_init(&inc, 0, 1);
  
  sem_wait(&manager);
  
  for (i = 0; i < maxCPU();) {
    
    sem_getvalue(&cpu[i], &state); 
    
    if (state == 1) {
      sem_wait(&cpu[i]);
      /*Colocar o processo nesta CPU*/
      pthread_create(&p->pid, &attr[i], work,(void *) &p);
      pthread_join(p->pid, NULL);
      sem_post(&cpu[i]);
      sem_post(&manager);
      break;
    }
    sem_wait(&inc);
    i++;
    sem_post(&inc);
  }
  
  
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
  
  int i, j, linha = 0;
  
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
  
  
  i = 0;
  while (!queueEmpty(pronto)) {
    i++;
    p = queueGet(pronto);
    
    pthread_create(&thread[i], NULL, gerente, (void *) &p);
    linha++;
    fprintf(stderr, "O processo %s acabou (Saída: linha %d)\n", p->nome, linha);
    
    sleep(1);
  }
   

  /* for (j = 0; j < i; j++){ */
  /*   pthread_join(thread[j], NULL); */
  /* } */
 
  for (i = 0; i < maxCPU(); i++) {
    pthread_attr_destroy(&attr[i]);
  }
  
  
  fprintf(saida,"0\n");
  if (verbose)
    fprintf(stderr, "Quantidade de mudancas de contexto: 0\n");
  pthread_exit(NULL);
  queueFree(pronto);
  pronto = NULL;
}
