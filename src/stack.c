/**
 * @file stack.c
 * @author Bockaj Andrej
 * @category Syntaktic analysis
 * @brief This file contains functions for stack
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

// Init stack

/// @brief Init stack
/// @param s pointer to stack
/// @param type which type of stack (rules or termianls)
void initStack(Stack *s, TypeOfData type)
{
    s->top = (Stack_item *)malloc(sizeof(Stack_item));
    if (s->top == NULL)
    {
        fprintf(stderr, "Chyba: malloc initStack.\n");
        exit(EXIT_FAILURE);
    }
    // Pointers
    s->bottom = s->top;
    s->top->next = NULL;
    s->top->prev = NULL;
    // Inserting values
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

/// @brief Check if stack is empty
/// @param s pointer to stack
/// @return if is stack empty
bool isEmpty(Stack_item *s)
{
    return ((strcmp(s->data.token_val.valueString.str, "$") == 0) && (s->data.token_type == TOKEN_EMPTY));
}

/// @brief Insert into stack
/// @param s pointer to stack
/// @param item inserting item
void push(Stack *s, const Stack_item item)
{
    Stack_item *newItem = (Stack_item *)malloc(sizeof(Stack_item));
    if (newItem == NULL)
    {
        fprintf(stderr, "Chyba: Pokus o push zásobníka.\n");
        exit(EXIT_FAILURE);
    }
    // Pointers
    newItem->next = NULL;
    newItem->prev = s->top;
    s->top->next = newItem;
    s->top = newItem;
    // Inserting values
    s->top->type = item.type;
    s->top->data.isPrec = item.data.isPrec;
    s->top->data.token_type = item.data.token_type;
    s->top->data.token_val = item.data.token_val;
    s->capacity++;
}

/// @brief Insert into stack but immediately next to the last terminal 
/// @param s pointer to stack
/// @param item inserting item
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
        // Shift pointer to the terminal
        if (isTerminal(tmp))
            push(s, item);
        else
        {
            while (!isTerminal(tmp->prev))
                tmp = tmp->prev;
            // Pointers
            newItem->next = tmp;
            newItem->prev = tmp->prev;
            tmp->prev->next = newItem;
            tmp->prev = newItem;
            // Inserting values
            newItem->type = item.type;
            newItem->data.isPrec = item.data.isPrec;
            newItem->data.token_type = item.data.token_type;
            newItem->data.token_val = item.data.token_val;
            // Increase actual capacity
            s->capacity++;
        }
    }
}

/// @brief Remove top element from stack
/// @param s pointer to stack
/// @param retItem returning item
void RemoveTop(Stack *s, Stack_item *retItem)
{
    // Full fill return value
    getElement(s, retItem);

    Stack_item *tmp = s->top;
    s->top = s->top->prev;
    s->top->next = NULL;
    s->capacity--;
    free(tmp);
}

/// @brief Function return top element without removing 
/// @param s pointer to stack
/// @param retItem returning item
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

/// @brief Return last entered terminal
/// @param s pointer to stack
/// @return Return last terminal
int topTerminal(Stack *s)
{
    Stack_item *tmp = s->top;
    // Shift pointer to the terminal
    while (!isEmpty(tmp))
    {
        if (isTerminal(tmp))
            return tmp->data.token_type;
        tmp = tmp->prev;
    }
    return EOF;
}

/// @brief Function for deallocation memory
/// @param s pointer to stack
void freeStack(Stack *s)
{
    // Dealloc string memory
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

/// @brief Check if item is terminal
/// @param Item checking item
/// @return if is it terminal return true
bool isTerminal(Stack_item *Item)
{
    if (Item->type == precedence)
        return (Item->data.isPrec && (Item->data.token_type != TOKEN_NTERMINAL));
    return false;
}

/// @brief Print all elements in stack
/// @param s pointer to stack
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
