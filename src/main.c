#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexical_analyser.h"
#include "syntactic_analysis.h"
#include <error.h>

FILE *file;

int main(int argc, char **argv)
{
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