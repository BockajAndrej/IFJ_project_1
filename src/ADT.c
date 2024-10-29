#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ADT.h"


char *copy_string(const char *str) {
    char *copy = malloc(strlen(str) + 1);
    if (copy != NULL) {
        strcpy(copy, str);
    }
    return copy;
}

void free_string(char *str) {
    free(str);
}

// Hashovacia funkcia
unsigned int hash(const char *key) {
    unsigned int hash_value = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hash_value = 31 * hash_value + key[i];
    }
    return hash_value % TABLE_SIZE;
}

// Funkcia na vytvorenie novej hashovacej tabuľky
HashTable *create_table() {
    HashTable *table = malloc(sizeof(HashTable));
    if (table) {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table->table[i] = NULL;
        }
    }
    return table;
}

// Funkcia na vloženie symbolu do tabuľky
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
    new_symbol->hasIntValue = hasInt;       // Nastaví sa, či je int hodnota platná
    new_symbol->floatValue = floatValue;
    new_symbol->hasFloatValue = hasFloat;   // Nastaví sa, či je float hodnota platná
    new_symbol->address = address;
    new_symbol->next = table->table[index];
    table->table[index] = new_symbol;
}

// Funkcia na zmazanie symbolu z tabuľky
void delete_symbol(HashTable *table, const char *name) {
    unsigned int index = hash(name);
    Symbol *current = table->table[index];
    Symbol *previous = NULL;
    
    while (current != NULL && strcmp(current->name, name) != 0) {
        previous = current;
        current = current->next;
    }
    
    if (current == NULL) return;  // Symbol sa nenašiel
    
    if (previous == NULL) {
        table->table[index] = current->next;
    } else {
        previous->next = current->next;
    }

    free_string(current->name);
    free_string(current->type);
    free(current);
}

// Funkcia na vyhľadanie symbolu
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

// Funkcia na uvoľnenie pamäte hashovacej tabuľky
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