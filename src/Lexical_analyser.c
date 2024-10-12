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
                char nextChar = (char)getc(file); // Look ahead
                if (nextChar == '/')              // Start of comment
                {
                    state = sComment; // Move to comment state
                }
                else // It’s division
                {
                    state = sOperator;         // Move to operator state
                    token.type = TOKEN_DIVIDE; // Set token type for division
                }
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
                token.type = TOKEN_LESS_THAN;
            }
            else if (c == '>') // Start of greater than or equal
            {
                state = sGreaterThan; // Move to greater than state
                token.type = TOKEN_GREATER_THAN;
            }
            else if (c == '‘') // netusim to co je wtf
            {
                state = sLeftSingleQuote;
            }
            else if (c == '\'') // Single quote for character literal
            {
                state = sSingleQuote;            // Move to state for single quote
                token.type = TOKEN_CHAR_LITERAL; // Set token type for character literal
            }
            else if (c == '\\') // Backslash for escape sequences
            {
                state = sBackSlash;           // Move to escape sequence handling state
                token.type = TOKEN_BACKSLASH; // Set token type for backslash
            }

            else if (c == '!')
            {
                state = sExclamation;
                token.type = TOKEN_EXCLAMATION;
            }

            break;
        case sIdentifier:
            // Handle identifier logic here
            break;

        case sKeyword:
            // Handle keyword logic here
            break;

        case sNamespace:
            // Handle namespace logic here
            break;

        case sIntLiteral:
            // Handle integer literal logic here
            break;

        case sFloatLiteral:
            // Handle floating-point literal logic here
            break;

        case sExponent:
            // Handle exponent part logic here
            break;

        case sStringLiteral:
            // Handle string literal logic here
            break;

        case sEscapeSequence:
            // Handle escape sequence logic here
            break;

        case sOperator:
            // Handle operator logic here

            break;
        case sBackSlash:
            break;

        case sAssign:
            // Handle assignment logic here
            break;

        case sEqual:
            // Handle equality check logic here
            break;

        case sNotEqual:
            // Handle not equal logic here
            break;

        case sLessEqual:
            // Handle less than or equal logic here
            break;

        case sLessThan:
            // Handle less than logic here
            break;

        case sGreaterEqual:
            // Handle greater than or equal logic here
            break;

        case sGreaterThan:
            // Handle greater than logic here
            break;

        case sLeftParen:
            // Handle left parenthesis logic here
            break;

        case sRightParen:
            // Handle right parenthesis logic here
            break;

        case sLeftBracket:
            // Handle left bracket logic here
            break;

        case sRightBracket:
            // Handle right bracket logic here
            break;

        case sComma:
            // Handle comma logic here
            break;

        case sSemicolon:
            // Handle semicolon logic here
            break;

        case sExclamation:
            // Handle exclamation logic here
            break;

        case sPipe:
            // Handle pipe logic here
            break;

        case sSingleQuote:
            // Handle single quote logic here
            break;

        case sLeftSingleQuote:
            // Handle left single quote logic here
            break;

        case sWhitespace:
            // Handle whitespace logic here
            break;

        case sComment:
            while (c != '\n' && c != EOF) // Consume until end of line
            {
                c = (char)getc(file);
                //teraz co? kde to ukladam
            }
            state = sStart; // Return to starting state after comment

            break;

        case sError:
            // Handle error logic here
            break;

        case sEOL:
            // Handle end of line logic here
            break;

        case sEnd:
            // Handle end of file/input logic here
            break;

        default:
            state = sError; // Invalid state fallback
            break;
        }
    }
}