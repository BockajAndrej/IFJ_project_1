#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ADT.h"

/**
 * @brief Copies a string dynamically.
 * 
 * @details This function allocates memory for a new string and copies the content
 * of the provided string into the newly allocated memory. If memory allocation fails,
 * it returns NULL.
 * 
 * @param str Pointer to the string to copy.
 * @return A dynamically allocated copy of the string, or NULL if memory allocation fails.
 */
char *copy_string(const char *str) {

    char *copy = malloc(strlen(str) + 1);

    if (copy != NULL) {

        strcpy(copy, str);
    }

    return copy;
}

/**
 * @brief Frees a dynamically allocated string.
 * 
 * @details This function deallocates the memory previously allocated for a string,
 * ensuring there are no memory leaks.
 * 
 * @param str Pointer to the string to free.
 */
void free_string(char *str) {

    free(str);
}

/**
 * @brief Hashes a string to generate an index in the hash table.
 * 
 * @details This function calculates the hash value of the provided string using a 
 * simple polynomial rolling hash function, returning an index suitable for use 
 * in the hash table.
 * 
 * @param key Pointer to the string to hash.
 * @return Hash index within the range of the table size.
 */
unsigned int hash(const char *key) {

    unsigned int hash_value = 0;

    for (int i = 0; key[i] != '\0'; i++) {

        hash_value = 31 * hash_value + key[i];
    }

    return hash_value % TABLE_SIZE;
}

/**
 * @brief Creates a new hash table.
 * 
 * @details This function allocates memory for a new hash table and initializes
 * all the entries to NULL. If memory allocation fails, it returns NULL.
 * 
 * @return Pointer to a newly allocated HashTable, or NULL if allocation fails.
 */
HashTable *create_table() {

    HashTable *table = malloc(sizeof(HashTable));

    if (table) {

        for (int i = 0; i < TABLE_SIZE; i++) {

            table->table[i] = NULL;
        }
    }

    return table;
}

/**
 * @brief Inserts a symbol into the hash table.
 * 
 * @details This function creates a new symbol entry and inserts it into the 
 * hash table at the index determined by the hash of the symbol's name. If a 
 * collision occurs, the new symbol is added to the linked list at that index.
 * 
 * @param table Pointer to the hash table.
 * @param name Name of the symbol.
 * @param type Type of the symbol.
 * @param intValue Integer value of the symbol (if applicable).
 * @param hasInt Indicates if intValue is valid.
 * @param floatValue Floating-point value of the symbol (if applicable).
 * @param hasFloat Indicates if floatValue is valid.
 * @param address Memory address of the symbol.
 */
void insert_symbol(HashTable *table, const char *name, const char *type, int intValue, int hasInt, float floatValue, int hasFloat, unsigned int address) {
   
    unsigned int index = hash(name);

    Symbol *new_symbol = malloc(sizeof(Symbol));

    if (new_symbol == NULL) {

        fprintf(stderr, "Chyba alokácie pamäte pre nový symbol\n");
        return;
    }

    new_symbol->name = copy_string(name);
    new_symbol->type = copy_string(type);
    new_symbol->intValue = intValue;
    new_symbol->hasIntValue = hasInt;
    new_symbol->floatValue = floatValue;
    new_symbol->hasFloatValue = hasFloat;
    new_symbol->address = address;
    new_symbol->next = table->table[index];
    table->table[index] = new_symbol;
}

/**
 * @brief Deletes a symbol from the hash table by name.
 * 
 * @details This function searches for a symbol in the hash table and removes 
 * it, adjusting the linked list pointers accordingly. It also frees the memory
 * allocated for the symbol's name and type.
 * 
 * @param table Pointer to the hash table.
 * @param name Name of the symbol to delete.
 */
void delete_symbol(HashTable *table, const char *name) {
   
    unsigned int index = hash(name);
    Symbol *current = table->table[index];
    Symbol *previous = NULL;
    
    while (current != NULL && strcmp(current->name, name) != 0) {

        previous = current;
        current = current->next;
    }
    
    if (current == NULL) return;
    
    if (previous == NULL) {

        table->table[index] = current->next;
    } else {

        previous->next = current->next;
    }

    free_string(current->name);
    free_string(current->type);
    free(current);
}

/**
 * @brief Searches for a symbol in the hash table by name.
 * 
 * @details This function traverses the linked list at the index corresponding
 * to the hash of the symbol's name and returns a pointer to the symbol if found,
 * or NULL if not found.
 * 
 * @param table Pointer to the hash table.
 * @param name Name of the symbol to search for.
 * @return Pointer to the Symbol if found, or NULL if not found.
 */
Symbol *search_symbol(HashTable *table, const char *name) {

    unsigned int index = hash(name);
    Symbol *current = table->table[index];

    while (current != NULL) {

        if (strcmp(current->name, name) == 0) {

            return current;
        }

        current = current->next;
    }

    return NULL;
}

/**
 * @brief Frees all memory associated with the hash table.
 * 
 * @details This function iterates through each entry in the hash table and frees
 * the memory associated with each symbol, followed by freeing the hash table itself.
 * 
 * @param table Pointer to the hash table to free.
 */
void free_table(HashTable *table) {

    for (int i = 0; i < TABLE_SIZE; i++) {

        Symbol *current = table->table[i];

        while (current != NULL) {

            Symbol *temp = current;
            current = current->next;
            free_string(temp->name);
            free_string(temp->type);
            free(temp);
        }
    }
    
    free(table);
}