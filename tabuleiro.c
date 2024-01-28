#include "tabuleiro.h"
#include "stack.h"

/**************************
 * ficheiro_entrada()
 *
 * Argumentos: titulo - nome do ficheiro de entrada a ser lido
 *
 * Returns: (File) input_entrada - Ficheiro de entrada ja aberto em modo de leitura
 *
 * Description: Abre um ficheiro de leitura e verifica se tem a extensão correta, 
 * caso não tenha a extensão correta o programa vai dar exit.
 * 
 *************************/
FILE*ficheiro_entrada(char*titulo){ 
    FILE*input_entrada;
    const char* entrada=".tilewalls";
    input_entrada = fopen(titulo, "r");

    if(strlen(titulo)<strlen(entrada)||strcmp(&titulo[strlen(titulo)-strlen(entrada)],entrada)!=0){
        printf("Error: Invalid file extension for %s.Expected .tilewalls\n",titulo);
        exit (EXIT_FAILURE);
    }

    if (input_entrada == NULL) {
        printf("Error: Unable to open file.\n");
        exit (EXIT_FAILURE);
    }
    
    if(strlen(titulo)<12){
        exit (EXIT_FAILURE);
    }
    return input_entrada;
}

/**************************
 * data()
 *
 * Argumentos: input - ficheiro de entrada
 *             linha, coluna - dimensao da matriz
 *
 * Returns: Devolve um array que corresponde a matriz em 1 dimensao
 * 
 * Side-Effects: 
 *
 * Description: lê a informação do input e aloca essa informação dinamicamente num array 
 * tendo em conta a dimensão da matriz
 * 
 *************************/
int* data(FILE* input, int linha,int coluna){ 
    int *vector = malloc(linha * coluna * sizeof(int));
    for (int j = 0; j < linha; j++) {
        for (int i = 0; i < coluna ; i++) {
            if(fscanf(input, "%d", &vector[i + (linha - 1 - j) * coluna])!=1){ 
                exit (EXIT_FAILURE);
            }
            }
        }
    return vector;
}

/**************************
 * ficheiro_out()
 *
 * Argumentos: titulo - nome do ficheiro de entrada a ser lido
 *
 * Returns: (File) input_saida - Ficheiro de saida já aberto em modo de escrita
 * 
 * Description: Cria um ficheiro de saida, abrindo-o em modo de escrita
 *
 *************************/

FILE*ficheiro_out(char*titulo){
    FILE*input_saida;
    const char* entrada=".tilewalls";
    const char* saida=".tileblasts";

    strcpy(&titulo[strlen(titulo)-strlen(entrada)],saida);
    input_saida = fopen(titulo, "w");
    if (input_saida == NULL) {
        printf("Error: Unable to open file.\n");
        exit (EXIT_FAILURE);
    }

    if(strlen(titulo)<12){
        exit (EXIT_FAILURE);
    }

    return input_saida;
}

/**************************
 * Copia matriz()
 *
 * Argumentos: coluna, linha - dimensão da matriz
 *             original - matriz original
 *             copia 
 *
 * Returns: (void)
 * 
 * Efeitos: faz uma cópia da matriz
 *
 * Descricao: copia a matriz "original" para uma matriz "copia" guardando todos os parametros importantes
 *
 *************************/
void CopiaMatriz(int coluna, int linha, int* original, int*copia){
    for (int i = 0; i < linha*coluna; i++){
        copia[i] = original[i];
    }
}

/*****************************************************************************
 * initTab ()
 *
 * Argumentos:capacity - o numero de filhos de um pai no pior dos casos (linha*coluna/2)
 *            linha, coluna - dimensoes do tabuleiro 
 *
 * Returns: inicial - Estrutura do tabuleiro alocada
 *
 * Descricao: Aloca memoria para um Tabuleiro      
 *            Aloca memoria para a matriz atual (Tabuleiro->id)
 *            Aloca memoria para um vetor da posicao dos filhos do pai
 *            Inicializa o numero de filhos do pai
 *            Inicializa os pontos 
 *
 ****************************************************************************/
Tabuleiro* initTab(int capacity, int coluna, int linha)
{
    /* allocate initial game board structure */
    Tabuleiro* inicial = (Tabuleiro*) malloc(sizeof(Tabuleiro));
    if(inicial == NULL)
    {
        perror("Failed to allocate memory for the boards!\n");
        exit(EXIT_FAILURE);
    }

    /* now allocate the actual board, just an L x C matrix */
    inicial->id = (int*)malloc(linha*coluna*sizeof(int));
    if(inicial->id==NULL)
    {
    perror("Failed to allocate/copy memory for the boards!\n");
    exit(EXIT_FAILURE);
    }
    else{
        for(int i=0;i<linha*coluna;i++){
            inicial->id[i]=-1;
        }
    }

    inicial->posicao=-1;
    inicial->num_filhos=0;
    inicial->pontos=0;
    inicial->posicao_filhos=(int*)malloc(capacity * sizeof(int));

    if(inicial->posicao_filhos==NULL){
        perror("Failed to allocate memory for array posicao_filhos");
        exit(EXIT_FAILURE);}

    else{
        for(int j=0;j<capacity;j++){
            inicial->posicao_filhos[j]=-1;
        }
    }

  return inicial;
}

/******************************************************************************
 * deleteTabuleiro ()
 *
 * Argumentos: tabuleiro - Estrutura do tabuleiro com parametros para apagar
 *
 * Returns: (void)
 * Efeitos: Dar free ao Tabuleiro
 *
 * Descricao: Da free da matriz (id) e dos restantes campos alocados dinamicamente da estrutura de input
 *
 *****************************************************************************/
void deleteTabuleiro (Tabuleiro* tabuleiro)
{
    free(tabuleiro->id);
    free(tabuleiro->posicao_filhos);
    free(tabuleiro);
}

/**************************
 * MaxPontos ()
 *
 * Argumentos: stack -stack onde estao guardados os tabuleiros de todas a jogadas ate ao momento
 *             caminho - array que guarda as posições da mancha rebentada
 *
 * Returns: (void)
 *
 * Descricao: Atualiza o caminho com as posições que correspondem à sequencia de explosao
 * de manchas que da a pontuação maxima encontrada até aquele momento
 * 
 *************************/
void MaxPontos(Stack* stack, int* caminho)
{
    Tabuleiro*aux=(Tabuleiro*)peek(stack);
    if ( aux->pontos > caminho[0])
    {
        caminho[0] = aux->pontos;
        int j = 1;

        while (j<=stack->top)
        {   
            Tabuleiro*temp=(Tabuleiro*)stack->array[j];
            caminho[j] = temp->posicao;
            j++;
        }

        for(;j<stack->capacity;j++){
            caminho[j]=-1;
        }
    }
}

