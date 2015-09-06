/* 
 * MAC0122 Principios de Desenvolvimento de Algoritmos
 */


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


typedef struct queueNode* Link;
struct queueNode { 
  Item conteudo; 
  Link prox; 
};

static Link inicio;


void 
queueInit(int n) 
{ 
  inicio = (Link) mallocSafe(sizeof *inicio);
  inicio->prox = inicio;
}

int 
queueEmpty()
{ 
  return inicio == inicio->prox; 
}

void
queuePut(Item conteudo)
{ 
  Link nova = (Link) mallocSafe(sizeof *nova);
  nova->prox = inicio->prox;
  inicio->prox = nova;
  inicio->conteudo = conteudo;
  inicio = nova; 
}

Item 
queueGet()
{ 
  Item conteudo;
  Link t = inicio->prox; 
  conteudo = t->conteudo;
  inicio->prox = t->prox;  
  free(t); 
  return conteudo;
}

void 
queueFree()
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
