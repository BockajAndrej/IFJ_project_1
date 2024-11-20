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
    s->bottom = s->top;
    s->top->type = type;
    if (s->top->type == precedence)
    {
        s->top->data.precedence.type = true;
        s->top->data.precedence.symbol = DOLLAR;
    }
    else if (s->top->type == rule)
    {
        s->top->data.rules = N_DOLLAR;
    }
    else
    {
        s->top->type = string;
        if (!dynamic_string_init(&s->top->data.token_val.valueString))
        {
            fprintf(stderr, "Chyba: dynamic string init.\n");
            exit(EXIT_FAILURE);
        }
        dynamic_string_add_char(&s->top->data.token_val.valueString, '$');
    }

    s->top->next = NULL;
    s->top->prev = NULL;
    s->capacity = 1;
}

// Funkcia na kontrolu, či je zásobník prázdny
bool isEmpty(Stack_item *s)
{
    if (s->type == precedence)
        return ((s->data.precedence.type == true) && (s->data.precedence.symbol == DOLLAR));
    else if (s->type == rule)
        return s->data.rules == N_DOLLAR;
    else if (s->type == string)
        return strcmp(s->data.token_val.valueString.str, "$") == 0;
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
    // Nastavenie ukazovatelov
    newItem->next = NULL;
    newItem->prev = s->top;
    s->top->next = newItem;
    s->top = newItem;
    s->top->type = item.type;

    if (s->top->type == precedence)
    {
        s->top->data.precedence.type = item.data.precedence.type;
        s->top->data.precedence.symbol = item.data.precedence.symbol;
    }
    else if (s->top->type == rule)
    {
        s->top->data.rules = item.data.rules;
    }
    else
    {
        switch (s->top->type)
        {
        case integer:
            s->top->data.token_val.intValue = item.data.token_val.intValue;
            break;
        case floating:
            s->top->data.token_val.floatValue = item.data.token_val.floatValue;
            break;
        case string:
            s->top->data.token_val.valueString = item.data.token_val.valueString;
            break;
        case boolean:
            s->top->data.token_val.boolValue = item.data.token_val.boolValue;
            break;
        default:
            break;
        }
    }
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
            newItem->data.precedence.type = item.data.precedence.type;
            newItem->data.precedence.symbol = item.data.precedence.symbol;
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
    getElement(s, retItem, true);

    Stack_item *tmp = s->top;
    s->top = s->top->prev;
    s->top->next = NULL;
    s->capacity--;
    free(tmp);
}

void RemoveBottom(Stack *s, Stack_item *retItem)
{
    // Naplnenie navratovej hodnoty
    getElement(s, retItem, false);
    Stack_item *tmp = s->bottom->next;
    if (tmp->next != NULL)
    {
        tmp->next->prev = tmp->prev;
        tmp->prev->next = tmp->next;
    }
    else
    {
        s->top = s->bottom;
        s->top->next = NULL;
    }
    s->capacity--;
    free(tmp);
}

/// @brief Funkcia na získanie vrcholového prvku bez jeho odstránenia
/// @param s
/// @param retItem
/// @param dir Urcuje smer ci bottom(false) alebo top(true)
void getElement(Stack *s, Stack_item *retItem, bool dir)
{
    if (dir)
    {
        if (isEmpty(s->top))
        {
            fprintf(stderr, "Chyba: Pokus o pop z prázdneho zásobníka.\n");
            exit(EXIT_FAILURE);
        }
        retItem->type = s->top->type;
        if (retItem->type == precedence)
        {
            retItem->data.precedence.type = s->top->data.precedence.type;
            retItem->data.precedence.symbol = s->top->data.precedence.symbol;
        }
        else if (retItem->type == rule)
        {
            retItem->data.rules = s->top->data.rules;
        }
        else
        {
            switch (s->top->type)
            {
            case integer:
                retItem->data.token_val.intValue = s->top->data.token_val.intValue;
                break;
            case floating:
                retItem->data.token_val.floatValue = s->top->data.token_val.floatValue;
                break;
            case string:
                retItem->data.token_val.valueString = s->top->data.token_val.valueString;
                break;
            case boolean:
                retItem->data.token_val.boolValue = s->top->data.token_val.boolValue;
                break;
            default:
                break;
            }
        }
    }
    else
    {
        if (isEmpty(s->bottom->next))
        {
            fprintf(stderr, "Chyba: Pokus o get z prázdneho zásobníka.\n");
            exit(EXIT_FAILURE);
        }
        retItem->type = s->bottom->next->type;
        if (retItem->type == precedence)
        {
            retItem->data.precedence.type = s->bottom->next->data.precedence.type;
            retItem->data.precedence.symbol = s->bottom->next->data.precedence.symbol;
        }
        else if (retItem->type == rule)
        {
            retItem->data.rules = s->bottom->next->data.rules;
        }
        else
        {
            switch (s->bottom->next->type)
            {
            case integer:
                retItem->data.token_val.intValue = s->bottom->next->data.token_val.intValue;
                break;
            case floating:
                retItem->data.token_val.floatValue = s->bottom->next->data.token_val.floatValue;
                break;
            case string:
                retItem->data.token_val.valueString = s->bottom->next->data.token_val.valueString;
                break;
            case boolean:
                retItem->data.token_val.boolValue = s->bottom->next->data.token_val.boolValue;
                break;
            default:
                break;
            }
        }
    }
}

int topTerminal(Stack *s)
{
    Stack_item *tmp = s->top;
    // Posunutie ukazovatela na terminal
    while (!isEmpty(tmp))
    {
        if (isTerminal(tmp))
            return tmp->data.precedence.symbol;
        tmp = tmp->prev;
    }
    return EOF;
}

// Funkcia na uvoľnenie pamäte alokovanej pre zásobník
void freeStack(Stack *s)
{
    // Uvoľni pamäť pred návratom
    if (s->top->type == string)
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
        return (Item->data.precedence.type == true && Item->data.precedence.symbol != NTERMINAL);
    return false;
}

void PrintAllStack(Stack *s)
{
    Stack_item *tmp = s->top;
    for (int i = s->capacity - 1; !isEmpty(tmp); i--)
    {
        if (s->top->type == precedence)
        {
            if (tmp->data.precedence.type)
                printf("Stack enum %d: %d\n", i, tmp->data.precedence.symbol);
            else
                printf("Stack char %d: %c\n", i, tmp->data.precedence.symbol);
        }
        else if (s->top->type == rule)
            printf("Rule Stack %d: %d\n", i, tmp->data.rules);
        else
        {
            switch (s->top->type)
            {
            case integer:
                printf("Token int Stack %d: %d\n", i, tmp->data.token_val.intValue);
                break;
            case floating:
                printf("Token float Stack %d: %f\n", i, tmp->data.token_val.floatValue);
                break;
            case string:
                printf("Token string Stack %d: %s\n", i, tmp->data.token_val.valueString.str);
                break;
            case boolean:
                printf("Token bool Stack %d: %d\n", i, tmp->data.token_val.boolValue);
                break;
            default:
                break;
            }
        }
        tmp = tmp->prev;
    }
}
