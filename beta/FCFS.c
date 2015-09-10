#include "FCFS.h"
#include "impressao.h"
#include <semaphore.h>
sem_t mutex;
sem_t cpu[CPUMAX];
sem_t manager;


void *work(void * time) {
  long i = 0;
  clock_t ini = clock();
  clock_t fim = clock();
  sem_wait(&mutex);
  printf("Entrei na work! Sou a thread %ld e Estou usando a cpu: %d o meu time é :%f, ini:%f, fim:%f\n", pthread_self(), sched_getcpu(), *(double *) time, ini, fim);
	while (i < 1E7) {
	  i++;
	}
	printf("Esse é o i %ld sou thread %ld, cpu %d, meu fim: %f \n", i, pthread_self(), sched_getcpu(), fim - ini);
	sem_post(&mutex);
	return NULL;
}

int maxCPU() {
  if (sysconf(_SC_NPROCESSORS_ONLN) > CPUMAX)
    return CPUMAX;
  return sysconf(_SC_NPROCESSORS_ONLN);
}


/* Devolve 0 se não pode receber processos e 1 se pode receber. */
int gerente(Processo p) {
}

/*************************************************************
 * Pega os processosda fila e inicia a partir de t0. 
 * Caso não haja CPU disponível espera até liberar alguma.
 *************************************************************/
void escalonadorFCFS(Link trace, FILE * saida) {
  Processo p;
  Link pronto;
  
  int i, state, rc, manstate;
  
  cpu_set_t cpuset[CPUMAX];
  pthread_attr_t attr[CPUMAX];
  
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
  
  while (!queueEmpty(pronto)) {
    p = queueGet(pronto);
    
    /*printf("Li o processo p: %s\n", p->nome);*/
    	
    sem_getvalue(&manager, &manstate);
    printf("Manstate de p: %d %s\n", manstate, p->nome);
    
    if (manstate > 0) {
      
      sem_wait(&manager);
	
      for (i = 0; i < maxCPU();i++) {
	sem_getvalue(&cpu[i], &state); 
	printf("State de p: %d %s CPU %d\n", state, p->nome, i);
	if (state == 1) {
	    sem_wait(&cpu[i]);
	    /*Colocar o processo nesta CPU*/
	    rc = pthread_create(&p->pid, &attr[i], work,(void *) &p->dt);
	    
	    /*pthread_join(p->pid, NULL);*/
	    sem_post(&manager);
	    sem_post(&cpu[i]);
	    break;
	}

      }
	
      
      printf("O gerente recebeu o processo %s\n", p->nome);
    }
  }

      for (i = 0; i < maxCPU(); i++) {
	pthread_attr_destroy(&attr[i]);
      }

      pthread_exit(NULL);
  queueFree(pronto);
  pronto = NULL;
}
