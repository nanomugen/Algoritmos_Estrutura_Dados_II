#include <stdio.h>
#include <stdlib.h>

struct Aresta {
    int origem, destino;
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
 
// Funcao utilizada para verificar se o grafo tem ou nao ciclo
int conexo( grafo* g ) {
    int i, j,k,t=0,m=0;
    subset subconjuntos[g->V];
    int r[g->V];
    
    for (i=0; i<g->V; i++) {
        subconjuntos[i].pai = i;
        subconjuntos[i].rank = 0;
        r[i]=0;
    }
    for(j=0; j<g->E; j++) {
        int rx = Find_Set(subconjuntos, g->VetorDeArestas[j].origem);
        int ry = Find_Set(subconjuntos, g->VetorDeArestas[j].destino);
            
        if (rx!=ry)
        Union(subconjuntos, rx, ry);
    }
    for(k=1;k<g->V;k++)
        r[subconjuntos[k].pai]++;
    for(k=0;k<g->V;k++)
        if(r[k]>0 && r[k]>m)
            m=r[k];
    
    return ++m;
}
 

int main()
{
    int nV, nE, k, t;
    scanf("%d", &nV);
    scanf("%d", &nE);
    
    grafo* g = criarGrafo(nV, nE);
 
    for(k=0; k<g->E; k++) {
        int p, q;
        scanf("%d", &p);
        scanf("%d", &q);
        g->VetorDeArestas[k].origem = p;
        g->VetorDeArestas[k].destino = q;    
    }

    t = conexo(g);
        printf("%d\n",t);
}
