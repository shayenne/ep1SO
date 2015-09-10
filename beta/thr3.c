#define _GNU_SOURCE

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sched.h>

#define handle_error_en(en, msg)				\
  do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)


static int  num_thread = 4;
static cpu_set_t cpuset;

void *work(void *threadid);


void *work(void *threadid){
  int sched_getcpu(void);
  long i;
  long tid = (long) threadid;
  clock_t time;
  
  time = clock();
  printf("COMECEI  Sou thread #%ld\t Clock %ld, CPU#%d \n",tid, time / CLOCKS_PER_SEC, sched_getcpu());

  for (i = 0; i < 1E9; i++);
    // printf("Sou thread #%ld\t CPU#%d\t i = %ld \n",tid, sched_getcpu(), i);   




  time = clock();
  printf("TERMINEI  Sou thread #%ld\t Clock %ld, CPU#%d \n",tid, time / CLOCKS_PER_SEC, sched_getcpu());
  //  printf("TERMINEI Sou thread #%ld\t Clock %ld \n",tid, time / CLOCKS_PER_SEC);

}


int main(){
  long i, j, s;
  clock_t time;
  cpu_set_t mask;

  pthread_t threads[num_thread];
  pthread_t tid, mythread;
  pthread_t thread;
  pthread_attr_t pt1, pt2, pt3, pt4;
  int ret;
  int rc;
  

  thread = pthread_self();

  CPU_ZERO(&cpuset);
  for (j = 0; j < num_thread; j++)
    CPU_SET(j, &cpuset);

  pthread_attr_init(&pt1);
  pthread_attr_init(&pt2);
  pthread_attr_init(&pt3);
  pthread_attr_init(&pt4);

  pthread_attr_setaffinity_np(&pt1, sizeof(cpuset), &cpuset);
  pthread_attr_setaffinity_np(&pt2, sizeof(cpuset), &cpuset);
  pthread_attr_setaffinity_np(&pt3, sizeof(cpuset), &cpuset);
  pthread_attr_setaffinity_np(&pt4, sizeof(cpuset), &cpuset);

  /*
  s = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
  if (s != 0)
    handle_error_en(s, "pthread_setaffinity_np");
  */
  /* Check the actual affinity mask assigned to the thread */
  /*
  s = pthread_getaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
  if (s != 0)
    handle_error_en(s, "pthread_getaffinity_np");
  */
  // printf("Set returned by pthread_getaffinity_np() contained:\n");
//for (j = 0; j < CPU_SETSIZE; j++)
    //if (CPU_ISSET(j, &cpuset))
      //      printf("    CPU %ld\n", j);
  
  


  for (i = 0; i < num_thread; i++){
    printf("In main: creating thread %ld\n", i);
    if (i == 0)
      rc = pthread_create(&threads[i], &pt1, work, (void *)i);

    if (i == 1)
      rc = pthread_create(&threads[i], &pt2, work, (void *)i);

    if (i == 2)
      rc = pthread_create(&threads[i], &pt3, work, (void *)i);

    if (i == 3)
      rc = pthread_create(&threads[i], &pt4, work, (void *)i);

    // else
    // rc = pthread_create(&threads[i], NULL, work, (void *)i);

    //s = pthread_setaffinity_np(threads[i], sizeof(cpu_set_t), &cpuset);    
    //    CPU_SET(threads[i], &cpuset);
    //    s = pthread_getaffinity_np(threads[i], sizeof(cpu_set_t), &cpuset);    



    if (rc){
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
    }
  }

  // printf("In main: MAIN TRABALHANDO\n");
  //work((void*)11);

  /*
  for (i = 0; i < num_thread; i++){
    pthread_join (threads[i], NULL);
  }
  */

  pthread_join (threads[0], NULL);
  pthread_join (threads[1], NULL);
  pthread_join (threads[2], NULL);
  pthread_join (threads[3], NULL);
  pthread_join (threads[4], NULL);

  pthread_attr_destroy(&pt1);
  pthread_attr_destroy(&pt2);
  pthread_attr_destroy(&pt3);
  pthread_attr_destroy(&pt4);

  pthread_exit(NULL);
  return 0;

}

