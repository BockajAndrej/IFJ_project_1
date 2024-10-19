#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexical_analyser.h"
#include <error.h>
FILE *file;

#include <stdio.h>
#include <string.h>

const char *token_type_to_string(Token_type type)
{
    switch (type)
    {
    case TOKEN_EOF:
        return "TOKEN_EOF";
    case TOKEN_EOL:
        return "TOKEN_EOL";
    case TOKEN_EMPTY:
        return "TOKEN_EMPTY";
    case TOKEN_IDENTIFIER:
        return "TOKEN_IDENTI";
    case TOKEN_KEYWORD:
        return "TOKEN_KEYWORD";
    case TOKEN_NEWLINE:
        return "TOKEN_NEWLINE";
    case TOKEN_TAB:
        return "TOKEN_TAB";
    case TOKEN_INT_LITERAL:
        return "TOKEN_INT_L";
    case TOKEN_FLOAT_LITERAL:
        return "TOKEN_FLOAT_L";
    case TOKEN_STRING_LITERAL:
        return "TOKEN_STRING_L";
    case TOKEN_BOOLEAN:
        return "TOKEN_BOOLEAN";
    case TOKEN_CHAR_LITERAL:
        return "TOKEN_CHAR_L";
    case TOKEN_EQUAL:
        return "TOKEN_EQUAL";
    case TOKEN_NOT_EQUAL:
        return "TOKEN_NOT_EQUAL";
    case TOKEN_LESS_EQUAL:
        return "TOKEN_LESS_EQUAL";
    case TOKEN_LESS_THAN:
        return "TOKEN_LESS_THAN";
    case TOKEN_GREATER_EQUAL:
        return "TOKEN_GREATER_EQ";
    case TOKEN_GREATER_THAN:
        return "TOKEN_GREATER_TH";
    case TOKEN_ASSIGNMENT:
        return "TOKEN_ASSIGN";
    case TOKEN_ADDITION:
        return "TOKEN_ADDITION";
    case TOKEN_SUBTRACTION:
        return "TOKEN_SUBTRAC";
    case TOKEN_MULTIPLY:
        return "TOKEN_MULTIPLY";
    case TOKEN_DIVISION:
        return "TOKEN_DIVISION";
    case TOKEN_BACKSLASH:
        return "TOKEN_BACKSLASH";
    case TOKEN_PIPE:
        return "TOKEN_PIPE";
    case TOKEN_EXCLAMATION:
        return "TOKEN_EXCLAMATION";
    case TOKEN_LPAREN:
        return "TOKEN_LPAREN";
    case TOKEN_RPAREN:
        return "TOKEN_RPAREN";
    case TOKEN_LEFT_BRACKET:
        return "TOKEN_LEFT_BR";
    case TOKEN_RIGHT_BRACKET:
        return "TOKEN_RIGHT_BR";
    case TOKEN_CURLYL_BRACKET:
        return "TOKEN_CURLYL_BR";
    case TOKEN_CURLYR_BRACKET:
        return "TOKEN_CURLYR_BR";
    case TOKEN_COMMA:
        return "TOKEN_COMMA";
    case TOKEN_SEMICOLON:
        return "TOKEN_SEMICOLON";
    case TOKEN_COMMENT:
        return "TOKEN_COMMENT";
    case TOKEN_UNDEFINED:
        return "TOKEN_UNDEFINED";
    case TOKEN_IMPORT:
        return "TOKEN_IMPORT";
    case TOKEN_DISCARD:
        return "TOKEN_DISCARD";
    case TOKEN_COLON:
        return "TOKEN_COLON";
    case TOKEN_DOT:
        return "TOKEN_DOT";
    default:
        return "UNKNOWN_TOKEN";
    }
}

void print_token(Token token)
{

    switch (token.type)
    {
    case TOKEN_EOF:
        printf("EOF\n");
        break;

    case TOKEN_EOL:
        printf("EOL\n");
        break;

    case TOKEN_EMPTY:
        printf("EMPTY\n");
        break;

    case TOKEN_NEWLINE:
        printf("NEWLINE\n");
        break;

    case TOKEN_TAB:
        printf("TAB\n");
        break;

    case TOKEN_INT_LITERAL:
    case TOKEN_FLOAT_LITERAL:
    case TOKEN_STRING_LITERAL:
    case TOKEN_BOOLEAN:
    case TOKEN_CHAR_LITERAL:
    case TOKEN_IDENTIFIER:
    case TOKEN_KEYWORD:
    case TOKEN_EQUAL:
    case TOKEN_NOT_EQUAL:
    case TOKEN_LESS_EQUAL:
    case TOKEN_LESS_THAN:
    case TOKEN_GREATER_EQUAL:
    case TOKEN_GREATER_THAN:
    case TOKEN_ASSIGNMENT:
    case TOKEN_ADDITION:
    case TOKEN_SUBTRACTION:
    case TOKEN_MULTIPLY:
    case TOKEN_DIVISION:
    case TOKEN_BACKSLASH:
    case TOKEN_PIPE:
    case TOKEN_EXCLAMATION:
    case TOKEN_LPAREN:
    case TOKEN_RPAREN:
    case TOKEN_LEFT_BRACKET:
    case TOKEN_RIGHT_BRACKET:
    case TOKEN_CURLYL_BRACKET:
    case TOKEN_CURLYR_BRACKET:
    case TOKEN_COMMA:
    case TOKEN_SEMICOLON:
    case TOKEN_COMMENT:
    case TOKEN_IMPORT:
    case TOKEN_DISCARD:
    case TOKEN_DOT:
    case TOKEN_COLON:
        printf("%s\t\t%s\t\t%d\n", token_type_to_string(token.type), token.value.valueString.str, token.keyword_val);
        break;

    case TOKEN_UNDEFINED:
        printf("UNDEFINED\t%s\t\t-\n", token.value.valueString.str);
        break;

    default:
        printf("Unknown Type\t-\t\t-\t\n");
        break;
    }
}

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