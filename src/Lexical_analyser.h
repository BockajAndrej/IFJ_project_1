#include <stdio.h>

typedef enum
{
    TOKEN_EOF,        // End of file marker
    TOKEN_EOL,        // End of line character
    TOKEN_EMPTY,      // Represents an empty token
    TOKEN_IDENTIFIER, // Variable or function name
    TOKEN_KEYWORD,    // Reserved keyword in the language

    TOKEN_INT_LITERAL,    // Integer literal (whole number)
    TOKEN_FLOAT_LITERAL,  // Floating-point number
    TOKEN_STRING_LITERAL, // String value
    TOKEN_BOOLEAN,        // Boolean value (true/false)

    TOKEN_EQUAL,         // Equality comparison (==)
    TOKEN_NOT_EQUAL,     // Inequality check (!=)
    TOKEN_LESS_EQUAL,    // Less than or equal to (<=)
    TOKEN_LESS_THAN,     // Less than (<)
    TOKEN_GREATER_EQUAL, // Greater than or equal to (>=)
    TOKEN_GREATER_THAN,  // Greater than (>)

    /// Operators
    TOKEN_ASSIGNMENT,  // Assignment operator (=)
    TOKEN_ADDITION,    // Addition operator (+)
    TOKEN_SUBTRACTION, // Subtraction operator (-)
    TOKEN_MULTIPLY,    // Multiplication operator (*)
    TOKEN_DIVIDE,      // Division operator (result as double)
    TOKEN_INT_DIVIDE,  // Integer division operator (\) (integers only)

    TOKEN_LPAREN,    // Left parenthesis '('
    TOKEN_RPAREN,    // Right parenthesis ')'
    TOKEN_COMMA,     // Comma ','
    TOKEN_SEMICOLON, // Semicolon ';'

} Token_type;

typedef enum
{
    KEYWORD_IF,     // if
    KEYWORD_ELSE,   // else
    KEYWORD_FN,     // function definition
    KEYWORD_CONST,  // constant declaration
    KEYWORD_I32,    // 32-bit integer type
    KEYWORD_F64,    // 64-bit float type
    KEYWORD_NULL,   // null value
    KEYWORD_PUB,    // public visibility modifier
    KEYWORD_RETURN, // return statement
    KEYWORD_U8,     // 8-bit unsigned integer type
    KEYWORD_VAR,    // variable declaration
    KEYWORD_VOID,   // void return type
    KEYWORD_WHILE,  // while loop
} Keyword;

typedef union
{
    Keyword keyword;   // Using the enum for keywords
    int intValue;      // For integer literals
    double floatValue; // For floating-point literals
    char *stringValue; // For string literals
    int boolValue;     // For boolean values (0 or 1)
} Token_Value;

typedef struct
{
    Token_type type;   // Type of the token
    Token_Value value; // Value of the token (union)
    int lineX, LineY;          // Line number in the source code
} Token;
