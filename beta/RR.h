#define _GNU_SOURCE
#include <stdio.h>  /* fprintf(), fscanf() */
#include <stdlib.h> /* EXIT_FAILURE, EXIT_SUCESS, malloc(), free() */
#include <string.h> /* strcmp() */
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include "queue.h"  /* queueInit(), queueEmpty(), queuePut(), 
                       queueGet(), queueFree() */
#include "item.h"
#include "util.h"   /* mallocSafe(), pause(), Bool */
#include "work.h"




void escalonadorRR(Link trace, FILE * saida);
