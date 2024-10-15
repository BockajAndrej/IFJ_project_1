#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexical_analyser.h"
#include <error.h>
FILE *file;

int main(int argc, char **argv)
{
    // Skontroluj, či bol zadaný súbor ako argument
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    }

    // Otvor súbor na čítanie
    file = fopen(argv[1], "r");
    if (file == NULL)
    {
        perror("Failed to open file");
    }

    Token token;

    // Spustí sa cyklus, ktorý načíta a zobrazuje tokeny, kým nedôjde k EOF
    do
    {
        token = get_token(file); // Získaj token
        switch (token.type)
        {
        case TOKEN_EOF:
            printf("Token: EOF\n");
            break;

        case TOKEN_EOL:
            printf("Token: EOL\n");
            break;

        case TOKEN_EMPTY:
            printf("Token: EMPTY\n");
            break;

        case TOKEN_IDENTIFIER:
            printf("Token: %d, Value: %s\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_KEYWORD:
            printf("Token: %d, Value: %s\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_NEWLINE:
            printf("Token: NEWLINE\n");
            break;

        case TOKEN_TAB:
            printf("Token: TAB\n");
            break;

        case TOKEN_INT_LITERAL:
            printf("Token: INT_LITERAL, Value: %d\n", token.value.intValue);
            break;

        case TOKEN_FLOAT_LITERAL:
            printf("Token: FLOAT_LITERAL, Value: %f\n", token.value.floatValue);
            break;

        case TOKEN_STRING_LITERAL:
            printf("Token: STRING_LITERAL, Value: %s\n", token.value.valueString.str);
            break;

        case TOKEN_BOOLEAN:
            printf("Token: BOOLEAN, Value: %d\n", token.value.boolValue);
            break;

        case TOKEN_CHAR_LITERAL:
                 // Předpokládáme, že máš charValue v unii
            break;

        case TOKEN_EQUAL:
            printf("Token: EQUAL\n");
            break;

        case TOKEN_NOT_EQUAL:
            printf("Token: NOT_EQUAL\n");
            break;

        case TOKEN_LESS_EQUAL:
            printf("Token: LESS_EQUAL\n");
            break;

        case TOKEN_LESS_THAN:
            printf("Token: LESS_THAN\n");
            break;

        case TOKEN_GREATER_EQUAL:
            printf("Token: GREATER_EQUAL\n");
            break;

        case TOKEN_GREATER_THAN:
            printf("Token: GREATER_THAN\n");
            break;

        case TOKEN_ASSIGNMENT:
            printf("Token: ASSIGNMENT\n");
            break;

        case TOKEN_ADDITION:
            printf("Token: ADDITION\n");
            break;

        case TOKEN_SUBTRACTION:
            printf("Token: SUBTRACTION\n");
            break;

        case TOKEN_MULTIPLY:
            printf("Token: MULTIPLY\n");
            break;

        case TOKEN_DIVIDE:
            printf("Token: DIVIDE\n");
            break;

        case TOKEN_BACKSLASH:
            printf("Token: BACKSLASH\n");
            break;

        case TOKEN_PIPE:
            printf("Token: PIPE\n");
            break;

        case TOKEN_EXCLAMATION:
            printf("Token: EXCLAMATION\n");
            break;

        case TOKEN_LPAREN:
            printf("Token: LPAREN\n");
            break;

        case TOKEN_RPAREN:
            printf("Token: RPAREN\n");
            break;

        case TOKEN_LEFT_BRACKET:
            printf("Token: LEFT_BRACKET\n");
            break;

        case TOKEN_RIGHT_BRACKET:
            printf("Token: RIGHT_BRACKET\n");
            break;

        case TOKEN_COMMA:
            printf("Token: COMMA\n");
            break;

        case TOKEN_SEMICOLON:
            printf("Token: SEMICOLON\n");
            break;

        default:
            printf("Token: Unknown type\n");
            break;
        }

        // Uvoľni dynamický reťazec pred ďalším cyklom
        dynamic_string_free(&token.value.valueString);
    } while (token.type != TOKEN_EOF); // Pokračuj, kým nedôjde k EOF

    fclose(file); // Nezabudni zavrieť súbor
    return EXIT_SUCCESS;
}