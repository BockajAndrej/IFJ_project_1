#ifndef ADT_H
#define ADT_H


typedef enum ValueType {
    VALUE_INT,
    VALUE_FLOAT,
    VALUE_STRING
} ValueType;


typedef struct Symbol {
    char *name;           
    ValueType type;       
    union {
        int intValue;     
        float floatValue; 
        char *strValue;   
    } value;
    struct Symbol *next;  
} Symbol;


typedef struct SymbolTable {
    Symbol *head;        
} SymbolTable;


char *copy_string(const char *str);
void free_string(char *str);

SymbolTable *create_table();
void insert_symbol(SymbolTable *table, const char *name, ValueType type, void *value);
Symbol *search_symbol(SymbolTable *table, const char *name);
void delete_symbol(SymbolTable *table, const char *name);
void free_table(SymbolTable *table);

#endif
