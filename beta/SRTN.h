#include <stdio.h>  /* fprintf(), fscanf() */
#include <stdlib.h> /* EXIT_FAILURE, EXIT_SUCESS, malloc(), free() */
#include <string.h> /* strcmp() */
#include "queue.h"  /* queueInit(), queueEmpty(), queuePut(), 
                       queueGet(), queueFree() */
#include "item.h"
#include "util.h"   /* mallocSafe(), pause(), Bool */


void escalonadorSRTF(Link trace, FILE * saida);
