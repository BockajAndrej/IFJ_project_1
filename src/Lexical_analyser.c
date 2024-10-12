#include <Lexical_analyser.h>
#include <error.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

Token get_token(FILE *file)
{
    if (file == NULL)
        return; // edit later

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
            }
            else if (c == '<') // Start of less than or equal
            {
                state = sLessThan; // Move to less than state
            }
            else if (c == '>') // Start of greater than or equal
            {
                state = sGreaterThan; // Move to greater than state
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

            else if (c == '!') // Logical NOT or not equal
            {
                // Look ahead to the next character
                c = (char)getc(file);
                if (c == '=')
                {                                 // If the next character is '='
                    token.type = TOKEN_NOT_EQUAL; // Token is '!='
                }
                else
                {
                    ungetc(c, file);                // Put back the character if it's not '='
                    token.type = TOKEN_EXCLAMATION; // Token is just '!'
                }
                state = sStart; // Return to the starting state
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
            c = (char)getc(file); // Read the next character
            if (c == 'n')
            {
                token.type = TOKEN_NEWLINE; // Example for newline escape
            }
            else if (c == 't')
            {
                token.type = TOKEN_TAB; // Example for tab escape
            }
            else if (c == '\\')
            {
                token.type = TOKEN_BACKSLASH; // Token for backslash itself
            }
            else
            {
                ungetc(c, file);              // Put back the character if it's not a recognized escape
                token.type = TOKEN_BACKSLASH; // Handle as just a backslash if needed
            }
            state = sStart; // Return to the starting state
            break;

        case sAssign:
            c = (char)getc(file); // Read the next character
            if (c == '=')
            {
                token.type = TOKEN_EQUAL; // Token is '==' (equality)
            }
            else
            {
                ungetc(c, file);               // Put back the character if it's not '='
                token.type = TOKEN_ASSIGNMENT; // Token is just '=' (assignment)
            }
            state = sStart; // Return to the starting state
            break;

        case sLessThan:
            c = (char)getc(file); // Read the next character
            if (c == '=')
            {
                token.type = TOKEN_LESS_EQUAL; // Token is '<='
            }
            else
            {
                ungetc(c, file);              // Put back the character if it's not '='
                token.type = TOKEN_LESS_THAN; // Token is just '<'
            }
            state = sStart; // Return to the starting state
            break;

        case sGreaterThan:
            c = (char)getc(file); // Read the next character
            if (c == '=')
            {
                token.type = TOKEN_GREATER_EQUAL; // Token is '>='
            }
            else
            {
                ungetc(c, file);                 // Put back the character if it's not '='
                token.type = TOKEN_GREATER_THAN; // Token is just '>'
            }
            state = sStart; // Return to the starting state
            break;

        case sLeftParen:
            token.type = TOKEN_LPAREN; // Set the token type for left parenthesis
            state = sStart;            // Return to the starting state
            break;

        case sRightParen:
            token.type = TOKEN_RPAREN; // Set the token type for right parenthesis
            state = sStart;            // Return to the starting state
            break;

        case sLeftBracket:
            token.type = TOKEN_LEFT_BRACKET; // Set the token type for left bracket
            state = sStart;                  // Return to the starting state
            break;

        case sRightBracket:
            token.type = TOKEN_RIGHT_BRACKET; // Set the token type for right bracket
            state = sStart;                   // Return to the starting state
            break;

        case sComma:
            token.type = TOKEN_COMMA; // Set the token type for comma
            state = sStart;           // Return to the starting state
            break;

        case sSemicolon:
            token.type = TOKEN_SEMICOLON; // Set the token type for semicolon
            state = sStart;               // Return to the starting state
            break;

        case sPipe:
            // Handle  "|""  pipe logic here
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
                // teraz co? kde to ukladam
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
        return token;
    }
}