#ifndef ADT_H
#define ADT_H

#define TABLE_SIZE 100  // Veľkosť hashovacej tabuľky

// Štruktúra pre záznam v hashovacej tabuľke (symbol)
typedef struct Symbol {
    char *name;                   // Kľúč symbolu
    char *type;                   // Typ symbolu
    int intValue;                 // Číselná hodnota
    float floatValue;             // Desatinná hodnota
    unsigned int address;         // Adresa symbolu
    unsigned int hasIntValue : 1; // Flag: 1 ak intValue je platné
    unsigned int hasFloatValue : 1; // Flag: 1 ak floatValue je platné
    struct Symbol *next;          // Ukazovateľ na ďalší symbol (pre kolízie)
} Symbol;


typedef struct HashTable {
    Symbol *table[TABLE_SIZE];  // Pole ukazovateľov na symboly
} HashTable;

char *copy_string(const char *str);

void free_string(char *str);

unsigned int hash(const char *key)   ;                 // Hashovacia funkcia

HashTable *create_table();                       // Vytvorenie novej hashovacej tabuľky

void insert_symbol(HashTable *table, const char *name, const char *type, int intValue, int hasInt, float floatValue, int hasFloat, unsigned int address);  // Vloženie symbolu do tabuľky

void delete_symbol(HashTable *table, const char *name);

Symbol *search_symbol(HashTable *table, const char *name);  // Vyhľadanie symbolu podľa kľúča

void free_table(HashTable *table);               // Uvoľnenie pamäte hashovacej tabuľky
#endif
