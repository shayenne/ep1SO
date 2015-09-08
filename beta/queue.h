/* 
 * Criar a fila e manter consistente o ponteiro para ela.
 * Quando fizer queuePut, atualizar o ponteiro da fila.
 * Finalizar corretamente todas as filas.
 */

/*
 * queue.h 
 * INTERFACE: funcoes para manipular a fila 
 */
 
 

#ifndef _QUEUE_H
#define _QUEUE_H

#include "item.h"

typedef struct queueNode* Link;
struct queueNode { 
  Item conteudo; 
  Link prox; 
};

Link queueInit(int);
 int queueEmpty(Link);
Link queuePut(Item, Link);
Item queueGet(Link);
void queueFree(Link);

#endif /* _QUEUE_H */
