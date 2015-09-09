#include <stdio.h>
#include "work.h"

void *work() {
    long i;

	printf("Entrei na work!\n");
    for (i = 0; i < 1E8; i++);

}
