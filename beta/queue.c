/* queue.c: IMPLEMENTACAO DA FILA */

/* Interface para esta implementacao */
#include "queue.h"

/* malloc() e free() precisam stdlib */
#include <stdlib.h> /* free() */

/* fprintf precisa stdio.h */
#include <stdio.h>  /* fprintf() */

/* tipo do objeto na fila */
#include "item.h" /* Item */

/* biblioteca de macros e funcoes de uso geral */
#include "util.h" /* mallocSafe() */

/*
 * FILA: uma implementacao com lista encadeada circular com 
 *       cabeca de lista
 */

Link 
queueInit(int n) 
{ 
  Link inicio = (Link) mallocSafe(sizeof *inicio);
  inicio->prox = inicio;
  inicio->conteudo = NULL;
  return inicio;
}

int 
queueEmpty(Link inicio)
{ 
  return inicio == inicio->prox; 
}

Link
queuePut(Item conteudo, Link inicio)
{ 
  Link nova = (Link) mallocSafe(sizeof *nova);
  nova->prox = inicio->prox;
  inicio->prox = nova;
  inicio->conteudo = conteudo;
  inicio = nova; 
  return inicio;
}

Item 
queueGet(Link inicio)
{ 
  Item conteudo;
  Link t = inicio->prox; 
  conteudo = t->conteudo;
  inicio->prox = t->prox;  
  free(t); 
  return conteudo;
}

void 
queueFree(Link inicio)
{ 
  Link p = inicio->prox;
  while (p != inicio) 
    { 
      Link t = p->prox;
      free(p);
      p = t;
    }
  free(inicio);
}
