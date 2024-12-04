/**
 * @file symtable.c
 * @author Filo Jakub
 * @category Semantic alalysis
 * @brief This file contains functions for hash table
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"

// Utility Functions

char *copy_string(const char *str)
{
    if (str == NULL)
        return NULL;
    char *copy = malloc(strlen(str) + 1);
    if (copy)
    {
        strcpy(copy, str);
    }
    return copy;
}

void free_string(char *str)
{
    if (str != NULL)
    {
        free(str);
    }
}

// DJB2 Hash Function
// @http://www.cse.yorku.ca/~oz/hash.html
unsigned long djb2_hash(const char *str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    return hash;
}

// Initialize the symbol stack
SymbolStack *initialize_symbol_stack()
{
    SymbolStack *stack = create_symbol_stack();
    push_scope(stack); // Push the global scope
    printf("Entered global scope.\n");
    return stack;
}

// Hash Table Functions

HashTable *create_hash_table()
{
    HashTable *table = malloc(sizeof(HashTable));
    if (!table)
    {
        fprintf(stderr, "Error allocating memory for hash table.\n");
        exit(EXIT_FAILURE);
    }

    table->buckets = malloc(sizeof(Symbol *) * HASH_TABLE_SIZE);
    if (!table->buckets)
    {
        fprintf(stderr, "Error allocating memory for hash table buckets.\n");
        free(table);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        table->buckets[i] = NULL;
    }

    return table;
}

void insert_hash_table(HashTable *table, const char *name, DataType type, void *value, bool isConst, bool isNull, bool isGlobal, DataType freturn_type)
{
    unsigned long hash = djb2_hash(name) % HASH_TABLE_SIZE;

    // Check for existing symbol in current scope
    Symbol *existing = search_hash_table(table, name);
    if (existing)
    {
        fprintf(stderr, "Error: Redeclaration of symbol '%s'.\n", name);
        return;
    }

    // Create a new symbol
    Symbol *new_symbol = malloc(sizeof(Symbol));
    if (!new_symbol)
    {
        fprintf(stderr, "Error allocating memory for symbol.\n");
        exit(EXIT_FAILURE);
    }

    new_symbol->name = copy_string(name);
    new_symbol->type = type;
    new_symbol->isConst = isConst;
    new_symbol->isNull = isNull;
    new_symbol->isGlobal = isGlobal;
    new_symbol->freturn_type = freturn_type;

    if (!isNull)
    {
        // Initialize the union based on DataType
        switch (type)
        {
        case TYPE_INT:
        case TYPE_INT_NULL:
            new_symbol->value.intValue = *(int *)value;
            break;
        case TYPE_FLOAT:
        case TYPE_FLOAT_NULL:
            new_symbol->value.floatValue = *(float *)value;
            break;
        case TYPE_STRING:
        case TYPE_STRING_NULL:
            new_symbol->value.strValue = copy_string((char *)value);
            if (!new_symbol->value.strValue && type == TYPE_STRING)
            {
                fprintf(stderr, "Error: Memory allocation failed for string value '%s'.\n", (char *)value);
                free_string(new_symbol->name);
                free(new_symbol);
                return;
            }
            break;
        case TYPE_FUNCTION:
            new_symbol->value.params = (Symbol *)value;
            break;
        // Handle other types as needed
        default:
            // For unsupported types, zero out the union
            memset(&(new_symbol->value), 0, sizeof(new_symbol->value));
            break;
        }
    }
    else
    {
        // For null values, ensure the union is zeroed out
        memset(&(new_symbol->value), 0, sizeof(new_symbol->value));
    }
    // Insert the symbol at the beginning of the chain
    new_symbol->next = table->buckets[hash];
    table->buckets[hash] = new_symbol;
}

Symbol *search_hash_table(HashTable *table, const char *name)
{
    unsigned long hash = djb2_hash(name) % HASH_TABLE_SIZE;
    Symbol *current = table->buckets[hash];

    while (current)
    {
        if (strcmp(current->name, name) == 0)
        {
            return current; // Symbol found
        }
        current = current->next;
    }

    return NULL; // Symbol not found
}

void delete_hash_table(HashTable *table, const char *name)
{
    unsigned long hash = djb2_hash(name) % HASH_TABLE_SIZE;
    Symbol *current = table->buckets[hash];
    Symbol *prev = NULL;

    while (current)
    {
        if (strcmp(current->name, name) == 0)
        {
            if (prev)
            {
                prev->next = current->next;
            }
            else
            {
                table->buckets[hash] = current->next;
            }
            free_string(current->name);
            if (current->type == TYPE_STRING)
            {
                free(current->value.strValue);
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void free_hash_table(HashTable *table)
{
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        Symbol *current = table->buckets[i];
        while (current)
        {
            Symbol *temp = current;
            current = current->next;
            free_string(temp->name);
            if (temp->type == TYPE_STRING)
            {
                free(temp->value.strValue);
            }
            free(temp);
        }
    }
    free(table->buckets);
    free(table);
}

// Symbol Stack Functions

SymbolStack *create_symbol_stack()
{
    SymbolStack *stack = malloc(sizeof(SymbolStack));
    if (!stack)
    {
        fprintf(stderr, "Error allocating memory for symbol stack.\n");
        exit(EXIT_FAILURE);
    }
    stack->top = NULL;
    return stack;
}

void push_scope(SymbolStack *stack)
{
    Scope *new_scope = malloc(sizeof(Scope));
    if (!new_scope)
    {
        fprintf(stderr, "Error allocating memory for new scope.\n");
        exit(EXIT_FAILURE);
    }
    new_scope->table = create_hash_table();
    new_scope->next = stack->top;
    stack->top = new_scope;
}

void pop_scope(SymbolStack *stack)
{
    if (stack->top == NULL)
    {
        fprintf(stderr, "Error: No scope to pop.\n");
        return;
    }

    Scope *temp = stack->top;
    stack->top = stack->top->next;

    free_hash_table(temp->table);
    free(temp);
}

void insert_symbol_stack(SymbolStack *stack, const char *name, DataType type, void *value, bool isConst, bool isNull, bool isGlobal, DataType freturn_type)
{
    if (stack->top == NULL)
    {
        fprintf(stderr, "Error: No active scope to insert symbol '%s'.\n", name);
        return;
    }
    insert_hash_table(stack->top->table, name, type, value, isConst, isNull, isGlobal, freturn_type);
}

Symbol *search_symbol_stack(SymbolStack *stack, const char *name)
{
    Scope *current_scope = stack->top;
    while (current_scope)
    {
        Symbol *symbol = search_hash_table(current_scope->table, name);
        if (symbol)
        {
            return symbol;
        }
        current_scope = current_scope->next;
    }
    return NULL; // Symbol not found in any scope
}

int upd_var_symbol_stack(SymbolStack *stack, Symbol *symbol, void *new_value, DataType type)
{
    if (!stack || !symbol || !new_value)
    {
        return -1; // Invalid parameters
    }

    // Update the value based on the variable's type
    switch (type)
    {
    case TYPE_INT:
    case TYPE_INT_NULL:
        symbol->value.intValue = *(int *)new_value;
        break;

    case TYPE_FLOAT:
    case TYPE_FLOAT_NULL:
        symbol->value.floatValue = *(float *)new_value;
        break;

    case TYPE_STRING:
    case TYPE_STRING_NULL:
        // Free existing string and allocate a new one
        free(symbol->value.strValue);
        symbol->value.strValue = copy_string((char *)new_value);
        if (!symbol->value.strValue)
        {
            fprintf(stderr, "Error: Memory allocation failed for string value '%s'.\n", (char *)new_value);
            return -1;
        }
        break;

    case TYPE_FUNCTION:
        // Function type values should generally not be updated like this
        fprintf(stderr, "Error: Cannot update function type variable '%s'.\n", symbol->name);
        return -1;
    case TYPE_NULL:

    break;

    default:
        fprintf(stderr, "Error: Unsupported type for variable '%s'.\n", symbol->name);
        return -1; // Unsupported type
    }

    return 0; // Update successful
}

void delete_symbol_stack(SymbolStack *stack, const char *name)
{
    if (stack->top == NULL)
    {
        fprintf(stderr, "Error: No active scope to delete symbol '%s'.\n", name);
        return;
    }
    delete_hash_table(stack->top->table, name);
}

void free_symbol_stack(SymbolStack *stack)
{
    while (stack->top)
    {
        pop_scope(stack);
    }
    free(stack);
}
