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
                state = TOKEN_EOF;
                token.type = TOKEN_EOF;
            }
            else if (c == ' ' || c == '\t')
            {
                state = sStart;
            }
            else if (c == '\n') // EOL, end of line
            {
                state = sEOL;
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
            }
            else if (c == '(') // Left parenthesis
            {
                state = sLeftParen;
            }
            else if (c == ')') // Right parenthesis
            {
                state = sRightParen;
            }
            else if (c == ',') // Comma
            {
                state = sComma;
            }
            else if (c == ';') // Semicolon
            {
                state = sSemicolon;
            }
            else if (c == '+') // Addition operator
            {
                state = sOperator;
                token.type = TOKEN_ADDITION;
            }
            else if (c == '-') // Subtraction operator
            {
                state = sOperator;
                token.type = TOKEN_SUBTRACTION;
            }
            else if (c == '*') // Multiplication operator
            {
                state = sOperator;
                token.type = TOKEN_MULTIPLY;
            }
            else if (c == '/') // Division operator or comment start
            {
                state = sBacklash;
                token.type = TOKEN_BACKLASH;
            }
            else if (c == '|') // Check for bitwise OR / logical OR
            {
                state = sPipe; // Transition to pipe state
                token.type = TOKEN_PIPE;
            }
            else if (c == '=') // Start of equality or assignment
            {
                state = sAssign; // Move to assignment state
                token.type = TOKEN_ASSIGNMENT;
            }
            else if (c == '<') // Start of less than or equal
            {
                state = sLessThan; // Move to less than state
                token.type = sLessThan;
            }
            else if (c == '>') // Start of greater than or equal
            {
                state = sGreaterThan; // Move to greater than state
                token.type = sGreaterThan;
            }
            else if (c == '‘') // netusim to co je wtf
            {
                state = sLeftSingleQuote;
            }
            else if (c == '\'')
            {
                state = sSingleQuote;
                token.type = TOKEN_ESCAPE;
            }
            else if (c == '!')
            {
                state = sExclamation;
                token.type = TOKEN_EXCLAMATION;
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