/* 
 * MAC0122 Principios de Desenvolvimento de Algoritmos
 */

/*
 * queue.h 
 * INTERFACE: funcoes para manipular a fila 
 */

#ifndef _QUEUE_H
#define _QUEUE_H

#include "item.h"


void queueInit(int);
 int queueEmpty();
void queuePut(Item);
Item queueGet();
void queueFree();

#endif /* _QUEUE_H */
