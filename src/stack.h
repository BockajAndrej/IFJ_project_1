#ifndef STACK_H
#define STACK_H

#include <stdbool.h> // Pre prácu s dátovým typom bool
#include "expression.h"
#include "lexical_analyser.h"

// Počiatočná kapacita zásobníka
#define INITIAL_CAPACITY 0

typedef enum typeOfData_t
{
    // Types for data union
    precedence,
    rule,
    // Types for tokens
    integer,
    floating,
    string, 
    boolean
}TypeOfData;

typedef struct stack_item_1_t
{
    int symbol;
    bool type;   // True => symbol = precedence
}Stack_item_1;

typedef union stack_item_0_t
{
    Stack_item_1 precedence;
    Token_Value token_val;
    Prec_rules_enum rules;
}Stack_item_0;

typedef struct stack_item_t
{
    Stack_item_0 data;
    TypeOfData type;
    struct stack_item_t *next;
    struct stack_item_t *prev;
} Stack_item;

// Definícia štruktúry Zásobníka
typedef struct
{
    Stack_item *top; // Index najvyššieho prvku
    Stack_item *bottom;
    int capacity;    // Aktuálna kapacita zásobníka
} Stack;

// Funkcia na inicializáciu zásobníka
void initStack(Stack *s, TypeOfData type);

// Funkcia na kontrolu, či je zásobník prázdny
bool isEmpty(Stack_item *s);

// Funkcia na pridanie prvku na vrchol zásobníka
void push(Stack *s, const Stack_item item);

void pushAfterTerminal(Stack *s, const Stack_item item);

// Funkcia na odstránenie a vrátenie vrcholového prvku zo zásobníka
void RemoveTop(Stack *s, Stack_item *retItem);

// Funkcia na získanie vrcholového prvku bez jeho odstránenia
void topElement(Stack *s, Stack_item *retItem);

int topTerminal(Stack *s);

// Funkcia na uvoľnenie pamäte alokovanej pre zásobník
void freeStack(Stack *s);

bool isTerminal(Stack_item *Item);

void PrintAllStack(Stack *s);

#endif // STACK_H