#include <Lexical_analyser.h>
#include <error.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

Token get_token(FILE *file)
{
    Token token;
    State state;

    state = sStart;
    token.type = sStart;

    char c;

    while (1)
    {
        c = (char)getc(file);

        switch (state)
        {
        case sStart:
            if (c == EOF)
            {
                token.type = TOKEN_EOF;
                return token;
            }
            else if (c == ' ' || c == '\t')
            {
                state = sStart;
            }
            else if (c == '\n')
            {
                // logika pre novy riadok
            }
            else if (isalpha(c) || c == '_') // Check for identifiers/keywords
            {
                state = sIdentifier;
            }
            else if (isdigit(c)) // Check for numbers
            {
                state = sIntLiteral; // Start reading an integer literal
            }
            else if (c == '\"') // Start of string literal
            {
                state = sStringLiteral;
            }
            else if (c == '/') // Could be start of comment or division
            {
                state = sOperator; // Temporarily move to operator state
                // Read the next character to check if it's part of a comment
            }
            else if (c == '(') // Left parenthesis
            {
                token.type = TOKEN_LPAREN;
                return token;
            }
            else if (c == ')') // Right parenthesis
            {
                token.type = TOKEN_RPAREN;
                return token;
            }
            else if (c == ',') // Comma
            {
                token.type = TOKEN_COMMA;
                return token;
            }
            else if (c == ';') // Semicolon
            {
                token.type = TOKEN_SEMICOLON;
                return token;
            }
            else if (c == '+') // Addition operator
            {
                token.type = TOKEN_ADDITION;
                return token;
            }
            else if (c == '-') // Subtraction operator
            {
                token.type = TOKEN_SUBTRACTION;
                return token;
            }
            else if (c == '*') // Multiplication operator
            {
                token.type = TOKEN_MULTIPLY;
                return token;
            }
            else if (c == '/') // Division operator
            {
                token.type = TOKEN_DIVIDE;
                return token;
            }
            else if (c == '=') // Start of equality or assignment
            {
                state = sAssign; // Move to assignment state
            }
            else if (c == '<') // Start of less than or equal
            {
                state = sLessThan; // Move to less than state
            }
            else if (c == '>') // Start of greater than or equal
            {
                state = sGreaterThan; // Move to greater than state
            }

            break;
        case sIdentifier:
            // Logic to read and complete identifier (check against keywords)
            break;

        case sKeyword:
            // Logic to handle keywords (check against the keyword list)
            break;

        case sIntLiteral:
            // Logic to read integer literals, ensuring no leading zeros
            break;

        case sFloatLiteral:
            // Logic to read floating-point literals, including exponent
            break;

        case sStringLiteral:
            // Logic to read string literals, handle escape sequences
            break;

        case sComment:
            // Logic to handle comments (ignore until the end of the line)
            break;

        case sOperator:
            // Logic to handle additional operators (handle cases like ==, !=, etc.)
            break;

        case sEqual:

            break;
        case sNotEqual:
            // Logic to handle not equal (!=)
            break;

        case sLessThan:
            break;

        case sGreaterThan:
            break;

        default:
            break;
        }
    }
}