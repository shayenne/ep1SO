#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NMAX 100
#define NAME 20


typedef struct processo {
	double t0;
	char nome[NAME];
	double dt;
	double deadline;
	int p;
	double rtime;
} Processo;


typedef struct lista {
	Processo * proc;
	struct lista *prox;
} Lista;

void printProcesso(Processo *p) {

	//printf("\nProcesso: %lf %s %lf %lf %d %lf\n", p->t0, p->nome, p->dt, p->deadline, p->p, p->rtime);
	printf("\nProcesso: %f %s\n", p->t0,p->nome);
}

void freeLista(Lista *l) {
	Lista * tmp;
	
	while (l != NULL) {
		tmp = l->prox;
		free(l->proc);
		free(l);
		l = tmp;
	}
}

Lista * addProcesso(Processo *p, Lista *l) {
	Lista *new = malloc(sizeof(Lista));
	printf("Aqui não\n");
	new->proc = p;
	printf("Coloquei o p no new");
	new->prox = l;
	printf("Coloquei o l no prox");
	
	return new;
}

Lista * leProcessos() {
	char buffer[NMAX];
	Processo *p;
	Lista *l = NULL;
	
	while(fgets(buffer, sizeof(buffer), stdin) != NULL) {
	//while(scanf("%s", buffer) == 0) {
		p = malloc(sizeof(Processo));
		sscanf(buffer, "%lf %s %lf %lf %d", &p->t0, p->nome, &p->dt, &p->deadline, &p->p);
		printf("%f", p->t0);
		printf("%s", p->nome);
		printf("%f", p->dt);
		printf("%f", p->deadline);
		printf("%d", p->p);
		
		p->rtime = p->dt;
		
		printProcesso(p);
		l = addProcesso(p, l);
	}
}

void *thread_function(void *arg) {
  int i, j;
  for ( i=0; i<10; i++ ) {
    printf("Thread says hi!\n");
    for (j = 0; j < 1000; j++)
  	printf("Oi %d\n", j);
    sleep(3);
  }
  return NULL;
}

int main(void) {
	Lista *l;
	
	l = leProcessos();
	freeLista(l);
	/*
  pthread_t mythread;
  
  if ( pthread_create( &mythread, NULL, thread_function, NULL) ) {
    printf("error creating thread.");
    abort();
  }

  if ( pthread_join ( mythread, NULL ) ) {
    printf("error joining thread.");
    abort();
  }
*/
  exit(0);

}
