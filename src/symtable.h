#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "ast.h"

#define HASH_TABLE_SIZE 211

typedef struct Symbol
{
    char *name;    // Symbol name
    DataType type; // Data type
    bool isConst;  // Indicates if the symbol is a constant
    bool isNull;   // Indicates if the symbol currently holds a null value
    union
    {
        int intValue;          // Integer value
        float floatValue;      // Float value
        char *strValue;        // String value
        struct Symbol *params; // Pointer to parameter chain for functions
    } value;
    struct Symbol *next; // Pointer for chaining in hash table
} Symbol;

typedef struct HashTable
{
    Symbol **buckets; // Array of pointers to Symbol (chains)
} HashTable;

typedef struct Scope
{
    HashTable *table;
    struct Scope *next; // Pointer to the next scope in the stack
} Scope;

typedef struct SymbolStack
{
    Scope *top; // Top of the stack (current scope)
} SymbolStack;

// Utility functions
char *copy_string(const char *str);
void free_string(char *str);

// Hash table functions
HashTable *create_hash_table();
void insert_hash_table(HashTable *table, const char *name, DataType type, void *value, bool isConst, bool isNull);
Symbol *search_hash_table(HashTable *table, const char *name);
void delete_hash_table(HashTable *table, const char *name);
void free_hash_table(HashTable *table);

// Symbol stack functions
SymbolStack *create_symbol_stack();
SymbolStack *initialize_symbol_stack();
void push_scope(SymbolStack *stack);
void pop_scope(SymbolStack *stack);
void insert_symbol_stack(SymbolStack *stack, const char *name, DataType type, void *value, bool isConst, bool isNull);
Symbol *search_symbol_stack(SymbolStack *stack, const char *name);
void delete_symbol_stack(SymbolStack *stack, const char *name);
void free_symbol_stack(SymbolStack *stack);

#endif
