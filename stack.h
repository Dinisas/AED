#ifndef STACK_H
#define STACK_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "Item.h"

typedef struct stack_ {
    int top;
    unsigned capacity;
    Item*array;
}Stack;

Stack*  createStack (unsigned capacity, int coluna, int linha);
int     isFull (Stack*);
int     isEmpty (Stack*);
void    push (Stack*, Item);
Item    pop (Stack* stack);
Item    peek (Stack*);
void    deleteStack (Stack *stack);

#endif