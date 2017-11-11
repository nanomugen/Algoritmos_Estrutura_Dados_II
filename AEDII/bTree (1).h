typedef struct btNo *bTree;

bTree btCriar(void);
void  btDestruir(bTree t);
int   btBuscar(bTree t, int chave);
void  btInserir(bTree t, int key);
int   btAltura(bTree t);
int   btContaNos(bTree t);
void  btImprimirChaves(bTree b);
void  btImprimirArvore(bTree b);
void  removeChave(bTree b, int chave);
void  removeFolha(bTree b, bTree pai, int posicao);
void  removeNaoFolha(bTree pai, int pos);
int   irmaoEsquerda(bTree b);
int   irmaoDireita(bTree b);
void  emprestarDireita(bTree pai, int posicao);
void  emprestarEsquerda(bTree pai, int posicao);
void  unirNoh(bTree pai, int posicao);
//void  unirNohPai(bTree b, bTree irmao, bTree pai, int posicao);


