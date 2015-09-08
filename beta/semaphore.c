#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <semaphore.h>
 
#define NUM_THREADS     50

/* a é minha variável compartilhada pelas threads */
int a;

/* mutex é o semáforo que vou usar para controlar o acesso à seção
 * crítica */
sem_t mutex;

void *TaskCode(void *argument)
{
   int tid;
   int i;

   tid = *((int *) argument);

   sem_wait(&mutex);
   for (i=0;i<100000;i++) {
      /* Início da seção crítica */
      a++;
      /* Fim da seção crítica */
   }
   sem_post(&mutex);

   fprintf(stderr,"[Olah, eu sou a thread %d e a=%d!]\n", tid,a);

   return NULL;
}
 
int main(void)
{
   pthread_t threads[NUM_THREADS];
   int thread_args[NUM_THREADS];
   int rc,i;

   a=0;
   /* Sempre é bom ter certeza que não existe um mutex já criado com
    * mesmo nome por outro processo no passado */
   sem_unlink("mutex");
   /* Inicialização do semáforo */
   sem_init(&mutex,0,1);
   
 
   /* criando todas as threads */
   for (i=0; i<NUM_THREADS; ++i) {
      thread_args[i] = i;
      rc = pthread_create(&threads[i], NULL, TaskCode, (void *) &thread_args[i]);
      assert(0==rc); /* se der algum erro na criacao das threads, saio do programa */
   }
 
   /* esperando todas as threads terminarem */
   for (i=0; i<NUM_THREADS; ++i) {
      rc = pthread_join(threads[i], NULL);
      assert(0 == rc);
   }

   /* Para não deixar o mutex criado (pode dar problema se rodar o
    * código de novo) */
   sem_destroy(&mutex); 

   fprintf(stderr,"[Valor final de a=%d]\n",a);
   
   return(0);
}
