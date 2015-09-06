/* 
 * MAC0122 Principios de Desenvolvimento de Algoritmos 
 *
 * Aplicacao de uma pilha para determinar a notacao posfixa
 * de uma dada expressao em notacao infixa.
 *
 * distancias.c
 */

#include <stdio.h>  /* fprintf(), fscanf() */
#include <stdlib.h> /* EXIT_FAILURE, EXIT_SUCESS, malloc(), free() */
#include <string.h> /* strcmp() */
#include "queue.h"  /* queueInit(), queueEmpty(), queuePut(), 
                       queueGet(), queueFree() */

#include "util.h"   /* mallocSafe(), pause(), Bool */

/* 
 *  P A R T E   I.   P R O T O T I P O S
 */

int *
distancias (int n, int **a, int c);

int **
carregueRede(char nomeArquivo[], int *nCidades);

int **
mallocRede(int nCidades); /* questao prova 1 */

void
freeRede(int nLin, int **rede); /* questao prova 1 */

/*  
 * P A R T E  II.  M A I N  
 */

int 
main(int argc, char *argv[]) 
{  
  char *nomeArquivoEntrada = NULL; /* nome do arquivo de estradas */
  char *nomeArquivoSaida = NULL; /* nome do arquivo de saida    */
  
  int **rede = NULL; /* rede[i][j] == 1 se existe estrada de i a j */
  int nCidades; /* numero de cidades */
  int origem = 0; /* cidade origem, default e' 0 */
  int *dist = NULL; /* dist[i] = comprimento do menor caminho de c a i */
  int i;
  
  FILE *arqSaida; /* arquivo de saida */
  
  if (argc == 1) 
    {
      fprintf(stdout,"Uso: %s -c<cid_origem> -f <arq_estradas> [-s <arq-saida>]\n", 
	      argv[0]);
      return 0;
    }
  
  /* Examina os argumentos na linha de comando */
  for (i = 1; i < argc; i++) 
    {
      if (sscanf(argv[i],"-c%d",&origem)==1);
      else if (strncmp(argv[i],"-f",2)==0) nomeArquivoEntrada = argv[++i];
      else if (strncmp(argv[i],"-s",2)==0) nomeArquivoSaida = argv[++i];
      else 
	{
	  fprintf(stderr,
		  "Uso: %s -c<cid_origem> -f <arq_estradas> [-s <arq-saida>]\n",
		  argv[0]);
	  return EXIT_FAILURE;
	}
    }

  /* saida default e' a padrao */
  if (nomeArquivoSaida == NULL) arqSaida = stdout; 
  else 
      if((arqSaida = fopen(nomeArquivoSaida,"w")) == NULL)
      {
          fprintf(stderr,
                  "ERRO: arquivo de saida %s nao pode ser criado.\n", 
                  nomeArquivoSaida);
          return EXIT_FAILURE;
      }

  rede = carregueRede(nomeArquivoEntrada,&nCidades);

  dist = distancias(nCidades,rede,origem);

  /*------------------------------------------------------------*/
  /* escreva a resposta */
  fprintf(arqSaida, "\nDistancias a partir da cidade %d:\n", origem);

  for (i = 0; i < nCidades; i++) 
    fprintf(arqSaida, "%d: %d\n", i, dist[i]);

  fclose(arqSaida);

  /*------------------------------------------------------------*/
  /* libere toda a area alocada */

  free(dist);
  freeRede(nCidades,rede);

  return EXIT_SUCCESS;
}

/*  
 * P A R T E III.  I M P L E M E N T A C A O   D A S   F U N C O E S
 */

/*
 * distancia
 *
 * RECEBE uma rede de estradas entre N cidades representada pela
 * matriz REDE e o indice C de uma cidade.
 *
 * A funcao retorna um vetor d[0..N-1] tal que para i=0,1,...,N-1,
 * d[i] e a distancia entre a cidade C e a cidade i. 
 *
 */

int *
distancias (int n, int **a, int c)
{
  int j;
  int *d;
 
  /* aloca vetor de distancias */
  d = (int*) mallocSafe(n * sizeof(int));
 
  queueInit(n); /* inicializa a fila */

  /* inicializa o vetor de distancias */  
  for (j = 0; j < n; j++)  
    d[j] = n; /* distancia n = infinito */
  d[c] = 0;
  
  queuePut(c); /* coloca c na fila */
  
  /* queueempty(): a fila esta vazia */ 
  while (!queueEmpty()) 
    {
      int i, di;

      i = queueGet(); /* i recebe o 1o. elemento da fila */
  
      di = d[i];
      for (j = 0; j < n; j++)
	if (a[i][j] == 1 && d[j] > di+1) 
	  {
	    d[j] = di + 1;
            
            queuePut(j); /* coloca j na fila */
	  }
    }
  
  queueFree();
  
  return d;
}

/*
 * carregueRede
 *
 * RECEBE o nome de um arquivo NOMEARQUIVO que contem
 * a representacao de uma rede de cidades e um ponteiro
 * N.
 * 
 * A funcao cria e RETORNA uma matriz que representa uma rede de
 * estradas:
 *
 *    - a posicao [i][j] da matriz contem 1 se existe um
 *      estrada direta entre a cidade i e a cidade j
 *    - a posicao [i][j] da matriz contem 0 se nao 
 *      existe estrada entre a cidade i e a cidade j
 *
 * A funcao DELVOVE o numero de cidades na rede atraves do 
 * ponteiro N.
 *
 * Se o arquivo nao puder ser lido, a funcao encerra a 
 * execucao do  programa.
 *
 * A formato do arquivo e o seguinte: 
 *
 *     - primeira linha contem, nessa ordem, o numero de 
 *       cidades n
 * 
 *     - as proximas linha contem uma matriz binaria de 
 *       dimensao n X n que representa a rede de estradas:
 *       posicao [i][j] contem um 1 se existe uma estrada
 *       entre as cidades i e j e contem um 0 se nao 
 *       existe uma estrada entre as cidades i e j.
 *     
 */
 
int **
carregueRede(char nomeArquivo[], int *n) 
{
  FILE *arqEntrada = NULL; /* arquivo de estradas */
  int  **rede = NULL;
  int  nCidades;
  int i, j;
  
  if ( (arqEntrada = fopen(nomeArquivo,"r")) == NULL ) 
  {
      fprintf(stderr,"\nERRO: arquivo %s nao pode ser aberto.\n",
	      nomeArquivo);
      exit(EXIT_FAILURE);
  }

  /* le numero de cidades */
  fscanf(arqEntrada,"%d ", &nCidades);
    
  /* aloca matriz de estradas */
  if ( (rede = mallocRede(nCidades)) == NULL )
  {
      fprintf(stderr,"\nERRO: rede %d X %d nao pode ser criada.\n",
	      nCidades, nCidades);
      exit(EXIT_FAILURE);
  }

 
  /* leitura das estradas */  
  for (i = 0; i < nCidades; i++) 
  {
    for (j = 0; j < nCidades; j++)
    {       
        if (fscanf(arqEntrada,"%d ", &rede[i][j]) != 1)  
	{
	  fprintf(stderr,"\nERRO: faltam dados no arquivo %s.\n",
		   nomeArquivo);
	  exit(EXIT_FAILURE);
	}
    }
  }

  fclose(arqEntrada);

  printf("Rede lida:\n");
  for (i = 0; i < nCidades; i++)
  {
      for (j = 0; j < nCidades; j++)
          printf("%d ", rede[i][j]);
      printf("\n");
  }

  /* valores a serem devolvidos e retornados */
  *n = nCidades;
  return rede;
}

/*-------------------------------------------------------------*/
int **
mallocRede(int nCidades)
{
    int **rede;
    int lin;
    
    /* aloca matriz de estradas */
    if ( (rede = mallocSafe(nCidades*sizeof(int*))) == NULL )
    { /* erro */
        return NULL;
    }      
    
    for (lin = 0; lin < nCidades; lin++)
    {
        if ( (rede[lin]=mallocSafe(nCidades*sizeof(int))) == NULL)
        { /* erro */
            while (--lin) free(rede[lin]);
            free(rede);
            return NULL;
        }
    }
    
    return rede;
}


/*-------------------------------------------------------------*/
void
freeRede(int nCidades, int **rede)
{
    int lin;

    for (lin = 0; lin < nCidades; lin++)
    {
        free(rede[lin]);
    }
    
    free(rede);
}
