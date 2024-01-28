#ifndef MANCHA_H
#define MANCHA_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "stack.h"

int Mancha(int coluna, int linha, int c, int l, int* id, int tamanho, int c_min);
bool Aperta(int coluna, int linha, int* id, int head, int c, bool usado, int c_min);
bool Queda(int coluna, int linha, int* id, int c_min);
bool Move(int coluna, int linha, int posicao, int* id, int c_min);
void EncontraManchas(Stack* stack, int coluna, int linha, int c_min);
void EncontraLances(Stack* stack, int* caminho, int coluna, int linha, int goal);

#endif