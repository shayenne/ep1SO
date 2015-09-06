#include<stdio.h>

struct node {
      Processo *proc; 
      struct node *prox;
   };
   
typedef struct node Node;

// Aloca dinamicamente um Node e devolve o endereço
// da cabeça de uma lista ligada.
Node * mallocLista() {
	Node *ini;
	ini = malloc (sizeof (celula));
	ini->prox = NULL;
	return ini;
}

// Imprime o conteúdo de uma lista encadeada
// com cabeça. O endereço do primeiro Node é ini.
void imprima (Node *ini) {
   Node *p;
   for (p = ini->prox; p != NULL; p = p->prox) 
      printf ("%d\n", p->proc);//Ajustar para imprimir o processo
}

// Esta função recebe um inteiro x e uma lista
// encadeada ini de inteiros, com celula-cabeça.
// A função devolve o endereço de uma celula que
// contém x ou devolve NULL se tal celula não
// existe.
Node *busca (int x, Node *ini)// Ajustar função
{
   Node *p;
   p = ini->prox;
   while (p != NULL && p->conteudo != x) 
      p = p->prox; 
   return p; 
}

// Esta função insere uma nova celula em uma 
// lista encadeada. A nova celula tem conteudo
// x e é inserida entre a celula apontada por 
// p e a seguinte. Supõe-se que p != NULL.

void 
insere (int x, celula *p)
{
   celula *nova;
   nova = mallocc (sizeof (celula));
   nova->conteudo = x;
   nova->prox = p->prox;
   p->prox = nova;
}

// Esta função recebe o endereço p de uma celula 
// de uma lista encadeada e remove da lista 
// a celula p->prox. A função supõe que
// p != NULL e p->prox != NULL.

void remove (celula *p)
{
   celula *morta;
   morta = p->prox;
   p->prox = morta->prox;
   free (morta);
}

// Esta função recebe uma lista encadeada ini,
// com cabeça, e remove da lista a primeira
// celula que contiver y, se tal celula existir.

void 
buscaERemove (int y, celula *ini)
{
   celula *p, *q;
   p = ini;
   q = ini->prox;
   while (q != NULL && q->conteudo != y) {
      p = q;
      q = q->prox;
   }
   if (q != NULL) {
      p->prox = q->prox;
      free (q);
   }
}

// Esta função recebe uma lista encadeada ini,
// com cabeça, e insere na lista uma nova celula
// imediatamente antes da primeira que contém y.
// Se nenhuma celula contém y, insere a nova
// celula no fim da lista. O conteudo da nova
// celula é x.

void 
buscaEInsere (int x, int y, celula *ini)
{
   celula *p, *q, *nova;
   nova = mallocc (sizeof (celula));
   nova->conteudo = x;
   p = ini;
   q = ini->prox;
   while (q != NULL && q->conteudo != y) {
      p = q;
      q = q->prox;
   }
   nova->prox = q;
   p->prox = nova;
}
