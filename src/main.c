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


void print_table(SymbolTable *table) {
    Symbol *current = table->head;

    printf("Current symbol table:\n");
    while (current) {
        printf(" - Name: %s, ", current->name);

        switch (current->type) {
            case VALUE_INT:
                printf("Value: %d (int)\n", current->value.intValue);
                break;
            case VALUE_FLOAT:
                printf("Value: %.2f (float)\n", current->value.floatValue);
                break;
            case VALUE_STRING:
                printf("Value: %s (string)\n", current->value.strValue);
                break;
        }

        current = current->next;
    }
    printf("\n");
}


void test_hash_table() {
    SymbolTable *table = create_table();
    printf("After creating the table:\n");
    print_table(table);

    int intValue = 10;
    float floatValue = 3.14;
    char *strValue = "Hello";

    insert_symbol(table, "x", VALUE_INT, &intValue);
    printf("After inserting 'x':\n");
    print_table(table);

    insert_symbol(table, "pi", VALUE_FLOAT, &floatValue);
    printf("After inserting 'pi':\n");
    print_table(table);

    insert_symbol(table, "greeting", VALUE_STRING, strValue);
    printf("After inserting 'greeting':\n");
    print_table(table);

    Symbol *s = search_symbol(table, "pi");
    if (s && s->type == VALUE_FLOAT) {
        printf("Found: %s -> %.2f\n", s->name, s->value.floatValue);
    }

    delete_symbol(table, "x");
    printf("After deleting 'x':\n");
    print_table(table);

    free_table(table);
}



int main(int argc, char **argv)
{
    if(argc == 1){
        test_hash_table();
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
    if (!FIRST(file))
    {
        printf("%s", " --- WRONG END --- \n");
        return 1;
    }

    printf("%s", " --- ENDED SUCESFULLY --- \n");

    // Token token;
    // // Print header for clarity
    // printf("Token Type\tValue\t\tKeyword Value\n");
    // printf("----------------------------------------------------------\n");

    // do
    // {
    //     token = get_token(file);
    //     print_token(token);

    //     dynamic_string_free(&token.value.valueString);
    // } while (token.type != TOKEN_EOF); // Continue until EOF

    fclose(file); // Nezabudni zavrieť súbor
    return EXIT_SUCCESS;
}
