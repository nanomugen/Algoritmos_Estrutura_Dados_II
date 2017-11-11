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


//Funcao que compara os valores dos pesos das arestas
int comparaCusto(const void* a, const void* b)
{
    int custo1 = ((const struct Aresta*)a)->custo;
    int custo2 = ((const struct Aresta*)b)->custo;

    if(custo1 < custo2) return -1;
    if(custo1 > custo2) return 1;
}

/*  Funcao Arvore Geradora Minima
    A funcao primeiro ordena as arestas em ordem crescente de peso e depois gera
    a arvore com o menor custo
*/
void arvoreGeradoraMinima(grafo*g){
	int i, j, custo=0;
	subset subconjuntos[g->V];

    for (i=0; i<g->V; i++) {

        subconjuntos[i].pai = i;
        subconjuntos[i].rank = 0;
    }

    qsort(g->VetorDeArestas, g->E, sizeof(g->VetorDeArestas[0]), comparaCusto);

    for(j=0; j<g->E; j++) {

        int rx = Find_Set(subconjuntos, g->VetorDeArestas[j].origem);
        int ry = Find_Set(subconjuntos, g->VetorDeArestas[j].destino);

        if(rx != ry){
            custo = custo + g->VetorDeArestas[j].custo;
            Union(subconjuntos, rx, ry);
        }
    }
    printf("Custo total: %d\n", custo);
}


int main()
{
    int nV, nE, k;
    scanf("%d", &nV);
    scanf("%d", &nE);

    grafo* g = criarGrafo(nV, nE);

    for(k=0; k<g->E; k++) {
        int p, q, c;
        scanf("%d", &p);
        scanf("%d", &q);
        scanf("%d", &c);
        g->VetorDeArestas[k].origem = p;
        g->VetorDeArestas[k].destino = q;
        g->VetorDeArestas[k].custo = c;

    }
    /*
    if (temCiclo(g))
        printf("\nGrafo contem ciclo\n");
    else
        printf("\nGrafo nao contem ciclo\n");
    */
    arvoreGeradoraMinima(g);

}
