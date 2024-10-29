#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexical_analyser.h"
#include "syntactic_analysis.h"
#include <error.h>
#include "ADT.h"

FILE *file;


void print_symbol(const Symbol *symbol) {
    printf("Name: %s\n", symbol->name);
    printf("Type: %s\n", symbol->type);
    if (symbol->hasIntValue) {
        printf("Integer Value: %d\n", symbol->intValue);
    }
    if (symbol->hasFloatValue) {
        printf("Float Value: %f\n", symbol->floatValue);
    }
    printf("Address: %u\n", symbol->address);
    printf("-------------------\n");
}

void test_hash_table() {
    // Inicializácia tabuľky
    HashTable *table = create_table();
    if (table == NULL) {
        printf("Chyba pri inicializácii hashovacej tabuľky.\n");
        return;
    }

    // Vloženie symbolov
    insert_symbol(table, "key1", "int", 42, 1, 0.0, 0, 1001);     // Len int hodnota
    insert_symbol(table, "key2", "float", 0, 0, 3.14, 1, 1002);   // Len float hodnota
    insert_symbol(table, "key3", "string", 0, 0, 0.0, 0, 1003);   // Bez int a float hodnôt

    // Vyhľadanie a výpis symbolov
    Symbol *result = search_symbol(table, "key1");
    if (result != NULL) {
        printf("Nájdený symbol pre 'key1':\n");
        print_symbol(result);
    } else {
        printf("Symbol pre 'key1' nebol nájdený.\n");
    }

    result = search_symbol(table, "key2");
    if (result != NULL) {
        printf("Nájdený symbol pre 'key2':\n");
        print_symbol(result);
    } else {
        printf("Symbol pre 'key2' nebol nájdený.\n");
    }

    result = search_symbol(table, "key3");
    if (result != NULL) {
        printf("Nájdený symbol pre 'key3':\n");
        print_symbol(result);
    } else {
        printf("Symbol pre 'key3' nebol nájdený.\n");
    }

    // Odstránenie symbolu a overenie
    delete_symbol(table, "key1");
    result = search_symbol(table, "key1");
    if (result == NULL) {
        printf("Symbol pre 'key1' bol úspešne odstránený.\n");
    } else {
        printf("Symbol pre 'key1' stále existuje.\n");
    }

    // Uvoľnenie pamäte tabuľky
    free_table(table);
    printf("Pamäť tabuľky bola uvoľnená.\n");
}


int main(int argc, char **argv)
{
    if(argc == 1){
        test_hash_table(); // Volanie testovacej funkcie
        return 0;
    }

    
    // Skontroluj, či bol zadaný súbor ako argument
    if (argc < 2)
        file = stdin;
    if (argc == 2)
        file = fopen(argv[1], "r");
    else
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    if (file == NULL)
        perror("Failed to open file");

    // Syntactic analysis
    // if (!FIRST(file))
    // {
    //     printf("%s", " --- WRONG END --- \n");
    //     return 1;
    // }

    // printf("%s", " --- ENDED SUCESFULLY --- \n");

    Token token;
    // Print header for clarity
    printf("Token Type\tValue\t\tKeyword Value\n");
    printf("----------------------------------------------------------\n");

    do
    {
        token = get_token(file);
        print_token(token);

        dynamic_string_free(&token.value.valueString);
    } while (token.type != TOKEN_EOF); // Continue until EOF

    fclose(file); // Nezabudni zavrieť súbor
    return EXIT_SUCCESS;
}
