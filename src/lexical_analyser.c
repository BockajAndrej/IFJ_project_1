/**
 * @file lexical_analyser.c
 * @author Jakub Filo
 * @category Lexical Analysis
 * @brief Implementation of the lexical analyser, including token processing and utility functions.
 */

#include "lexical_analyser.h"
#include "newstring.h"

/**
 * @def KEYWORD_COUNT
 * @brief Defines the number of keywords in the language.
 *
 * Update this value if more keywords are added to the language.
 */
#define KEYWORD_COUNT 17 // Update this if you add more keywords

/**
 * @var keywords
 * @brief Array of keyword strings used for identifying language keywords.
 *
 * Each element in the array corresponds to a specific keyword enumeration.
 */
const char *keywords[KEYWORD_COUNT] = {
    "if",     // KEYWORD_IF
    "else",   // KEYWORD_ELSE
    "fn",     // KEYWORD_FN
    "const",  // KEYWORD_CONST
    "i32",    // KEYWORD_I32
    "f64",    // KEYWORD_F64
    "u8",     // KEYWORD_U8
    "[]u8",   // KEYWORD_U8_ARRAY
    "?i32",   // KEYWORD_I32_NULL
    "?f64",   // KEYWORD_F64_NULL
    "?u8",    // KEYWORD_U8_NULL
    "?[]u8",  // KEYWORD_U8_ARRAY_NULL
    "pub",    // KEYWORD_PUB
    "return", // KEYWORD_RETURN
    "var",    // KEYWORD_VAR
    "void",   // KEYWORD_VOID
    "while"   // KEYWORD_WHILE
};

/**
 * @brief Checks if a given token is a keyword.
 * @details Iterates through the list of predefined keywords to determine if the token matches any keyword.
 * If a match is found, it sets the `keyword_val` field of the token accordingly.
 *
 * @param token A pointer to the token to be checked.
 * @return `true` if the token is a keyword, `false` otherwise.
 *
 * @see Keyword
 */
bool is_keyword(Token *token)
{
    for (int i = 0; i < KEYWORD_COUNT; i++)
    {
        if (strcmp(token->value.valueString.str, keywords[i]) == 0)
        {
            token->keyword_val = (Keyword)i; // Set the keyword value
            return true;                     // It is a keyword
        }
    }
    return false; // It is not a keyword
}

/**
 * @brief Converts a token type to its string representation.
 * @details This function maps each `Token_type` enumeration to a corresponding string for easy readability.
 *
 * @param type The token type to convert.
 * @return A constant character pointer representing the token type as a string.
 */
const char *token_type_to_string(Token_type type)
{
    switch (type)
    {
    case TOKEN_EOF:
        return "TOKEN_EOF";
    case TOKEN_EOL:
        return "TOKEN_EOL";
    case TOKEN_IDENTIFIER:
        return "TOKEN_IDENTI";
    case TOKEN_KEYWORD:
        return "TOKEN_KEYWORD";
    case TOKEN_EMPTY:
        return "TOKEN_EMPTY";
    case TOKEN_NEWLINE:
        return "TOKEN_NEWLINE";
    case TOKEN_NULL:
        return "TOKEN_NULL";
    case TOKEN_INT_LITERAL:
        return "TOKEN_INT_L";
    case TOKEN_FLOAT_LITERAL:
        return "TOKEN_FLOAT_L";
    case TOKEN_STRING_LITERAL:
        return "TOKEN_STRING_L";
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
    case TOKEN_QUESTION_MARK:
        return "TOKEN_QUEST";
    default:
        return "UNKNOWN_TOKEN";
    }
}

/**
 * @brief Prints the details of a token.
 * @details This function outputs the token type, its value, and the keyword enumeration if applicable.
 * It handles different token types to provide meaningful output.
 *
 * @param token The token to be printed.
 */
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
    case TOKEN_NEWLINE:
        printf("NEWLINE\n");
        break;
    case TOKEN_UNDEFINED:
        printf("UNDEFINED\t%s\t\t-\n", token.value.valueString.str);
        break;
    default:
        printf("%s\t\t%s\t\t%d\n", token_type_to_string(token.type), token.value.valueString.str, token.keyword_val);
        break;
    }
}

const char *token_to_type(Token_type value)
{
    switch (value)
    {
    case TOKEN_INT_LITERAL:
        return "i32";
    case TOKEN_FLOAT_LITERAL:
        return "f64";
    case TOKEN_STRING_LITERAL:
        return "[]u8";
    default:
        break;
    }
    return "";
}

/**
 * @brief Checks if a string contains only valid characters (a-z, A-Z, 0-9, and '_').
 * @details Iterates through each character in the string and verifies if it is an alphanumeric character or an underscore.
 * If an invalid character is found, it calls the `handle_error` function with a lexical error code.
 *
 * @param str The string to be checked.
 */
void validate_identifier(const char *str)
{
    while (*str)
    {
        if (!isalnum(*str) && *str != '_')
        {
            handle_error(ERR_LEX); // Handle lexical error
        }
        str++;
    }
}

/**
 * @brief Retrieves the next token from the input file.
 * @details This function implements a finite state machine (FSM) to parse the input file character by character,
 * constructing tokens based on the defined states. It handles various token types, including identifiers,
 * keywords, literals, operators, and punctuation.
 *
 * @param file A pointer to the file being analyzed.
 * @return The next token extracted from the file. If an error occurs during string initialization or reading,
 * a TOKEN describing the error will be returned.
 *
 * @see Token
 * @see State
 */
Token get_token(FILE *file)
{

    Token token; // Initialize token
    State state; // Initialize state

    if (file == NULL) // Check if file is NULL
    {
        token.type = TOKEN_EOF; // Set token type to EOF
        return token;           // Return EOF token
    }
    state = sStart;

    if (!dynamic_string_init(&token.value.valueString)) // Initialize dynamic string
    {
        handle_error(ERR_COMPILER_INTERNAL); // Handle initialization error
    }

    char c;               // Character variable for reading input
    bool invalid = false; // Flag for invalid tokens

    while (1)
    {
        token.keyword_val = 0; // Reset keyword value
        c = (char)getc(file);

        switch (state) // State machine
        {
        case sStart:                   // Initial state
            if (c == ' ' || c == '\t') // Ignore whitespace
            {
                state = sStart; // Stay in start state
            }
            else if (c == EOF)
            {
                token.type = TOKEN_EOF; // Set token type to EOF
                return token;
            }
            else if (c == '@') // Start of import statement
            {
                state = sImport;
                dynamic_string_add_char(&token.value.valueString, c); // Add character to token
            }
            else if (c == '\n') /* EOL, end of line  should add \t \r support? */
            {
                token.type = TOKEN_EOL;
                return token;
            }
            else if (isalpha(c)) // Check for identifiers/keywords
            {
                state = sIdentifierorKeyword;
                dynamic_string_add_char(&token.value.valueString, c);
            }
            else if (c == '_')
            {
                dynamic_string_add_char(&token.value.valueString, c);

                char next_c = (char)getc(file);

                // If the next character is a letter or a digit, treat it as part of an identifier
                if (isalnum(next_c))
                {
                    dynamic_string_add_char(&token.value.valueString, next_c);
                    state = sIdentifierorKeyword;
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
                        dynamic_string_free(&token.value.valueString);
                        handle_error(ERR_LEX);
                    }
                }
            }
            else if (isdigit(c)) // Check for numbers
            {
                state = sIntLiteral; // Start reading an integer literal
                dynamic_string_add_char(&token.value.valueString, c);
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

                c = (char)getc(file); // Read next char
                if (c == ']')
                {
                    state = sIdentifierorKeyword;
                    ungetc(c, file);
                }
                else
                {
                    token.type = TOKEN_LEFT_BRACKET;
                    ungetc(c, file);
                    return token;
                }
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
                token.type = TOKEN_SEMICOLON;
                state = sStart;
                return token;
            }
            else if (c == ':') // Colon
            {
                dynamic_string_add_char(&token.value.valueString, c);
                token.type = TOKEN_COLON;
                state = sStart;
                return token;
            }
            else if (c == '.') // Dot
            {
                dynamic_string_add_char(&token.value.valueString, c);
                token.type = TOKEN_DOT;
                state = sStart;
                return token;
            }
            else if (c == '?') // Question mark
            {
                state = sQuestionmark;
                dynamic_string_add_char(&token.value.valueString, c);
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
            else if (c == '|') // Check for |
            {
                dynamic_string_add_char(&token.value.valueString, c);
                token.type = TOKEN_PIPE;
                return token;
            }
            else if (c == '=') // Start of equality or assignment
            {
                state = sAssign;
                dynamic_string_add_char(&token.value.valueString, c);
            }
            else if (c == '<') // Start of less than or equal
            {
                state = sLessThan;
                dynamic_string_add_char(&token.value.valueString, c);
            }
            else if (c == '>') // Start of greater than or equal
            {
                state = sGreaterThan;
                dynamic_string_add_char(&token.value.valueString, c);
            }
            else if (c == '\\') // Backslash for escape sequences
            {
                dynamic_string_add_char(&token.value.valueString, c);
                token.type = TOKEN_BACKSLASH;
                return token;
            }
            else if (c == '!') // Logical NOT or not equal
            {
                dynamic_string_add_char(&token.value.valueString, c);
                state = sExclamation;
            }
            break;
        case sQuestionmark: // state for "?"
        {
            invalid = true;
            if (isalpha(c) || c == '[')
            {

                dynamic_string_add_char(&token.value.valueString, c); // Add to token
                state = sIdentifierorKeyword;                         // Transition to identifier state
            }
            else
            {
                dynamic_string_free(&token.value.valueString);
                handle_error(ERR_LEX);
            }
        }
        break;
        case sIdentifierorKeyword: // keyword or identifier or left bracket
        {
            while (!isspace(c) && c != EOF)
            {
                // check for wrong symbols
                if (!isalpha(c) && !isdigit(c) && c != '_' && c != '[' && c != ']')
                {
                    break;
                }
                dynamic_string_add_char(&token.value.valueString, c);
                c = (char)getc(file);
            }

            ungetc(c, file);

            if (is_keyword(&token))
            {
                token.type = TOKEN_KEYWORD;
            }
            else if (strcmp(token.value.valueString.str, "null") == 0)
            {
                token.type = TOKEN_NULL;
            }
            else if (strcmp(token.value.valueString.str, "[") == 0)
            {
                token.type = TOKEN_LEFT_BRACKET;
            }
            else if (invalid)
            {
                dynamic_string_free(&token.value.valueString);
                handle_error(ERR_LEX);
            }
            else
            {
                // ! funkcia na povolene znaky
                validate_identifier(token.value.valueString.str);
                token.type = TOKEN_IDENTIFIER;
            }

            return token;
        }
        break;
        /**
         * Handles the state transitions for integer and floating-point literals in the lexical analyzer.
         *
         * States:
         * - sIntLiteral: Handles integer literals. Transitions to sDot for floating-point or sExponentStart for scientific notation.
         * - sDot: Handles the dot in floating-point literals. Transitions to sFloatLiteral if followed by a digit.
         * - sFloatLiteral: Handles the digits in floating-point literals. Transitions to sExponentStart for scientific notation.
         * - sExponentStart: Handles the start of the exponent part in scientific notation. Transitions to sExponentSign or sExponent.
         * - sExponentSign: Handles the sign of the exponent in scientific notation. Transitions to sExponent.
         * - sExponent: Handles transforming the notation to number
         *
         * Each state processes the current character and either adds it to the token's value, transitions to another state, or finalizes the token.
         * If an invalid character is encountered, an error is handled appropriately.
         */
        case sIntLiteral:
        {
            if (isdigit(c))
            {
                dynamic_string_add_char(&token.value.valueString, c);
                state = sIntLiteral;
            }
            else if (c == '.')
            {
                dynamic_string_add_char(&token.value.valueString, c);
                state = sDot; // Transition to sDot, state before float
            }
            else if (c == 'e' || c == 'E')
            {
                dynamic_string_add_char(&token.value.valueString, c);
                state = sExponentStart; // Transition to sExponentStart to handle + - sign
            }
            else
            {
                token.type = TOKEN_INT_LITERAL;
                state = sStart;
                ungetc(c, file);
                return token;
            }
        }
        break;
        case sDot:
        {
            if (isdigit(c))
            {
                dynamic_string_add_char(&token.value.valueString, c);
                state = sFloatLiteral;
            }
            else
            {
                dynamic_string_free(&token.value.valueString);
                handle_error(ERR_LEX);
            }
        }
        break;
        case sFloatLiteral:
        {
            if (isdigit(c))
            {
                dynamic_string_add_char(&token.value.valueString, c);
                state = sFloatLiteral;
            }
            else if (c == 'e' || c == 'E')
            {
                dynamic_string_add_char(&token.value.valueString, c);
                state = sExponentStart;
            }
            else
            {
                token.type = TOKEN_FLOAT_LITERAL;
                ungetc(c, file);
                return token;
            }
        }
        break;
        case sExponentStart:
        {
            if (c == '+' || c == '-')
            {
                dynamic_string_add_char(&token.value.valueString, c);
                state = sExponentSign;
            }
            else if (isdigit(c))
            {
                dynamic_string_add_char(&token.value.valueString, c);
                state = sExponent;
            }
            else
            {
                dynamic_string_free(&token.value.valueString);
                handle_error(ERR_LEX);
            }
        }
        break;
        case sExponentSign:
        {
            if (isdigit(c))
            {
                dynamic_string_add_char(&token.value.valueString, c);
                state = sExponent;
            }
            else
            {
                token.type = TOKEN_UNDEFINED;
                return token;
            }
        }
        break;
        case sExponent:
        {
            if (isdigit(c))
            {
                dynamic_string_add_char(&token.value.valueString, c);
                state = sExponent;
            }
            else
            {
                // Attempt to parse the number, checking for integer or float
                double calculatedNum = strtod(token.value.valueString.str, NULL);
                dynamic_string_clear(&token.value.valueString);

                // Check if it's a valid integer (i.e., no fractional part)
                if (calculatedNum == (long long)calculatedNum)
                {
                    // If the number is an integer (even with an exponent), treat it as an integer
                    long long intVal = (long long)calculatedNum;
                    if (!add_double_to_dynamic_string(&token.value.valueString, intVal))
                    {
                        dynamic_string_free(&token.value.valueString);
                        handle_error(ERR_LEX);
                    }
                    token.type = TOKEN_INT_LITERAL;
                }
                else
                {
                    // Otherwise, it's a float
                    if (!add_double_to_dynamic_string(&token.value.valueString, calculatedNum))
                    {
                        dynamic_string_free(&token.value.valueString);
                        handle_error(ERR_LEX);
                    }
                    token.type = TOKEN_FLOAT_LITERAL;
                }

                ungetc(c, file);
                return token;
            }
        }
        break;
        case sStringLiteral:
        {
            if (c == '\\') // Check for escape sequences
            {
                state = sEscapeSequence;
            }
            else if (c == '"') // End of string literal
            {
                token.type = TOKEN_STRING_LITERAL;
                return token;
            }
            else if (c < 32) // Check for invalid characters in string
            {
                dynamic_string_free(&token.value.valueString);
                handle_error(ERR_LEX); // Handle lexical error
            }
            else // Add character to string literal
            {
                dynamic_string_add_char(&token.value.valueString, c); // Add character to token
            }
        }
        break;
        case sEscapeSequence:
        {
            switch (c)
            {
            case 'n':
                dynamic_string_add_char(&token.value.valueString, '\n'); // Newline
                break;
            case 't':
                dynamic_string_add_char(&token.value.valueString, '\t'); // Tab
                break;
            case 'r':
                dynamic_string_add_char(&token.value.valueString, '\r'); // Carriage return
                break;
            case '\"':
                dynamic_string_add_char(&token.value.valueString, '\"'); // Double quote
                break;
            case '\\':
                dynamic_string_add_char(&token.value.valueString, '\\'); // Backslash
                break;
            case 'x': // Hexadecimal escape
            {
                // Converting \xdd from hex to real numer
                char hex[3];
                hex[0] = (char)getc(file);
                hex[1] = (char)getc(file);
                hex[2] = '\0';

                if (isxdigit(hex[0]) && isxdigit(hex[1]))
                {
                    int value = (int)strtol(hex, NULL, 16);
                    dynamic_string_add_char(&token.value.valueString, (char)value);
                }
                else
                {
                    dynamic_string_free(&token.value.valueString);
                    handle_error(ERR_LEX);
                }
                break;
            }
            default: // Unrecognized escape, handle as error
                dynamic_string_free(&token.value.valueString);
                handle_error(ERR_LEX);
                break;
            }

            state = sStringLiteral;
        }
        break;
        case sDivision:
        {
            if (c == '/')
            {
                state = sComment; // Transition to comment state if another '/' is found
                dynamic_string_add_char(&token.value.valueString, c);
            }
            else
            {
                ungetc(c, file);             // Put back the character if it's not '/'
                token.type = TOKEN_DIVISION; // Token is just '/' (division)
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
            if (isalpha(c))
            {
                dynamic_string_add_char(&token.value.valueString, c);
                state = sImport;
            }
            else
            {
                ungetc(c, file);
                if (strcmp(token.value.valueString.str, "@import") == 0)
                    token.type = TOKEN_IMPORT;
                else
                {
                    dynamic_string_free(&token.value.valueString);
                    handle_error(ERR_LEX);
                }

                state = sStart;
                return token;
            }
        }
        break;
        default:
            break;
        }
    }
    dynamic_string_free(&token.value.valueString);
    return token;
}
