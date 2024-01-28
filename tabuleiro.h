#ifndef TABULEIRO_H
#define TABULEIRO_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "stack.h"

typedef struct _tabuleiro{
  int *id;
  int posicao;
  int num_filhos;
  int pontos;
  int* posicao_filhos;
} Tabuleiro;

FILE*ficheiro_entrada(char*titulo);
int* data(FILE* input, int linha,int coluna);
FILE*ficheiro_out(char*titulo);
Tabuleiro* initTab(int capacity, int coluna, int linha);
void deleteTabuleiro (Tabuleiro* tabuleiro);
void MaxPontos(Stack* stack, int* caminho);
void CopiaMatriz(int coluna, int linha, int* original, int*copia);

#endif