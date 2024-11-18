#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

// Funkcia na inicializáciu zásobníka
void initStack(Stack *s)
{
    s->top = (Stack_item *)malloc(sizeof(Stack_item));
    if (s->top == NULL)
    {
        fprintf(stderr, "Chyba: malloc initStack.\n");
        exit(EXIT_FAILURE);
    }
    s->top->symbol.precedence = DOLLAR;
    s->top->type = true;
    s->top->next = NULL;
    s->capacity = 1;
}

// Funkcia na kontrolu, či je zásobník prázdny
bool isEmpty(Stack_item *s)
{
    if (s->type)
        return s->symbol.precedence == DOLLAR;
    return false;
}

// Funkcia na pridanie prvku na vrchol zásobníka
void push(Stack *s, const Stack_item item)
{
    Stack_item *newItem = (Stack_item *)malloc(sizeof(Stack_item));
    if (newItem == NULL)
    {
        fprintf(stderr, "Chyba: Pokus o push zásobníka.\n");
        exit(EXIT_FAILURE);
    }
    // Plnenie hodnot
    newItem->next = s->top;
    newItem->type = item.type;
    if (item.type)
        newItem->symbol.precedence = item.symbol.precedence;
    else
        newItem->symbol.operand = item.symbol.operand;
    // Presmerovanie top ukazovatela
    s->top = newItem;
    s->capacity++;
}

void pushAfterTerminal(Stack *s, const Stack_item item)
{
    if (isEmpty(s->top))
        push(s, item);
    else
    {
        Stack_item *tmp = s->top;
        Stack_item *newItem = (Stack_item *)malloc(sizeof(Stack_item));
        if (newItem == NULL)
        {
            fprintf(stderr, "Chyba: Pokus o push zásobníka.\n");
            exit(EXIT_FAILURE);
        }
        // Posunutie ukazovatela na terminal
        if (isTerminal(tmp))
            push(s, item);
        else
        {
            while (!isTerminal(tmp->next))
                tmp = tmp->next;
            // Plnenie hodnot
            newItem->next = tmp->next;
            newItem->type = item.type;
            if (item.type)
                newItem->symbol.precedence = item.symbol.precedence;
            else
                newItem->symbol.operand = item.symbol.operand;
            // Presmerovanie ukazovatela
            tmp->next = newItem;
            s->capacity++;
        }
    }
}

/// @brief Remove top element from stack
/// @param s stack
/// @param retSymbol returning symbol
/// @return Type of symbol
void pop(Stack *s, Stack_item *retItem)
{
    if (isEmpty(s->top))
    {
        fprintf(stderr, "Chyba: Pokus o pop z prázdneho zásobníka.\n");
        exit(EXIT_FAILURE);
    }
    retItem->type = s->top->type;
    if (s->top->type)
        retItem->symbol.precedence = s->top->symbol.precedence;
    else
        retItem->symbol.operand = s->top->symbol.operand;
    Stack_item *tmp = s->top;
    s->top = s->top->next;
    s->capacity--;
    free(tmp);
}

// Funkcia na získanie vrcholového prvku bez jeho odstránenia
int topElement(Stack *s)
{
    if (s->top->type)
        return s->top->symbol.precedence;
    return s->top->symbol.operand;
}

int topTerminal(Stack *s)
{
    Stack_item *tmp = s->top;
    // Posunutie ukazovatela na terminal
    while (!isEmpty(tmp))
    {
        if (isTerminal(tmp))
            return tmp->symbol.precedence;
        tmp = tmp->next;
    }
    return EOF;
}

// Funkcia na uvoľnenie pamäte alokovanej pre zásobník
void freeStack(Stack *s)
{
    while (s->top != NULL)
    {
        Stack_item *tmp = s->top;
        s->top = s->top->next;
        free(tmp);
    }
    s->top = NULL;
    s->capacity = 0;
}

bool isTerminal(Stack_item *Item)
{
    if (Item->type)
        if (Item->symbol.precedence != NTERMINAL)
            return true;
    return false;
}

void PrintAllStack(Stack *s)
{
    Stack_item *tmp = s->top;
    for (int i = 0; !isEmpty(tmp); i++)
    {
        if (tmp->type)
            printf("Stack %d: %d\n", i, tmp->symbol.precedence);
        else
            printf("Stack %d: %c\n", i, tmp->symbol.operand);
        tmp = tmp->next;
    }
}