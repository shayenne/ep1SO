/* 
 * MAC0122 Princios de Desenvolvimento de Algoritmos
 */

#ifndef _ITEM_H
#define _ITEM_H

#define MAXNOME 20
/* item.h */
typedef struct processo {
	double t0;
	char nome[MAXNOME];
	double dt;
	double deadline;
	int p;
	double rtime;
} *Processo;

typedef Processo Item;


#endif  /* _ITEM_H */
 
