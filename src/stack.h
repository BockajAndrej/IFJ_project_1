#ifndef STACK_H
#define STACK_H

#include <stdbool.h> // Pre prácu s dátovým typom bool
#include "expression.h"

// Počiatočná kapacita zásobníka
#define INITIAL_CAPACITY 0

typedef struct stack_item_t
{
    Stack_symbol symbol;
    bool type;  // True => symbol = precedence
    struct stack_item_t *next;
} Stack_item;

// Definícia štruktúry Zásobníka
typedef struct
{
    Stack_item *top; // Index najvyššieho prvku
    int capacity;    // Aktuálna kapacita zásobníka
} Stack;

// Funkcia na inicializáciu zásobníka
void initStack(Stack *s);

// Funkcia na kontrolu, či je zásobník prázdny
bool isEmpty(Stack_item *s);

// Funkcia na pridanie prvku na vrchol zásobníka
void push(Stack *s, Stack_item item);

void pushAfterTerminal(Stack *s, Stack_item item);

// Funkcia na odstránenie a vrátenie vrcholového prvku zo zásobníka
void pop(Stack *s, Stack_item *retItem);

// Funkcia na získanie vrcholového prvku bez jeho odstránenia
int topElement(Stack *s);

int topTerminal(Stack *s);

// Funkcia na vyprázdnenie zásobníka bez uvoľnenia pamäte
void resetStack(Stack *s);

// Funkcia na uvoľnenie pamäte alokovanej pre zásobník
void freeStack(Stack *s);

bool isTerminal(Stack_item *Item);

void PrintAllStack(Stack *s);

#endif // STACK_H