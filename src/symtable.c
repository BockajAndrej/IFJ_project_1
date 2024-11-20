#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"



char *copy_string(const char *str) {
    if (str == NULL) return NULL; 

    char *copy = malloc(strlen(str) + 1);
    if (copy) { 
        strcpy(copy, str); 
    }
    return copy; 
}

void free_string(char *str) {
    if (str != NULL) { 
        free(str);     
    }
}



SymbolTable *create_table() {
    SymbolTable *table = malloc(sizeof(SymbolTable));
    if (table) {
        table->head = NULL; 
    }
    return table;
}


void insert_symbol(SymbolTable *table, int level, const char *name, ValueType type, void *value) {
    Symbol *new_symbol = malloc(sizeof(Symbol));
    if (!new_symbol) {
        fprintf(stderr, "Chyba alokácie pamäte pre symbol\n");
        return;
    }

    new_symbol->name = copy_string(name);
    new_symbol->type = type;   
    new_symbol->level = level;      


    switch (type) {
        case VALUE_INT:
            new_symbol->value.intValue = *(int *)value;
            break;
        case VALUE_FLOAT:
            new_symbol->value.floatValue = *(float *)value;
            break;
        case VALUE_STRING:
            new_symbol->value.strValue = copy_string((char *)value);
            break;
    }

    new_symbol->next = table->head;
    table->head = new_symbol;
}


Symbol *search_symbol(SymbolTable *table, const char *name) {
    Symbol *current = table->head;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current; 
        }
        current = current->next;
    }
    return NULL; 
}


void delete_symbol(SymbolTable *table, const char *name) {
    Symbol *current = table->head;
    Symbol *previous = NULL;

    while (current && strcmp(current->name, name) != 0) {
        previous = current;
        current = current->next;
    }

    if (!current) return; 

    if (previous) {
        previous->next = current->next;
    } else {
        table->head = current->next;
    }

    free_string(current->name);
    if (current->type == VALUE_STRING) {
        free(current->value.strValue); 
    }
    free(current);
}


void free_table(SymbolTable *table) {
    Symbol *current = table->head;
    while (current) {
        Symbol *temp = current;
        current = current->next;

        free(temp->name);
        if (temp->type == VALUE_STRING) {
            free(temp->value.strValue);
        }
        free(temp);
    }
    free(table);
}
