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
            printf("Token: %d  EOF=0\n", token.type);
            break;

        case TOKEN_EOL:
            printf("Token: %d  EOL=1\n", token.type);
            break;

        case TOKEN_EMPTY:
            printf("Token: EMPTY=2\n");
            break;

        case TOKEN_IDENTIFIER:
            printf("Token: %d, Value: %s\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_KEYWORD:
            printf("Token: %d, Value: %s\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_NEWLINE:
            printf("Token: %d  NEWLINE\n", token.type);
            break;

        case TOKEN_TAB:
            printf("Token: %d  TAB\n", token.type);
            break;

        case TOKEN_INT_LITERAL:
            printf("Token: INT_LITERAL 7=%d , Value: %s\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_FLOAT_LITERAL:
            printf("Token: FLOAT_LITERAL 8=%d, Value: %s\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_STRING_LITERAL:
            printf("Token: STRING_LITERAL, Value: %s\n", token.value.valueString.str);
            break;

        case TOKEN_BOOLEAN:
            printf("Token: BOOLEAN, Value: %d\n", token.value.boolValue);
            break;

        case TOKEN_CHAR_LITERAL:
            break;

        case TOKEN_EQUAL:
            printf("Token: %d  EQUAL [%s]\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_NOT_EQUAL:
            printf("Token: %d  NOT EQUAL [%s]\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_LESS_EQUAL:
            printf("Token: %d  LESS_EQUAL [%s]\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_LESS_THAN:
            printf("Token: %d  LESS THAN [%s]\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_GREATER_EQUAL:
            printf("Token: %d  GREATER EQUAL [%s]\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_GREATER_THAN:
            printf("Token: %d  GREATER THAN [%s]\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_ASSIGNMENT:
            printf("Token: %d  ASSIGNMENT [%s]\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_ADDITION:
            printf("Token: %d  ADDITION [%s]\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_SUBTRACTION:
            printf("Token: %d  SUBTRACTION [%s]\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_MULTIPLY:
            printf("Token: %d  MULTIPLY [%s]\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_DIVISION:
            printf("Token: %d  DIVIDE [%s]\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_BACKSLASH:
            printf("Token: %d  BACKSLASH [%s]\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_PIPE:
            printf("Token: %d  PIPE [%s]\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_EXCLAMATION:
            printf("Token: %d  EXCLAMATION [%s]\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_LPAREN:
            printf("Token: %d  LPAREN [%s]\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_RPAREN:
            printf("Token: %d  RPAREN [ %s ]\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_LEFT_BRACKET:
            printf("Token: %d  LEFT BRACKET [ %s ]\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_RIGHT_BRACKET:
            printf("Token: %d  RIGHT BRACKET [ %s ]\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_CURLYL_BRACKET:
            printf("Token: %d  LEFT CURLY BRACKET [ %s ]\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_CURLYR_BRACKET:
            printf("Token: %d  RIGHT CURLY BRACKET [ %s ]\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_COMMA:
            printf("Token: %d  COMMA [%s]\n", token.type, token.value.valueString.str);
            break;

        case TOKEN_SEMICOLON:
            printf("Token: %d  SEMICOLON [%s]\n", token.type, token.value.valueString.str);
            break;
        case TOKEN_COMMENT:
            printf("Token: %d  comment IS \"%s\"\n", token.type, token.value.valueString.str);
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