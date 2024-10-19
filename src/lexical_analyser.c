#include "lexical_analyser.h"
#include <error.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <string.h>
#include "newstring.h"

#define KEYWORD_COUNT 13

const char *keywords[KEYWORD_COUNT] = {
    "const", "else", "fn", "if", "i32",
    "f64", "null", "pub", "return", "u8",
    "var", "void", "while"};

// Funkce pro kontrolu, zda je identifikátor klíčové slovo
bool is_keyword(const char *identifier)
{
    for (int i = 0; i < KEYWORD_COUNT; i++)
    {
        if (strcmp(identifier, keywords[i]) == 0)
        {
            return true; // Je to klíčové slovo
        }
    }
    return false; // Není to klíčové slovo
}

Token get_token(FILE *file)
{
    if (file == NULL)
    {
        printf("file null");
    }

    Token token;
    State state;

    state = sStart;

    // Inicializuj dynamic_string pre hodnotu tokenu
    if (!dynamic_string_init(&token.value.valueString))
    {
        token.type = TOKEN_EOF;
        return token; // Vráť chybový token
    }

    char c;

    while (1)
    {
        c = (char)getc(file);

        if (c == EOF)
        {
            // state = sEnd;
            dynamic_string_free(&token.value.valueString);
            token.type = TOKEN_EOF;
            return token;
        }

        switch (state)
        {
        case sStart:

            if (c == ' ' || c == '\t')
            {
                state = sStart;
            }
            else if (c == '@')
            {
                state = sImport;
                dynamic_string_add_char(&token.value.valueString, c);
            }
            else if (c == '\n') /* EOL, end of line  should add \t \r support? */
            {
                state = sEOL;           // Přepni na stav konce řádku
                token.type = TOKEN_EOL; // Předpokládáme, že TOKEN_EOL je definován
                return token;           // Vrať token EOL
            }
            else if (isalpha(c)) // Check for identifiers/keywords
            {
                state = sIdentifierorKeyword;
                dynamic_string_add_char(&token.value.valueString, c);
            }
            else if (c == '_')
            {
                dynamic_string_add_char(&token.value.valueString, c);

                // Read the next character without shadowing the original 'c'
                char next_c = (char)getc(file);

                // If the next character is a letter or a digit, treat it as part of an identifier
                if (isalnum(next_c))
                {
                    // Add underscore to the identifier
                    dynamic_string_add_char(&token.value.valueString, next_c); // Add the letter/digit to the identifier
                    state = sIdentifierorKeyword;                              // Transition to identifier state
                }
                else
                {
                    // If the next character is not valid, discard the underscore
                    if (next_c == '\n' || next_c == EOF || next_c == ' ')
                    {
                        token.type = TOKEN_DISCARD; // Mark as discard
                        return token;
                    }
                    else
                    {
                        token.type = TOKEN_UNDEFINED; // Invalid token
                        ungetc(next_c, file);         // Return the character back to the input
                        return token;
                    }
                }
            }
            else if (isdigit(c)) // Check for numbers
            {
                state = sIntLiteral; // Start reading an integer literal
                dynamic_string_add_char(&token.value.valueString, c);
                // ungetc(c,file);
            }
            else if (c == '\"') // Start of string literal
            {
                state = sStringLiteral;
            }
            else if (c == '/') // Could be start of comment or division
            {
                dynamic_string_add_char(&token.value.valueString, c);
                state = sDivision;
            }
            else if (c == '(') // Left parenthesis
            {
                dynamic_string_add_char(&token.value.valueString, c);
                token.type = TOKEN_LPAREN;
                return token;
            }
            else if (c == ')') // Right parenthesis
            {
                dynamic_string_add_char(&token.value.valueString, c);
                token.type = TOKEN_RPAREN;
                return token;
            }
            else if (c == '{') // Left parenthesis
            {
                dynamic_string_add_char(&token.value.valueString, c);
                token.type = TOKEN_CURLYL_BRACKET;
                return token;
            }
            else if (c == '}') // Right parenthesis
            {
                dynamic_string_add_char(&token.value.valueString, c);
                token.type = TOKEN_CURLYR_BRACKET;
                return token;
            }
            else if (c == '[')
            {
                dynamic_string_add_char(&token.value.valueString, c);
                token.type = TOKEN_LEFT_BRACKET;
                return token;
            }
            else if (c == ']')
            {
                dynamic_string_add_char(&token.value.valueString, c);
                token.type = TOKEN_RIGHT_BRACKET;
                return token;
            }
            else if (c == ',') // Comma
            {
                token.type = TOKEN_COMMA; // Set the token type for comma
                dynamic_string_add_char(&token.value.valueString, c);
                return token;
            }
            else if (c == ';') // Semicolon
            {
                dynamic_string_add_char(&token.value.valueString, c);
                token.type = TOKEN_SEMICOLON; // Set the token type for semicolon
                state = sStart;               // Return to the starting state
                return token;
            }
            else if (c == '+') // Addition operator
            {
                dynamic_string_add_char(&token.value.valueString, c);
                token.type = TOKEN_ADDITION;
                return token;
            }
            else if (c == '-') // Subtraction operator
            {
                dynamic_string_add_char(&token.value.valueString, c);
                token.type = TOKEN_SUBTRACTION;
                return token;
            }
            else if (c == '*') // Multiplication operator
            {
                dynamic_string_add_char(&token.value.valueString, c);
                token.type = TOKEN_MULTIPLY;
                return token;
            }
            else if (c == '|') // Check for bitwise OR / logical OR
            {
                dynamic_string_add_char(&token.value.valueString, c);
                token.type = TOKEN_PIPE;
                return token;
            }
            else if (c == '=') // Start of equality or assignment
            {
                state = sAssign; // Move to assignment state
                dynamic_string_add_char(&token.value.valueString, c);
            }
            else if (c == '<') // Start of less than or equal
            {
                state = sLessThan; // Move to less than state
                dynamic_string_add_char(&token.value.valueString, c);
            }
            else if (c == '>') // Start of greater than or equal
            {
                state = sGreaterThan; // Move to greater than state
                dynamic_string_add_char(&token.value.valueString, c);
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
                dynamic_string_add_char(&token.value.valueString, c);
                state = sExclamation;
            }

            break;

        case sIdentifierorKeyword:
        {
            // Počkáme na další znak, dokud nezjistíme, že je identifikátor kompletní
            while (isalpha(c) || isdigit(c) || c == '_')
            {
                // Přidáme znak do dynamického řetězce
                dynamic_string_add_char(&token.value.valueString, c);
                c = (char)getc(file); // Načti další znak
            }

            // Ukončujeme identifikátor
            ungetc(c, file); // Vrať znak zpět do vstupu

            // Zkontroluj, zda je identifikátor klíčové slovo
            if (is_keyword(token.value.valueString.str))
                token.type = TOKEN_KEYWORD; // Nastav typ tokenu na klíčové slovo
            else
                token.type = TOKEN_IDENTIFIER; // Nastav typ tokenu na identifikátor

            // Pokud je to identifikátor, můžeme vrátit token
            return token;
        }
        break;
        case sIdentifier:
        {
            // Zpracuj logiku pro identifikátory zde
            // Například: Přidávej znaky, dokud nebudou považovány za konec
            while (isalpha(c) || isdigit(c) || c == '_')
            {
                // Přidáme znak do dynamického řetězce
                dynamic_string_add_char(&token.value.valueString, c);
                c = (char)getc(file); // Načti další znak
            }
            ungetc(c, file); // Vrať znak zpět do vstupu

            token.type = TOKEN_IDENTIFIER; // Nastav typ tokenu na identifikátor
            return token;                  // Vrať token
        }
        break;
        case sIntLiteral:
        {
            // Handle integer literal logic here
            while (isdigit(c))
            {
                dynamic_string_add_char(&token.value.valueString, c);
                c = (char)getc(file); // Read the next character
            }

            // Check for a decimal point or exponent
            if (c == '.')
            {
                dynamic_string_add_char(&token.value.valueString, c);
                state = sFloatLiteral; // Transition to float literal state
            }
            else if (c == 'e' || c == 'E')
            {
                dynamic_string_add_char(&token.value.valueString, c);
                state = sExponent; // Transition to exp literal state
            }
            else
            {
                state = sStart;
                ungetc(c, file);                // Put the char back for further processing
                token.type = TOKEN_INT_LITERAL; // Set token type to integer literal
                return token;
            }
        }
        break;
        case sFloatLiteral:
        {
            // Handle floating-point literal logic here
            while (isdigit(c))
            {
                dynamic_string_add_char(&token.value.valueString, c);
                c = (char)getc(file); // Read the next character
            }

            // Check for an exponent
            if (c == 'e' || c == 'E')
            {
                dynamic_string_add_char(&token.value.valueString, c);
                state = sExponent; // Transition to exponent state
            }
            else
            {
                // Finished floating-point literal
                token.type = TOKEN_FLOAT_LITERAL; // Set token type to float literal
                ungetc(c, file);                  // Put the character back for further processing
                return token;                     // Return the float token
            }
        }
        break;
        case sExponent:
        {
            // Handle exponent part logic here
            // Exponent can have an optional sign
            if (c == '+' || c == '-')
            {
                dynamic_string_add_char(&token.value.valueString, c); // Add sign to the exponent
                c = (char)getc(file);                                 // Read next character
            }

            // Exponent must be a sequence of digits
            if (!isdigit(c))
            {
                token.type = TOKEN_UNDEFINED; // Invalid exponent
                ungetc(c, file);              // Put the character back
                return token;                 // Return error token
            }

            // Read the exponent digits
            while (isdigit(c))
            {
                dynamic_string_add_char(&token.value.valueString, c);
                c = (char)getc(file); // Read the next character
            }

            // Finished processing exponent
            token.type = TOKEN_FLOAT_LITERAL; // Set token type to float literal
            ungetc(c, file);                  // Put the character back for further processing
            return token;                     // Return the float token
        }
        break;

        case sStringLiteral:
        {
            if (c == '\\') // Check for escape sequences
            {
                state = sEscapeSequence; // Transition to escape sequence handling
            }
            else if (c == '"') // End of string literal
            {
                token.type = TOKEN_STRING_LITERAL; // Set token type to string literal
                return token;                      // Return string token
            }
            else if (c < 32)
            {
                // Unexpected end of file without closing quote
                token.type = TOKEN_UNDEFINED; // Set error token
                return token;
            }
            else
            {
                dynamic_string_add_char(&token.value.valueString, c);
            }
        }
        break;
        case sEscapeSequence:
        {
            if (c == 'n')
            {
                dynamic_string_add_char(&token.value.valueString, '\n'); // Newline escape
            }
            else if (c == 't')
            {
                dynamic_string_add_char(&token.value.valueString, '\t'); // Tab escape
            }
            else if (c == 'r')
            {
                dynamic_string_add_char(&token.value.valueString, '\r'); // Carriage return escape
            }
            else if (c == '\"')
            {
                dynamic_string_add_char(&token.value.valueString, '\"'); // Quote escape
            }
            else if (c == '\\')
            {
                dynamic_string_add_char(&token.value.valueString, '\\'); // Backslash escape
            }
            else if (c == 'x') // Hexadecimal escape
            {
                char hex[3];               // Buffer to hold hex digits
                hex[0] = (char)getc(file); // First hex digit
                hex[1] = (char)getc(file); // Second hex digit
                hex[2] = '\0';             // Null-terminate the string

                // Check if the hex characters are valid
                if (isxdigit(hex[0]) && isxdigit(hex[1]))
                {
                    int value = (int)strtol(hex, NULL, 16);                         // Convert hex to integer
                    dynamic_string_add_char(&token.value.valueString, (char)value); // Add the character to the string
                }
                else
                {
                    token.type = TOKEN_UNDEFINED; // Invalid hex escape
                    return token;
                }
            }
            else
            {
                token.type = TOKEN_UNDEFINED; // Invalid escape sequence
                return token;
            }

            state = sStringLiteral; // Return to string literal state
        }
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
            return token;
            break;
        case sDivision:
        {
            if (c == '/')
            {
                state = sComment;
                dynamic_string_add_char(&token.value.valueString, c);
            }
            else
            {
                ungetc(c, file);             // Put back the character if it's not '='
                token.type = TOKEN_DIVISION; // Token is just '=' (assignment)
                return token;
            }
        }
        break;
        case sComment:
        {
            while (c != '\n' && c != EOF) // Consume until end of line
            {
                dynamic_string_add_char(&token.value.valueString, c);
                c = (char)getc(file);
            }
            state = sStart; // Return to starting state after comment
            token.type = TOKEN_COMMENT;
            return token;
        }
        break;
        case sAssign:
        {
            if (c == '=')
            {
                token.type = TOKEN_EQUAL; // Token is '==' (equality)
                dynamic_string_add_char(&token.value.valueString, c);
            }
            else
            {
                ungetc(c, file);               // Put back the character if it's not '='
                token.type = TOKEN_ASSIGNMENT; // Token is just '=' (assignment)
            }
            state = sStart; // Return to the starting state
            return token;
        }
        break;
        case sExclamation:
        {
            if (c == '=')
            {
                token.type = TOKEN_NOT_EQUAL; // Token is '!='
                dynamic_string_add_char(&token.value.valueString, c);
            }
            else
            {
                ungetc(c, file);                // Put back the character if it's not '='
                token.type = TOKEN_EXCLAMATION; // Token is just '!'
            }
            return token;
            state = sStart; // Return to the starting state
        }
        break;
        case sLessThan:
        {
            if (c == '=')
            {
                token.type = TOKEN_LESS_EQUAL; // Token is '<='
                dynamic_string_add_char(&token.value.valueString, c);
            }
            else
            {
                ungetc(c, file);              // Put back the character if it's not '='
                token.type = TOKEN_LESS_THAN; // Token is just '<'
            }
            state = sStart; // Return to the starting state
            return token;
        }
        break;
        case sGreaterThan:
        {
            if (c == '=')
            {
                token.type = TOKEN_GREATER_EQUAL; // Token is '>='
                dynamic_string_add_char(&token.value.valueString, c);
            }
            else
            {
                ungetc(c, file);                 // Put back the character if it's not '='
                token.type = TOKEN_GREATER_THAN; // Token is just '>'
            }
            state = sStart; // Return to the starting state
            return token;
        }
        break;
        case sImport:
        {
            while (isalpha(c))
            {
                // Přidáme znak do dynamického řetězce
                dynamic_string_add_char(&token.value.valueString, c);
                c = (char)getc(file); // Načti další znak
            }
            ungetc(c, file); // Vrať znak zpět do vstupu

            if (strcmp(token.value.valueString.str, "@import") == 0)
            {
                token.type = TOKEN_IMPORT;
            }
            else
            {
                token.type = TOKEN_UNDEFINED;
            }
            state = sStart;
            return token;
        }
        break;

        case sSingleQuote:
            // Handle single quote logic here
            break;

        case sLeftSingleQuote:
            // Handle left single quote logic here
            break;

        case sError:
            // Handle error logic here
            break;

        case sEOL:
            state = sStart;
            break;

        default:
            state = sError; // Invalid state fallback
            break;
        }
    }
    // Uvoľni pamäť pred návratom
    dynamic_string_free(&token.value.valueString);
    return token; // Vráť token
}