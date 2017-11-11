// Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "bTree.h"

// Define a quantidade total de filhos no:
#define ORDEM (3)

struct btNo {
    int ehFolha;                  // 0 se eh uma folha, outro valor se for uma folha.
    int numChaves;                // Quantidade total de chaves no no
    int chaves[ORDEM];            // Vetor com chaves (o ultimo elemento eh usado como auxiliar)
    struct btNo *filhos[ORDEM+1]; // Vetor com ponteiros para os filhos (o ultimo elemento eh usado como auxiliar)
};

bTree btCriar(void) {
    bTree b;
    b = malloc(sizeof(*b));
    assert(b);                   // Confere se a memoria foi corretamente alocada
// 	memset(b, 0x00, sizeof(*b)); // Atribui 0 para todos os campos
    b->ehFolha = 1;              // A raiz, inicialmente, eh uma folha!
    b->numChaves = 0;
    return b;
}

void btDestruir(bTree b) {
    int i;
    if(!b->ehFolha) { // Destruir todos os filhos primeiro, caso existir...
        for(i = 0; i < b->numChaves + 1; i++) {
            btDestruir(b->filhos[i]);
        }
    }
    free(b);
}

// Realiza uma busca binaria em um vetor e retorna o indice da chave
int buscarChave(int n, const int *a, int chave)
{
    int lo;
    int hi;
    int mid;

    lo = -1;
    hi = n;

    while(lo + 1 < hi) {
        mid = (lo+hi)/2;
        if(a[mid] == chave) {
            return mid;
        } else if(a[mid] < chave) {
            lo = mid;
        } else {
            hi = mid;
        }
    }

    return hi;
}

int btBuscar(bTree b, int chave)
{
    int pos;
    if(b->numChaves == 0) { // Caso nao ter chaves, retornar...
        return 0;
    }

    pos = buscarChave(b->numChaves, b->chaves, chave); // Encontrar o indice da chave

    if(pos < b->numChaves && b->chaves[pos] == chave) {  // Chave encontrada!!!
        return 1;
    } else {
        return(!b->ehFolha && btBuscar(b->filhos[pos], chave)); // Chave nao encontrada, tentar no noh filho, se existir...
    }
}

bTree inserirInterno(bTree b, int chave, int *mediana);
void btInserir(bTree b, int chave)
{
    bTree b1;    // Novo no esquerda
    bTree b2;    // Novo no direita
    int mediana; // Elemento do meio (que subira)

    b2 = inserirInterno(b, chave, &mediana);

    if(b2) { // A raiz precisa se dividir: eh preciso criar uma nova raiz!
        b1 = malloc(sizeof(*b1));
        assert(b1);
        memmove(b1, b, sizeof(*b)); // Copia lista
        b->numChaves = 1;
        b->ehFolha = 0;
        b->chaves[0] = mediana;
        b->filhos[0] = b1;
        b->filhos[1] = b2;
    }
}
bTree inserirInterno(bTree b, int chave, int *mediana)
{
    int pos;
    int mid;
    bTree b2;

    pos = buscarChave(b->numChaves, b->chaves, chave); // Obter posicao da chave no no atual

    if(pos < b->numChaves && b->chaves[pos] == chave) { // A chave ja existe, abortar!
        return 0;
    }

    if(b->ehFolha) { // Encontrou a folha para inserir. Inserir na posicao correta!
        memmove(&b->chaves[pos+1], &b->chaves[pos], sizeof(*(b->chaves)) * (b->numChaves - pos)); // Todas as chaves acima de pos move um espaco acima
        b->chaves[pos] = chave;
        b->numChaves++;
    } else {
        b2 = inserirInterno(b->filhos[pos], chave, &mid); // Inserir chave em um filho...
        if(b2) { // A folha filha foi dividida, inserir a chave em b...
            memmove(&b->chaves[pos+1], &b->chaves[pos]  , sizeof(*(b->chaves)) * (b->numChaves - pos)); // Todos as chaves acima de pos move um espaco acima
            memmove(&b->filhos[pos+2], &b->filhos[pos+1], sizeof(*(b->chaves)) * (b->numChaves - pos)); // Todos os filhos acima de pos move um espaco acima
            b->chaves[pos]   = mid;
            b->filhos[pos+1] = b2;
            b->numChaves++;
        }
    }

    if(b->numChaves >= ORDEM) { // Verificar se eh preciso dividir b
        mid = b->numChaves/2; // Obter indice da chave mediana
        *mediana = b->chaves[mid]; // Copiar valor da chave mediana para ser utilizado na funcao que chamou
        b2 = malloc(sizeof(*b2)); // Criar nova folha!
        b2->numChaves = b->numChaves - mid - 1;
        b2->ehFolha = b->ehFolha;

        memmove(b2->chaves, &b->chaves[mid+1], sizeof(*(b->chaves)) * b2->numChaves); // Copiar a metade dos elementos de b para b2
        if(!b->ehFolha) { // Se nao for folha, copiar os filhos tambem!
            memmove(b2->filhos, &b->filhos[mid+1], sizeof(*(b->filhos)) * (b2->numChaves + 1));
        }
        b->numChaves = mid; // Agora b tem metade dos elementos
        return b2;
    } else { // Nada precisa ser feito...
        return 0;
    }
}

int btAltura(bTree b) {
	int n = 1;

	if(b->ehFolha){
		return 1;
	}
	else{
		n = n + btAltura(b->filhos[0]);
		return n;
	}
}

int btContaNos(bTree b) {
	int i, n = 1;

	if (b->ehFolha){
		return 1;
	}

	else{
		for (i = 0; i <= b->numChaves; i++){
			n = n + btContaNos(b->filhos[i]);
		}
		return n;
	}
}

void btImprimirChaves(bTree b) {
	int i;

	if (b->ehFolha){
		for (i = 0; i < b->numChaves; i++){
			printf("%d ", b->chaves[i]);
		}
	}
	else{
		for (i = 0; i <= b->numChaves; i++){
			btImprimirChaves(b->filhos[i]);

			if(i < b->numChaves){
				printf("%d ", b->chaves[i]);
			}
		}
	}
	//printf("\n");
}



void btImprimirArvore(bTree b) {
	int totNos;
	bTree *fila;
	int *nivel;
	int p = 0; // Posicao atual
	int n = 0; // Numero de elementos

	int nivel_atual = -1;

	totNos = btContaNos(b);
	fila   = (bTree*) malloc(sizeof(*fila)  * totNos);
	nivel  = (int*)   malloc(sizeof(*nivel) * totNos);
	nivel[n] = 0;
	fila [n] = b;
	n++;

	while(p < n) {
		int i;
		int prox_nivel = nivel[p];
		bTree t = fila[p++];

		for(i = 0; i < t->numChaves+1 && !(t->ehFolha); i++) {
			nivel[n]  = prox_nivel + 1;
			fila[n++] = t->filhos[i];
		}

		if(nivel_atual != prox_nivel) {
			if(prox_nivel != 0)
				printf("\n");
			printf("%i: ", prox_nivel+1);
			nivel_atual = prox_nivel;
		}

		printf("[");
		for(i = 0; i < t->numChaves; i++) {
			printf("%i", t->chaves[i]);
			if(i != t->numChaves-1)
				printf(",");
		}
		printf("] ");
	}

	printf("\n");

	free(fila);
	free(nivel);
}

void removeChave(bTree b, int chave){
	int pos, pos_pai;
	int key = chave;
	bTree pai;
	pai = malloc(sizeof(*pai));
	pos = buscarChave(b->numChaves, b->chaves, key);

	if(btBuscar(b, key)){
		while (b->chaves[pos] != key){
			memmove(pai, b, sizeof(*b));
			b = b->filhos[pos];
			pos_pai = pos;
			pos = buscarChave(b->numChaves, b->chaves, key);
		}
		if(b->ehFolha){
			removeFolha(pai, pos, pos_pai);
		}
		else{
			removeNaoFolha(b, pos);
		}
	}

}


//rotina para remover uma chave de uma folha
void removeFolha(bTree pai, int posicao, int posicao2){
	//int div;
	int pos = posicao; //posicao no vetor onde a chave esta localizada
	int pos2 = posicao2; //indice do ponteiro para o filho no qual a posicao da chave esta localizada

	if (checaNumChaves(pai_>filhos[pos2])){
		memmove(&pai->filhos[pos2]->chaves[pos], &pai->filhos[pos2]->chaves[pos+1], sizeof(*(pai->filhos[pos2]->chaves)) * (pai->filhos[pos2]->numChaves - pos));
		pai->filhos[pos2]->numChaves--;
	}
	else {
		//memmove(&pai->filhos[pos2]->chaves[pos], &pai->filhos[pos2]->chaves[pos+1], sizeof(*(pai->filhos[pos2]->chaves)) * (pai->filhos[pos2]->numChaves - pos));
		//pai->filhos[pos2]->numChaves--;
		if(pai->filhos[pos2-1]){
			if(checaNumChaves(pai->filhos[pos2-1])){
				emprestarEsquerda(pai, pos, pos2);
			}
		}
		else if (pai->filhos[pos2+1]){
			if(checaNumChaves(pai->filhos[pos2+1])){
				emprestarDireita(pai, pos, pos2);
			}
		}
		else {
			unirNoh(pai, pos)
		}
	}
//checar se a arvore esta balanceada
	checar(pai);
}

void removeNaoFolha(bTree pai, int pos){
	//devemos buscar a chave com maior valor no filho a direita do noh
	int pos = pos;
	bTree temp;
	temp = malloc(sizeof(*temp));
	if (pai->filhos[0]->ehFolha){
		pai->chaves[pos] = pai->filhos[0]->chaves[(pai->filhos[0]->numChaves)-1];
		pai->filhos[0]->numChaves--;
	}
	else{
		temp = pai->filhos[0];
		while(!temp->filhos[temp->filhos->numChaves]->ehFolha){
			temp = temp->filhos[temp->filhos->numChaves];
		}
		pai->chaves[pos] = temp->filhos[temp->filhos->numChaves]->chaves[(temp->filhos->numChaves)-1];
	}

	checar(pai);


}

//Checa o numero de chaves de um noh, se ele tem o minimo de chaves e o maximo de chaves
int checaNumChaves(bTree b){
    int div;

    if (ORDEM % 2 != 0){
		div = (ORDEM/2)+1;
	}
	else{
		div = ORDEM/2;
	}
	if (b->numChaves > div -1 && b->numChaves < ORDEM){
	    return 1;
	}
	else{
	    return 0;
	}
}









//VERIFICAR -> ESTA UM POUCO CONFUSO
//empresta a chave do irmao a direita
void emprestarDireita(bTree pai, int posicao, int posicao2){
	int chave_pai = pai->chaves[posicao2]; //Recebe o valor da chave na posicao em que o pai esta indicando para o seu filho
	int chave_filho_direita = pai->filhos[posicao2+1]->chaves[0]; //Recebe o primeiro elemento do filho a direita
	int pos = pai->filhos[posicao2]->numChaves;
	int posi = posicao;
	int posi2 = posicao2;
	int pos2=0;
	//memmove(&pai->filhos[posi2]->chaves[posi], &pai->filhos[posi2]->chaves[posi+1], sizeof(*(pai->filhos[posi2]->chaves)) * (pai->filhos[posi2]->numChaves - posi));
	//memmove(&b->chaves[pos+1], &b->chaves[pos], sizeof(*(b->chaves)) * (b->numChaves - pos));
	pai->filhos[posi2]->chaves[pos-1] = chave_pai;
	pai->chaves[posi2] = chave_filho_direita;
	memmove(&pai->filhos[posi2+1]->chaves[pos2], &pai->filhos[posi2+1]->chaves[pos2+1], sizeof(*(pai->filhos[posi2+1]->chaves)) * (pai->filhos[posi2+1]->numChaves - pos2));
	pai->filhos[posi2+1]->numChaves--;
	//pai->filhos[posi]->numChaves++;

}








//empresta a chave do irmao a esquerda
void emprestarEsquerda(bTree pai, int posicao, int posicao2){
	int num = pai->filhos[posicao2-1]->numChaves;
	int chave_pai = pai->chaves[posicao2];
	int chave_filho_esquerda = pai->filho[posicao2-1]->chaves[num-1];
	int pos = 0;
	int posi2 = posicao2;
	int posi = posicao;

	//memmove(&pai->filhos[posi]->chaves[pos+1], &pai->filhos[posi]->chaves[pos], sizeof(*(pai->filhos[posi]->chaves)) * (pai->filhos[posi]->numChaves - pos));
	pai->filhos[posi2]->chaves[pos] = chave_pai
	pai->chaves[posi2] = chave_filho_esquerda;
	memmove(&pai->filhos[posi2-1]->chaves[pai->filhos[posi2-1]->numChaves-1], &pai->filhos[posi2-1]->chaves[pai->filhos[posi2-1]->numChaves], sizeof(*(pai->filhos[posi2]->chaves)) * (pai->filhos[posi2]->numChaves - pai->filhos[posi2-1]->chaves[pai->filhos[posi2-1]->numChaves-1]));
	pai->filhos[posi2-1]->numChaves--;
	//pai->filhos[posi]->numChaves++;
}












//caso os irmaos nao tenham a quantidade suficiente de chaves, unir o elemento do pai com os dois filhos correspondentes
void unirNoh(bTree pai, int posicao, int posicao2){
	int chave_pai = pai->chaves[posicao2], i;
	int posi = posicao;
	int posi2 = posicao2;
	if (pai->filhos[posi2+1]){

		int pos = 0;
		//int posi = posicao;

		memmove(&pai->filhos[posicao+1]->chaves[pos+1], &pai->filhos[posicao+1]->chaves[pos], sizeof(*(pai->filhos[posicao+1]->chaves)) * (pai->filhos[posicao+1]->numChaves - pos));
		pai->filhos[posicao+1]->chaves[pos] = chave_pai;
		pai->filhos[posicao+1]->numChaves++;
		for(i = pai->filhos[posi]->numChaves; i > 0; i--){
			memmove(&pai->filhos[posicao+1]->chaves[pos+1], &pai->filhos[posicao+1]->chaves[pos], sizeof(*(pai->filhos[posicao+1]->chaves)) * (pai->filhos[posicao+1]->numChaves - pos));
			pai->filhos[posi+1]->chaves[pos] = pai->filhos[posi]->chaves[i];
			pai->filhos[posi]->numChaves--;
			pai->filhos[posi+1]->numChaves++;
		}
		memmove(&pai->chaves[posi], &pai->chaves[posi+1], sizeof(*(pai->chaves)) * (pai->numChaves - posi));
		pai->numChaves--;
		free(pai->filhos[posi]);
		}

	else{
	//irmao = pai->filhos[posi-1]
	int pos = 0;
	//int posi = posicao;
	memmove(&pai->filhos[posi]->chaves[pos+1], &pai->filhos[posi]->chaves[pos], sizeof(*(pai->filhos[posi]->chaves)) * (pai->filhos[posi]->numChaves - pos));
	pai->filhos[posi]->chaves[pos] = chave_pai;
	for(i = pai->filhos[posi-1]->numChaves; i > 0; i--){
		memmove(&pai->filhos[posi]->chaves[pos+1], &pai->filhos[posi]->chaves[pos], sizeof(*(pai->filhos[posi]->chaves)) * (pai->filhos[posi]->numChaves - pos));
		pai->filhos[posi-1]->chaves[pos] = pai->filhos[posi-1]->chaves[i];
		pai->filhos[posi-1]->numChaves++;
		pai->filhos[posi-1]->numChaves--;
	}
	memmove(&pai->chaves[posi], &pai->chaves[posi+1], sizeof(*(pai->chaves)) * (pai->numChaves - posi));
	pai->numChaves--;
	free(pai->filhos[posi-1]);
	}
}












