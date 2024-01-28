
#include "stack.h"
#include "tabuleiro.h"

/*Reutilizado do codigo fornecido pelo Lab 6 da cadeira de AED*/
/* function to create a stack of given capacity; initializes size as 0 */

Stack* createStack(unsigned capacity,int coluna, int linha)
{
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (Item*) malloc(stack->capacity * sizeof(Item));
    while(!isFull(stack)){ //Inicializamos o stack com tabuleiros tendo em conta o pior caso
      Tabuleiro*tab=initTab(capacity,coluna,linha);
      push(stack,tab);
    }
    return stack;
}

/* Stack is full when top is equal to the last index */
int isFull(Stack* stack)
{
    return stack->top == stack->capacity - 1;
}


/* Stack is empty when top is equal to -1 */
int isEmpty(Stack* stack)
{
  return stack->top == -1;
}


/* Function to add an item to stack.  It increases top by 1 */
void push(Stack* stack, Item item)
{
  /* cannot add if it is full */
  if (isFull(stack))
    return;
  stack->array[++stack->top] = item;

  return;
}


/* Function to remove an item from stack.  It decreases top by 1 */
Item pop(Stack* stack)
{
  /* nothing to return if it is empty */
  if (isEmpty(stack))
    return NULL;

  Item poppedItem = stack->array[stack->top--];

  return(poppedItem);
}


/* Function to return the top from stack without removing it */
Item peek(Stack* stack)
{
  if (isEmpty(stack))
    return NULL;

  return stack->array[stack->top];
}


/* function to delete all elements form stack */
void deleteStack(Stack *stack)
{
  /* note it does not delete the elements, just the stack itself */
  stack->top=stack->capacity-1;
  while(!isEmpty(stack)){ //damos free a todos os tabuleiros alocados
    Tabuleiro*lixo=pop(stack);
    deleteTabuleiro(lixo);
  }
  
  free (stack->array);
  free(stack);

  return;
}

