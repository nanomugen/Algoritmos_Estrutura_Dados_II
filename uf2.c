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
int temCiclo( grafo* g ) {
    int i, j;
    subset subconjuntos[g->V];
 
    for (i=0; i<g->V; i++) {
        subconjuntos[i].pai = i;
        subconjuntos[i].rank = 0;
    }

    for(j=0; j<g->E; j++) {
        int rx = Find_Set(subconjuntos, g->VetorDeArestas[j].origem);
        int ry = Find_Set(subconjuntos, g->VetorDeArestas[j].destino);
            
        if (rx==ry)
            return 1;
            
        Union(subconjuntos, rx, ry);
    }
    return 0;
}
 

int main()
{
    int nV, nE, k;
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

    if (temCiclo(g))
        printf("\nGrafo contem ciclo\n");
    else
        printf("\nGrafo nao contem ciclo\n");
}