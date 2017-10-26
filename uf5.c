
#include <stdio.h>
#include <stdlib.h>

struct Aresta {
    int origem, destino, custo;
};
typedef struct Aresta aresta;
 
struct Grafo {
    int V; // Numero de vertices
    int E; // Numero de arestas
    
    // O grafo eh representado por um vetor de arestas
    aresta* VetorDeArestas; 
};
typedef struct Grafo grafo;

struct Subset {
    int pai;
    int rank;
};
typedef struct Subset subset;


grafo* criarGrafo(int V, int E) {
    grafo* g = (grafo*) malloc( sizeof(grafo) );
    g->V = V;
    g->E = E;
    
    g->VetorDeArestas = (aresta*) malloc(g->E * sizeof(aresta) );
 
    return g;
}
 
// Funcao que procura o representante do elemento i
int Find_Set(subset subconjuntos[], int i) {
    if (subconjuntos[i].pai != i)
        subconjuntos[i].pai = Find_Set(subconjuntos, subconjuntos[i].pai);
    return subconjuntos[i].pai;
}
 
// Funcao que junta os conjuntos de x e y.
void Union(subset subconjuntos[], int x, int y) {
    int rx = Find_Set(subconjuntos, x);
    int ry = Find_Set(subconjuntos, y);
    
    if (subconjuntos[rx].rank < subconjuntos[ry].rank)
        subconjuntos[rx].pai = ry;
    else if (subconjuntos[rx].rank > subconjuntos[ry].rank)
        subconjuntos[ry].pai = rx;
    else {
        subconjuntos[ry].pai = rx;
        subconjuntos[rx].rank++;
    }
}
/*
void quicksort (grafo* g, int p, int r){
   int j;                         
   if (p < r) {                   
      j = separa (g, p, r);       
      quicksort (g, p, j-1);      
      quicksort (g, j+1, r);      
   }
}
int separa (grafo* g, int p, int r){
   int i = p+1, j = r;
   aresta* c;
   *c = g->VetorDeArestas[p]; 
   aresta* t;
   while (i <= j) {
      if (g->VetorDeArestas[i].custo <= c->custo) ++i;
      else if (c->custo < g->VetorDeArestas[j].custo) --j; 
      else {
         *t = g->VetorDeArestas[i];
         g->VetorDeArestas[i] = g->VetorDeArestas[j];
         g->VetorDeArestas[j] = *t;
         ++i;
         --j;
      }
   }
   // agora i == j+1                 
   g->VetorDeArestas[p] = g->VetorDeArestas[j];
   g->VetorDeArestas[j] = *c;
   return j; 
}
*/

//função necessária para o qsort
int menorcusto(const void* a, const void* b){
    int custoA = ((const struct Aresta*)a)->custo;
    int custoB = ((const struct Aresta*)b)->custo;

    if(custoA < custoB) return -1;
    if(custoA < custoB) return 0;
    if(custoA > custoB) return 1;
}

//função adaptada para encontrar o custo da menor árvore geradora
int conexo( grafo* g ) {
    int i, j,k,t=0,custo=0;
    subset subconjuntos[g->V];
    //int r[g->V];
    
    for (i=0; i<g->V; i++) {
        subconjuntos[i].pai = i;
        subconjuntos[i].rank = 0;
        //r[i]=0;
    }
    qsort(g->VetorDeArestas, g->E, sizeof(aresta), menorcusto);
    for(j=0; j<g->E; j++) {
        int rx = Find_Set(subconjuntos, g->VetorDeArestas[j].origem);
        int ry = Find_Set(subconjuntos, g->VetorDeArestas[j].destino);
            
        if (rx!=ry){
            custo = custo + g->VetorDeArestas[j].custo;
            Union(subconjuntos, rx, ry);
        }
        
    }
    return custo;
}
 

int main()
{
    int nV, nE, k, t;
    scanf("%d", &nV);
    scanf("%d", &nE);
    
    grafo* g = criarGrafo(nV, nE);
 
    for(k=0; k<g->E; k++) {
        int p, q,r;
        scanf("%d", &p);
        scanf("%d", &q);
        scanf("%d", &r);
        g->VetorDeArestas[k].origem = p;
        g->VetorDeArestas[k].destino = q;  
        g->VetorDeArestas[k].custo = r;  
          
    }

    t = conexo(g);
        printf("%d\n",t);
}
