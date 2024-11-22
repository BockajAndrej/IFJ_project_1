#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

// Funkcia na inicializáciu zásobníka
void initStack(Stack *s, TypeOfData type)
{
    s->top = (Stack_item *)malloc(sizeof(Stack_item));
    if (s->top == NULL)
    {
        fprintf(stderr, "Chyba: malloc initStack.\n");
        exit(EXIT_FAILURE);
    }
    // Ukazovatele
    s->bottom = s->top;
    s->top->next = NULL;
    s->top->prev = NULL;
    // Plnenie hodnot
    s->top->type = type;
    s->top->data.isPrec = true;
    s->top->data.token_type = TOKEN_EMPTY;
    if (!dynamic_string_init(&s->top->data.token_val.valueString))
    {
        fprintf(stderr, "Chyba: dynamic string init.\n");
        exit(EXIT_FAILURE);
    }
    dynamic_string_add_char(&s->top->data.token_val.valueString, '$');
    s->capacity = 1;
}

// Funkcia na kontrolu, či je zásobník prázdny
bool isEmpty(Stack_item *s)
{
    return ((strcmp(s->data.token_val.valueString.str, "$") == 0) && (s->data.token_type == TOKEN_EMPTY));
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
    // Nastavenie ukazovatelov
    newItem->next = NULL;
    newItem->prev = s->top;
    s->top->next = newItem;
    s->top = newItem;
    // Plnenie hodnot
    s->top->type = item.type;
    s->top->data.isPrec = item.data.isPrec;
    s->top->data.token_type = item.data.token_type;
    s->top->data.token_val = item.data.token_val;
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
            while (!isTerminal(tmp->prev))
                tmp = tmp->prev;
            // Presmerovanie ukazovatelov
            newItem->next = tmp;
            newItem->prev = tmp->prev;
            tmp->prev->next = newItem;
            tmp->prev = newItem;
            // Plnenie hodnot
            newItem->type = item.type;
            newItem->data.isPrec = item.data.isPrec;
            newItem->data.token_type = item.data.token_type;
            newItem->data.token_val = item.data.token_val;
            // Zvysenie aktualnej kapacity
            s->capacity++;
        }
    }
}

/// @brief Remove top element from stack
/// @param s stack
/// @param retSymbol returning symbol
/// @return Type of symbol
void RemoveTop(Stack *s, Stack_item *retItem)
{
    // Naplnenie navratovej hodnoty
    getElement(s, retItem);

    Stack_item *tmp = s->top;
    s->top = s->top->prev;
    s->top->next = NULL;
    s->capacity--;
    free(tmp);
}

/// @brief Funkcia na získanie vrcholového prvku bez jeho odstránenia
/// @param s
/// @param retItem
/// @param dir Urcuje smer ci bottom(false) alebo top(true)
void getElement(Stack *s, Stack_item *retItem)
{
    if (isEmpty(s->top))
    {
        fprintf(stderr, "Chyba: Pokus o pop z prázdneho zásobníka.\n");
        exit(EXIT_FAILURE);
    }
    retItem->type = s->top->type;
    retItem->data.isPrec = s->top->data.isPrec;
    retItem->data.token_type = s->top->data.token_type;
    retItem->data.token_val = s->top->data.token_val;
}

int topTerminal(Stack *s)
{
    Stack_item *tmp = s->top;
    // Posunutie ukazovatela na terminal
    while (!isEmpty(tmp))
    {
        if (isTerminal(tmp))
            return tmp->data.token_type;
        tmp = tmp->prev;
    }
    return EOF;
}

// Funkcia na uvoľnenie pamäte alokovanej pre zásobník
void freeStack(Stack *s)
{
    // Uvoľni pamäť pred návratom
    dynamic_string_free(&s->top->data.token_val.valueString);

    while (s->top != NULL)
    {
        Stack_item *tmp = s->top;
        s->top = s->top->prev;
        free(tmp);
    }
    s->top = NULL;
    s->capacity = 0;
}

bool isTerminal(Stack_item *Item)
{
    if (Item->type == precedence)
        return (Item->data.isPrec && (Item->data.token_type != TOKEN_NTERMINAL));
    return false;
}

void PrintAllStack(Stack *s)
{
    Stack_item *tmp = s->top;
    for (int i = s->capacity - 1; !isEmpty(tmp); i--)
    {
        if (s->top->type == precedence)
            printf("Stack (prec) %d: %s - (type) = %d\n", i, tmp->data.token_val.valueString.str, tmp->data.token_type);
        else
            printf("Stack (token) %d: %s - (type) = %d\n", i, tmp->data.token_val.valueString.str, tmp->data.token_type);
        tmp = tmp->prev;
    }
}
