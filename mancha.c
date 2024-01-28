#include "mancha.h"
#include "stack.h"
#include "tabuleiro.h"

/******************************************************************************
 * Mancha ()
 *
 * Argumentos: c, l - coordenadas iniciais de uma posicao da mancha a ser apagada,caso existir
 *            id - matriz
 *            tamanho - tamanho da mancha atualizado com o encontrar de novas posicoes da mancha 
 *            coluna, linha - dimensoes da matriz
 *
 * Returns: tamanho - tamanho final da mancha encontrada/ tamanho ate ao momento
 *
 * Descricao: Funcao recursiva que atraves de uma posicao (l,c) vai descobrindo uma mancha 
 * ao ver as posicoes adjacentes nos quatros sentidos (cima,baixo,direita,esquerda), devolvendo
 * o tamanho ate ao momento, que vai sendo incrementado ate chegar ao tamanho final da mancha escolhida
 * 
 *****************************************************************************/
int Mancha(int coluna, int linha, int c, int l, int* id, int tamanho, int c_min)
{
    int n = c + l * coluna; //transforma coordenadas de duas dimensoes so numa
    int aux = id[n];

    id[n] = -1;
    if (l + 1 < linha && id[n + coluna] != -1 && id[n + coluna] == aux) // cima
    {
        tamanho = Mancha(coluna, linha, c, l + 1, id, tamanho + 1,c_min);
    }
    if (c + 1 < coluna && id[n + 1] != -1 && id[n + 1] == aux) // direita
    {
        tamanho = Mancha(coluna, linha, c + 1, l, id, tamanho + 1,c_min);
    }
    if (l - 1 >= 0 && id[n - coluna] != -1 && id[n - coluna] == aux) // baixo
    {
        tamanho = Mancha(coluna, linha, c, l - 1, id, tamanho + 1,c_min);
    }
    if (c - 1 >= c_min && id[n - 1] != -1 && id[n - 1] == aux) // esquerda
    {
        tamanho = Mancha(coluna, linha, c - 1, l, id, tamanho + 1,c_min);
    }
    return tamanho;
}

/******************************************************************************
 *Aperta ()
 *
 * Argumentos: id - matriz
 *            coluna, linha - dimensoes da matriz
 *            head - incialmente coluna-1
 *            c - incialmente coluna-1
 *            usado - bool que devolve true or false caso a mancha apagada faca move horizontal
 *            da matriz
 *
 * Returns: (bool) usado (true or false), caso a mancha em estudo apos apagada 
 * influenciar a matriz, fazendo um movimento horizontal quando a matriz apresenta 
 * um coluna vazia (totalmente preecnchida por -1)
 * 
 * Side-Effects: Faz o movimento horizontal dos blocos da matriz, apos detetar uma coluna vazia
 *
 * Description: Faz o movimento horizontal de blocos de forma rescursiva 
 * se existir condicoes para tal e guarda um bool usado (verdadeiro se ocorrer queda, caso contrario falso)
 *
 *****************************************************************************/

bool Aperta(int coluna, int linha, int* id, int head, int c, bool usado, int c_min)
{
    int soma = 0;
    int i = c;
    for (int j = 0; j < linha; j++) { soma += id[i + j * coluna]; }
    if (soma == -linha) //se a soma dos ids for igual a -o numero de linhas, a coluna esta vazia
    {
        c--;
        if (c >= c_min) { usado = Aperta(coluna, linha, id, head, c, usado, c_min); }
    }
    else
    {
        if (head != c)
        {
            for (int j = 0; j < linha; j++)
            {
                id[head + j * coluna] = id[c + j * coluna];
                id[c + j * coluna] = -1;
                if (head < coluna - 1) { usado = true; }
            }
        }
        head--;
        c = head;
        if (head > 0) { usado = Aperta(coluna, linha, id, head, c, usado,c_min); }
    }
    return usado;
}

/******************************************************************************
 *Queda ()
 *
 * Argumentos: id - matriz
 *            coluna, linha - dimensoes da matriz
 *            c_min - delimita a ultima coluna util,a apartir dela esta tudo a -1's a esquerda
 *
 * Returns: (bool) usado (true or false), caso a mancha em estudo apos apagada 
 * influenciar a matriz, fazendo ou nao queda vertical de blocos
 * 
 * Side-Effects: Faz a queda vertical dos blocos da matriz, apos ser apagada a mancha em questao
 *
 * Description: Faz a queda vertical de blocos se existir condicoes para tal 
 * e guarda um bool usado (verdadeiro se ocorrer queda, caso contrario falso)
 *
 *****************************************************************************/
bool Queda(int coluna, int linha, int* id, int c_min)
{
    bool usado = false;
    for (int i = coluna-1;i>=c_min; i--) //percorre as colunas ate a c_min(utilma coluna util)
    {
        int partida = 0;
        for (int j = partida; j < linha; j++)//percorre todas as linhas da coluna
        {
            if (id[i + j * coluna] == -1)
            {
                for (int k = j + 1; k < linha; k++)//percorre todas as linhas da coluna acima do -1 encontrado
                {
                    if (id[i + k * coluna] != -1)
                    {
                        id[i + j * coluna] = id[i + k * coluna]; //troca a celula a -1 com a celula que caie toma a sua posicao na matriz
                        id[i + k * coluna] = -1;//mete a posicao da celula que agora caiu a -1
                        partida = k;//a partida agora e a celula onde foi efetuada a ultima troca(queda)
                        usado = true;
                        break;
                    }
                }
            }
        }
    }
    return usado;
}

/**************************
 * Move ()
 *
 * Argumentos: coluna, linha -  dimensões da matriz
 *             posição - l,c transformado em apenas uma cordenada
 *             id - matriz
 *
 * Returns: (bool) usadoQ || usadoA, retorna true se ocorrer um dos dois ou ambos (queda e aperta)
 * 
 * Side-effects: Realiza a chamada da funcao queda e aperta na matriz, apos a explosao de uma mancha
 * 
 * Descricao: Verifica se a mancha encontrada vai realizar uma das funções queda ou aperta
 * se o fizer a função vai proceder e eliminar essa macha caso contrario irá "guardá-la" 
 * para mais tarde. Consideramos teoricamente que uma mancha que nao faca queda nem aperta
 * nao tem de ser considerada com um filho naquele momento, esperamos ate aquela mancha se juntar
 * com outras ou explodimo-a no final da jogada, contudo testa se ha uma mancha por debaixo desta que 
 * crie uma linha vazia, se isto ocorrer a mancha e quebrada para nao evitar o movimento horizontal
 * de colunas
 * 
 *************************/

bool Move(int coluna, int linha, int posicao, int* id, int c_min)
{
    bool usadoQ, usadoA;
    usadoQ = Queda(coluna, linha, id,c_min);/*Faz a queda se houver posicoes vazias abaixo*/
    if(!usadoQ)
    {
    int aux=id[posicao%coluna];
    int num=posicao/coluna;
    int counter=1;
    for (int i = 1;i<posicao/coluna;i++){
        if(id[posicao%coluna+i*coluna]==aux){
            counter++;
        }
    }
    if(counter==num){
        usadoQ=true;
    }
    }
    usadoA = Aperta(coluna, linha, id, coluna - 1, coluna - 1, false, c_min); /*Aperta para a direita se houver linhas completas vazias*/
    return usadoQ || usadoA;
}

/**************************
 * EncontraManchas()
 *
 * Argumentos: stack - stack onde estao guardados os tabuleiros de todas a jogadas ate ao momento
 *             coluna, linha - dimensões da matriz
 *
 * Returns: (void)
 *
 * Descricao: vê a matriz que se encontra no topo da pilha (naquele momento) identifica todas as manchas
 * da matriz, e adiciona a posicao de todas as manchas que fazem queda e aperta ao vetor posicao filhos dessa matriz pai
 * no final adicuiona tambem ao vetor as ultimas manchas
 * 
 *************************/
void EncontraManchas(Stack* stack, int coluna, int linha, int c_min)
{
    Tabuleiro* pai = (Tabuleiro*)stack->array[stack->top];
    Tabuleiro*filho= (Tabuleiro*)stack->array[stack->top+1];
    Tabuleiro*aux= (Tabuleiro*)stack->array[stack->capacity-1];
    aux->posicao=-1;

    CopiaMatriz(coluna, linha,pai->id,aux->id);

    for (int l=0;l<linha; l++){
        for(int c=coluna-1;c>=c_min;c--){

            if((c+l)%2==0){ //le a matriz como se fosse as pecas pretas de um tabuleiro de xadrez (dois em dois), reduz para metade o numero de posicoes precisas de ser vistas na matriz
            

                if (aux->id[c+l*coluna] != -1){

                    int tamanho = Mancha(coluna, linha,c,l, aux->id, 1,c_min);
                    if (tamanho > 1){

                        CopiaMatriz(coluna, linha, pai->id,filho->id);
                        Mancha(coluna, linha,c,l, filho->id, 1, c_min);

                        if(Move(coluna, linha, c+l*coluna,filho->id,c_min)){
                            pai->posicao_filhos[pai->num_filhos]= c+l*coluna; //adiciona a posicao de todas as manchas encontradas ao vetor das posicoes dos filhos desse pai
                            pai->num_filhos++; //incrementa o numero de filhos do pai
                        }

                        else{
                        aux->posicao=c+l*coluna; //guarda a posicao das ultimas manchas, que nao fazem queda nem aperta
                        }
                    }
                }
            }
        }
    }

    if(pai->num_filhos==0 && aux->posicao>=0){ // adiciona as ultimas manchas ao vetor dos filhos 
        pai->posicao_filhos[pai->num_filhos]= aux->posicao;
        pai->num_filhos=1;
    }
}

/**************************
 * Encontralances ()
 *
 * Argumentos: stack - stack onde estao guardados os tabuleiros de todas a jogadas ate ao momento
 *             caminho - array que guarda as posições da melhor seuqencia de manchas explodidas em termos de pontos
 *             o caminho[0] guarda o maximo de pontos atingidos ate ao momento
 *             coluna, linha - dimensões da matriz 
 *             goal - objetivo do teste em questao
 *
 * Returns: (void)
 *
 * Descricao: procura recursivamente o melhor caminho possivél de forma a obter
 *  a maxima pontuação possível daquela matriz, utilizando os tabuleiros no stack 
 * que guardam essas jogadas, ou seja, vai a todos os ramos da "árvore" calcula os 
 * pontos e atualiza o caminho se este for superior ao anterior
 *************************/

void EncontraLances(Stack* stack, int* caminho, int coluna, int linha,int goal)
{
    /*Retorna se já tiver atingido os pontos pretendidos*/
    if (caminho[0] >= goal && goal>0) return; //sai progressivamente do DFS apos encontrar uma sequencia que cumpre os objetivos

    Tabuleiro* pai = (Tabuleiro*)stack->array[stack->top];
    Tabuleiro*filho= (Tabuleiro*)stack->array[stack->top+1];

    int c_min=coluna-1;

    for(int i=coluna-1;i>=0;i--){
        if(pai->id[i]!=-1){
            c_min=i; //encontra o c_min a ultima coluna util
        }
    }
    
    /*Encontra todas as manchas possiveis na matriz no topo do stack*/
    EncontraManchas(stack, coluna, linha,c_min);

    /*Adiciona tabuleiros no Stack em chamadas recursivas para todas as Manchas identificados acima*/
    
    for(int i=0; i<pai->num_filhos;i++){
        
        stack->top++;
        CopiaMatriz(coluna,linha,pai->id,filho->id);

        filho->posicao=pai->posicao_filhos[i];
        int tamanho = Mancha(coluna, linha, filho->posicao % coluna, filho->posicao/ coluna, filho->id, 1,c_min);

        filho->pontos=pai->pontos+(tamanho*(tamanho-1));
        Move(coluna, linha, filho->posicao,filho->id,c_min);

        EncontraLances(stack, caminho, coluna, linha,goal);
        if (caminho[0] >= goal && goal>0) break;
    }

    /*Calcula pontos para o caminho que está no stack. Se superar stack anterior, imprime*/
    if (caminho[0] <= goal && goal == 1) {MaxPontos(stack, caminho);}
    if (goal != 1) {MaxPontos(stack, caminho);}
    

    /*Reutiliza os Tabuleiros do stack para novas jogadas, evitando fazer tantos mallocs e frees*/
    if (stack->top > 0){
        Tabuleiro*temp=(Tabuleiro*)stack->array[stack->top];
        temp->posicao=-1;
        temp->pontos=0;
        temp->num_filhos=0;
        stack->top--;
    }
}