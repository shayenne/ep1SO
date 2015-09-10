#include "SJF.h"
#include "impressao.h"
#include <semaphore.h>
sem_t mutex;
sem_t inc;
sem_t cpu[CPUMAX];
sem_t manager;
cpu_set_t cpuset[CPUMAX];
pthread_attr_t attr[CPUMAX];


int cmpfunc (const void * a, const void * b)
{
   const Processo * ia = (const Processo *)a; 
    const Processo * ib = (const Processo *)b;
    return ((*ia)->dt  - (*ib)->dt); 
   
}

void *workSJF(void * time) {
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

int maxCPU2() {
  if (sysconf(_SC_NPROCESSORS_ONLN) > CPUMAX)
    return CPUMAX;
  return sysconf(_SC_NPROCESSORS_ONLN);
}


/* Devolve 0 se não pode receber processos e 1 se pode receber. */
void * gerenteSJF(void * proc) {
  int i, state, manstate;
  Processo p = *(Processo *) proc;
  sem_getvalue(&manager, &manstate);
  printf("Manstate de p: %d %s\n", manstate, p->nome);

  sem_unlink("inc");
  sem_init(&inc, 0, 1);
  
  sem_wait(&manager);
  
  for (i = 0; i < maxCPU2();) {
    
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
void escalonadorSJF(Link trace, FILE * saida) {
  Processo p, atual, new, *array;
  Link prontos, juntos;
  pthread_t thread[100];
  
  int i, j, tam;
  

  
  /*thread = pthread_self();*/
  
  /* Inicializa conjunto de CPU's disjuntos */
  for (i = 0; i < maxCPU2(); i++) {
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
  sem_init(&manager,0,maxCPU2());
  
  for (i = 0; i < maxCPU2(); i++) {
    sem_unlink("cpu[i]");
    /* Inicialização do semáforo */
    sem_init(&cpu[i],0,1);
  }




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
      
      qsort(array, tam, sizeof(Processo), cmpfunc);
      
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



  i = 0;
  while (!queueEmpty(prontos)) {
    i++;
    p = queueGet(prontos);
    
    pthread_create(&thread[i], NULL, gerenteSJF, (void *) &p);
    sleep(1);
  }
   
  for (j = 0; j < i; j++)
    pthread_join(thread[j], NULL);
  
  for (i = 0; i < maxCPU2(); i++) {
    pthread_attr_destroy(&attr[i]);
  }
  
  pthread_exit(NULL);
  queueFree(prontos);
  prontos = NULL;
}
