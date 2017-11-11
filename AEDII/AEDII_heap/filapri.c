#include <stdio.h>
#include <stdlib.h>

// imprime A[1..n], precedido da mensagem msg
void imprimeVetor (long int *A, long int n) 
{
    int i;    
    printf("[");
	for (i=1; i<n; i++)
	{
		printf("%ld, ", A[i]);
	}
	printf("%ld]\n", A[n]);
}

void MaxHeapify (long int *A, long int m, long int i)
{
	int e = 2*i, d = 2*i+1,maior;
	if(e<=m){
		if(*(A+e)>*(A+i))
			maior=e;
		else
			maior=i;
		if(d<=m && *(A+d)>*(A+maior))
			maior=d;
		if(maior!=i){
			long int aux = *(A+i);
			*(A+i) = *(A+maior);
			*(A+maior)=aux;
			MaxHeapify(A,m,maior);
		}
		
	}
}


// Aumenta a prioridade de uma chave.
void HeapIncreaseKey(long int *A, long int i, long int key){
	long int ex;
	if(key<*(A+i))
		printf("Erro.\n");
	else{
		*(A+i)=key;
		while(i>1 && *(A+i/2)<*(A+i)){
			ex=*(A+i);
			*(A+i)=*(A+i/2);
			*(A+i/2)=ex;
			i=i/2;
			
		}
	}
}

// Insere um novo elemento
void MaxHeapInsert(long int *A, long int *n, long int key)
{
	*n=*n+1;
	*(A+*n)=-99999;
	HeapIncreaseKey(A,*n,key);
}

// Retorna o maior elemento, removendo.
long int HeapExtractMax(long int *A, long int *n){
	if(*n<1){
		printf("Erro.\n");
		return 0;
	}
	else{
	
		long int max = *(A+1);
		*(A+1)=*(A+*n);
		*n=*n-1;
		MaxHeapify(A,*n,1);
		return max;
	}
}



int main () 
{
	int i;
	
	long int n, m;
	long int *A;
	long int key;
	
	scanf("%ld", &n);
	A = (long int*) malloc((n + 1) * sizeof(long int));

	m = n;
	n = 0;
	for (i=1; i<=m; i++)
	{
		scanf("%ld", &key);
		MaxHeapInsert(A, &n, key);
	}

	printf("vetor lido com %ld chaves: ", n);
	imprimeVetor(A, n);

    printf("ordem decrescente: [");
	while (n > 1)
	{
		printf("%ld, ", HeapExtractMax(A, &n));
	}
	printf("%ld]\n", HeapExtractMax(A, &n));

	free(A);

	return 0;
}
