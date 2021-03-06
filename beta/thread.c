#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>

pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_var   = PTHREAD_COND_INITIALIZER;


void *work(void *threadid) {
    long i;
    long *ptid = (long *) threadid;
    clock_t ini, fim;

    //if (tid == 3)
    //   for (i = 0; i < 1E8; i++);

    ini = clock();
    printf("COMECEI  Sou thread #%ld\t Clock %ld \n", *ptid,
            ini);

    for (i = 0; i < 1E8; i++);

    fim = clock();
    printf("TERMINEI Sou thread #%ld\t Clock %ld \n", *ptid,
            fim);
    pthread_exit(NULL);
}

int main() {
    long i;
    clock_t time;

    pthread_t threads[10];
    pthread_t tid, mythread;
    int ret;
    int rc;
    int num_thread = 10;
    int interrupt = 2;
    int cont = 19;

    for (i = 0; i < num_thread; i++) {

        printf("In main: creating thread %ld\n", i);
        rc = pthread_create(&threads[i], NULL, &work, (void *) &i);

        if (i == 3) {
            pthread_kill(threads[i], SIGSTOP);
            printf("In main: thread %ld SUSPEND\n", i);
        }

        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }



    pthread_kill(threads[3], SIGCONT);
    printf("In main: LIBERADO\n");
    pthread_exit(NULL);


    return 0;

}

