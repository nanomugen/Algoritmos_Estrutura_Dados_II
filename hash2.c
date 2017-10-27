#include <stdio.h>
#include <stdlib.h>

#define M 2039
// Tamanho da tabela.

//#define hash(v) (v % M)
// Transforma uma chave v em um índice no intervalo 0..M-1.

#define tipoChave long int
long int hash(long int chave){
    if(chave/1000000000>0)
        return (chave%100000)%M;
    else
        return ((chave/100)%100)%M;
}

struct tipoObj {
   tipoChave chave;
   char valor;
};
typedef struct tipoObj tipoObjeto;

tipoObjeto objetonulo;

// Definição de um nó das listas de colisões.
typedef struct node *link;
struct node {
   tipoObjeto obj; 
   link       next;
} ;
typedef struct node STnode;

// Tabela que aponta para as M listas de colisões.
link *tab;
// Inicializa uma tabela de símbolos que, espera-se, armazenará 
// cerca de 10000 objetos. A espinha dorsal da tabela será um 
// vetor tab[0..M-1].
//
void STinit() 
{ 
   int h;
   tab = malloc(M * sizeof (link));
   for (h = 0; h < M; h++) 
      tab[h] = NULL; 
}

// Insere obj na tabela de símbolos.
//
void STinsert(tipoObjeto obj) 
{ 
   int h;
   tipoChave v = obj.chave;
   h = hash(v);

//printf ("ra=%d hash=%d\n", obj.chave, h);

   link novo = malloc(sizeof (STnode));
   novo->obj = obj;
   novo->next = tab[h];
   tab[h] = novo;
}

// Devolve um objeto cuja chave é v. Se tal objeto não existe,
// a função devolve um objeto fictício com chave nula.
//
tipoObjeto STsearch(tipoChave v) 
{ 
   link t;
   int h;
   h = hash(v);
   for (t = tab[h]; t != NULL; t = t->next) 
      if (t->obj.chave == v) break;
   if (t != NULL) return t->obj;
   return objetonulo;
}

int existe(tipoChave chave)
{
   tipoObjeto obj = STsearch(chave);
   return obj.chave != objetonulo.chave;
}

/****************************************************************************
/* Completar estatisticas, para calcular e exibir:
/*  . quantidade de chaves na tabela hash
/*  . fator de carga da tabela hash
/*  . maior comprimento das listas de colisões
/*  . quantidade de compartimentos da tabela sem chave
/****************************************************************************/
void conta(int *n){
    int i,cont=0,cont2=0,cont3=0,maior=0;
    //int* acumulador= malloc (*int) 3*sizeof(int);
    for(i=0;i<M;i++){
        cont2=0;
        if(tab[i] != NULL){
            cont++;
            cont2++;
            link temp = tab[i];
            while(temp->next != NULL){
                cont++;
                cont2++;
                temp=temp->next;
                
            }
        if(cont2>maior)
            maior=cont2;
        }
        else
            cont3++;
    }
    *n= cont;
    *(n+1) = maior;
    *(n+2) = cont3;
    //return acumulador;
}

void estatisticas () 
{
    int *n = malloc(3*sizeof(int)); 
    conta(n);
	printf ("Quantidade de chaves = %d\n",*n);

	printf ("Fator de Carga = %f\n", *n/(float)M);

	printf ("Maior comprimento das listas de colisões: %d\n", *(n+1));

	printf ("Compartimentos sem chave: %d\n", *(n+2));

}

int main()
{
    tipoChave ra;
	tipoObjeto *obj;

	STinit();
	objetonulo.chave = 0; // Chaves "válidas" são estritamente positivas

	scanf("%ld", &ra);

	// le uma sequencia de RAs, encerrada por um negativo
	while (ra >0) {
		if (!existe(ra)) { // se chave nao existe na tabela, insere
			obj = malloc(sizeof (tipoObjeto));
			obj->chave = ra;
			STinsert(*obj);
		}
    	scanf("%ld", &ra);
	}
	
	estatisticas();	

}

