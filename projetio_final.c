#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "stack.h"
#include "mancha.h"
#include "tabuleiro.h"

int main(int argc,char**argv) {

    if(argc<2){
        printf("Error:missing filename in argument!\n");
        exit (EXIT_FAILURE);
    }

    FILE*input_entrada=ficheiro_entrada(argv[1]);
    FILE*input_saida=ficheiro_out(argv[1]);

    int linha, coluna, v;
    
    //while loop que percorre todas as matrzies no ficheiro enquanto houver matrzies para ler
    while(fscanf(input_entrada,"%d %d %d", &linha, &coluna, &v)!=EOF){    

    fprintf(input_saida,"%d %d %d\n",linha,coluna,v);
    int* id= data(input_entrada, linha, coluna);
    int capacity = coluna * linha / 2 + 2; /* +1 para ter espaco para adicionar a matriz inicial !!*/
    
    int* caminho = (int*)malloc(capacity * sizeof(int));
    caminho[0]=0;

    for(int i=1;i<capacity;i++){ //inicializamos o caminho a -1
        caminho[i]=-1;
    }

    Stack* stack = createStack(capacity,coluna,linha); //Criamos o stack
    stack->top=0;

    Tabuleiro*aux=(Tabuleiro*)peek(stack);
    CopiaMatriz(coluna,linha,id,aux->id);

    int counter=0;
    if(v>0 || v==-3){
        EncontraLances(stack, caminho, coluna, linha,v); //chamamos o EncontraLances com o objetivo pedido
        if((v>0 && caminho[0]>=v)||v==-3){
            for(int i=1;i<capacity;i++){
                 if(caminho[i]!=-1){
                 counter++;
                }
            }

            fprintf(input_saida,"%d %d\n",counter,caminho[0]); //escrevemos no ficheiro os pontos,numero de lances e os respetivos lances
            for(int j=1;j<=counter;j++){
                fprintf(input_saida,"%d %d\n",caminho[j] / coluna + 1, caminho[j] % coluna + 1);
            }
        }
        else{
            fprintf(input_saida,"%d %d",0,-1);
        }

    fprintf(input_saida,"\n\n");
    }

    if(v==0 || v==-1){
        EncontraLances(stack, caminho, coluna, linha,1); //chamamos o EncontraLances com objetivo 1
        for(int i=1;i<capacity;i++){
            if(caminho[i]!=-1){
                counter++;
            }
        }

        fprintf(input_saida,"%d %d\n",counter,caminho[0]); //escrevemos no ficheiro os pontos,numero de lances e os respetivos lances
        for(int j=1;j<=counter;j++){
            fprintf(input_saida,"%d %d\n",caminho[j] / coluna + 1, caminho[j] % coluna + 1);
        }
        fprintf(input_saida,"\n");
    }

    deleteStack(stack); //damos free a tudo o que alocamos
    free(id);
    free(caminho);
    }

    fclose(input_entrada);
    fclose(input_saida);
    return 0;
}