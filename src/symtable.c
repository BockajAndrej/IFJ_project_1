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
// @https://theartincode.stanis.me/008-djb2/
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
    return stack;
}

// Hash Table Functions

HashTable *create_hash_table()
{
    // Allocate memory for the hash table structure
    HashTable *table = malloc(sizeof(HashTable));
    if (!table)
    {
        fprintf(stderr, "Error allocating memory for hash table.\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for the hash table's bucket array
    table->buckets = malloc(sizeof(Symbol *) * HASH_TABLE_SIZE);
    if (!table->buckets)
    {
        fprintf(stderr, "Error allocating memory for hash table buckets.\n");
        free(table);
        exit(EXIT_FAILURE);
    }

    // Initialize each bucket to NULL
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        table->buckets[i] = NULL;
    }

    return table;
}

void insert_hash_table(HashTable *table, const char *name, DataType type, void *value, bool isConst, bool isNull, bool isGlobal, DataType freturn_type)
{
    // Calculate the hash index for the symbol's name
    unsigned long hash = djb2_hash(name) % HASH_TABLE_SIZE;

    // Check for existing symbol in current scope
    Symbol *existing = search_hash_table(table, name);
    if (existing)
    {
        fprintf(stderr, "Error: Redeclaration of symbol '%s'.\n", name);
        return;
    }

    // Create a new symbol to insert into the hash table
    Symbol *new_symbol = malloc(sizeof(Symbol));
    if (!new_symbol)
    {
        fprintf(stderr, "Error allocating memory for symbol.\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the new symbol's fields
    new_symbol->name = copy_string(name);
    new_symbol->type = type;
    new_symbol->isConst = isConst;
    new_symbol->isNull = isNull;
    new_symbol->isGlobal = isGlobal;
    new_symbol->freturn_type = freturn_type;

    // If the value is not NULL, initialize the symbol's value based on its type
    if (!isNull)
    {
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
        default:
            memset(&(new_symbol->value), 0, sizeof(new_symbol->value)); // Zero out for unsupported types
            break;
        }
    }
    else
    {
        // If the symbol is marked as null, zero out its value
        memset(&(new_symbol->value), 0, sizeof(new_symbol->value));
    }

    // Insert the new symbol at the beginning of the linked list in the corresponding bucket
    new_symbol->next = table->buckets[hash];
    table->buckets[hash] = new_symbol;
}

Symbol *search_hash_table(HashTable *table, const char *name)
{
    unsigned long hash = djb2_hash(name) % HASH_TABLE_SIZE; // Hash the symbol's name to find its index
    Symbol *current = table->buckets[hash];                 // Start searching from the corresponding bucket

    while (current)
    {
        if (strcmp(current->name, name) == 0) // Check if the current symbol matches the name
        {
            return current; // Symbol found, return it
        }
        current = current->next; // Move to the next symbol in the bucket (chaining)
    }

    return NULL; // Symbol not found, return NULL
}

void delete_hash_table(HashTable *table, const char *name)
{
    unsigned long hash = djb2_hash(name) % HASH_TABLE_SIZE; // Hash the symbol's name to find its index
    Symbol *current = table->buckets[hash];                 // Start searching from the corresponding bucket
    Symbol *prev = NULL;                                    // Keep track of the previous symbol to update the list

    while (current)
    {
        if (strcmp(current->name, name) == 0) // Check if the current symbol matches the name
        {
            if (prev) // If not the first symbol in the list
            {
                prev->next = current->next; // Skip over the current symbol (delete it)
            }
            else
            {
                table->buckets[hash] = current->next; // If it's the first symbol, update the bucket's head
            }

            // Free resources associated with the symbol
            free_string(current->name); // Free the symbol's name
            if (current->type == TYPE_STRING)
            {
                free(current->value.strValue); // If the symbol is a string, free its value
            }
            free(current); // Free the symbol itself
            return;        // Done, exit the function
        }
        prev = current;          // Move the `prev` pointer to the current symbol
        current = current->next; // Move to the next symbol in the list
    }
}

void free_hash_table(HashTable *table)
{
    // Iterate through each bucket in the hash table
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        Symbol *current = table->buckets[i]; // Start at the head of the linked list for this bucket
        while (current)
        {
            Symbol *temp = current;
            current = current->next; // Move to the next symbol in the list
            free_string(temp->name); // Free the name of the symbol

            // If the symbol type is a string, free the string value
            if (temp->type == TYPE_STRING)
            {
                free(temp->value.strValue);
            }

            // Finally, free the symbol itself
            free(temp);
        }
    }

    // Free the array of buckets in the hash table
    free(table->buckets);

    // Free the hash table itself
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

    stack->top = NULL; // Initialize the top of the stack to NULL
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

    new_scope->table = create_hash_table(); // Create a new hash table for the new scope
    new_scope->next = stack->top;           // Link the new scope to the current top scope
    stack->top = new_scope;                 // Set the new scope as the top of the stack
}

void pop_scope(SymbolStack *stack)
{
    if (stack->top == NULL)
    {
        fprintf(stderr, "Error: No scope to pop.\n");
        return;
    }

    Scope *temp = stack->top;      // Save the current top scope
    stack->top = stack->top->next; // Move the top pointer to the next scope

    free_hash_table(temp->table); // Free the hash table associated with the old scope
    free(temp);                   // Free the memory allocated for the scope itself
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
    Scope *current_scope = stack->top; // Start from the top of the symbol stack (most recent scope)

    while (current_scope) // Traverse through the stack, from current to outer scopes
    {
        Symbol *symbol = search_hash_table(current_scope->table, name); // Search for symbol in the current scope's hash table
        if (symbol)                                                     // If symbol is found, return it
        {
            return symbol;
        }
        current_scope = current_scope->next; // Move to the next outer scope
    }

    return NULL; // Return NULL if the symbol was not found in any scope
}

int upd_var_symbol_stack(SymbolStack *stack, Symbol *symbol, void *new_value, DataType type)
{
    if (!stack || !symbol || !new_value) // Check for invalid parameters
    {
        return -1; // Return -1 for invalid parameters
    }

    // Update the symbol's value based on its data type
    switch (type)
    {
    case TYPE_INT:
    case TYPE_INT_NULL:
        symbol->value.intValue = *(int *)new_value; // Update int value
        break;

    case TYPE_FLOAT:
    case TYPE_FLOAT_NULL:
        symbol->value.floatValue = *(float *)new_value; // Update float value
        break;

    case TYPE_STRING:
    case TYPE_STRING_NULL:
        free(symbol->value.strValue);                            // Free the old string
        symbol->value.strValue = copy_string((char *)new_value); // Copy the new string
        if (!symbol->value.strValue)                             // Handle string memory allocation failure
        {
            fprintf(stderr, "Error: Memory allocation failed for string value '%s'.\n", (char *)new_value);
            return -1; // Return -1 on failure
        }
        break;

    case TYPE_FUNCTION:
        // Function values should not be updated in this manner
        fprintf(stderr, "Error: Cannot update function type variable '%s'.\n", symbol->name);
        return -1; // Return -1 for function type symbols
    case TYPE_NULL:
        break;

    default:
        // Handle unsupported types
        fprintf(stderr, "Error: Unsupported type for variable '%s'.\n", symbol->name);
        return -1; // Return -1 for unsupported types
    }

    return 0; // Return 0 on successful update
}

void delete_symbol_stack(SymbolStack *stack, const char *name)
{
    if (stack->top == NULL) // Check if there is no active scope
    {
        fprintf(stderr, "Error: No active scope to delete symbol '%s'.\n", name);
        return;
    }
    delete_hash_table(stack->top->table, name); // Delete the symbol from the current scope's hash table
}

void free_symbol_stack(SymbolStack *stack)
{
    while (stack->top) // Loop through all scopes
    {
        pop_scope(stack); // Pop each scope and free its associated resources
    }
    free(stack); // Free the memory for the symbol stack itself
}
