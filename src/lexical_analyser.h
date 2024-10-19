#ifndef LEXICAL_ANALYSER_H
#define LEXICAL_ANALYSER_H

#include <stdio.h>
#include "newstring.h"

typedef enum
{
    TOKEN_EOF,        // 0 End of file marker
    TOKEN_EOL,        // 1 End of line character
    TOKEN_EMPTY,      // 2 Represents an empty token
    TOKEN_IDENTIFIER, // 3 Variable or function name
    TOKEN_KEYWORD,    // 4 Reserved keyword in the language
    TOKEN_NEWLINE,    // 5 Newline character
    TOKEN_TAB,        // 6 Tab character

    TOKEN_INT_LITERAL,    // 7 Integer literal (whole number)
    TOKEN_FLOAT_LITERAL,  // 8 Floating-point number
    TOKEN_STRING_LITERAL, // 9 String value
    TOKEN_BOOLEAN,        // 10 Boolean value (true/false)
    TOKEN_CHAR_LITERAL,   // 11 char after '\'

    TOKEN_EQUAL,         // 12 Equality comparison (==)
    TOKEN_NOT_EQUAL,     // 13 Inequality check (!=)
    TOKEN_LESS_EQUAL,    // 14 Less than or equal to (<=)
    TOKEN_LESS_THAN,     // 15 Less than (<)
    TOKEN_GREATER_EQUAL, // 16 Greater than or equal to (>=)
    TOKEN_GREATER_THAN,  // 17 Greater than (>)

    /// Operators
    TOKEN_ASSIGNMENT,  // 18 Assignment operator (=)
    TOKEN_ADDITION,    // 19 Addition operator (+)
    TOKEN_SUBTRACTION, // 20 Subtraction operator (-)
    TOKEN_MULTIPLY,    // 21 Multiplication operator (*)
    TOKEN_DIVISION,    // 22 Division operator (result as double)
    TOKEN_BACKSLASH,   // 23 '\'
    TOKEN_PIPE,        // 24 | logic OR or in the condition thingy if () ||
    TOKEN_EXCLAMATION, // 25 '!' symbol

    TOKEN_LPAREN,         // 26 Left parenthesis '('
    TOKEN_RPAREN,         // 27 Right parenthesis ')'
    TOKEN_LEFT_BRACKET,   // 28 Left bracket '['
    TOKEN_RIGHT_BRACKET,  // 29 Right bracket ']'
    TOKEN_CURLYL_BRACKET, // 30 Left bracket '{'
    TOKEN_CURLYR_BRACKET, // 31 Right bracket '}'
    TOKEN_COMMA,          // 32 Comma ','
    TOKEN_SEMICOLON,      // 33 Semicolon ';'
    TOKEN_COMMENT,        // 34
    TOKEN_IMPORT,         //@import
    TOKEN_DISCARD,        // only '_'
    TOKEN_COLON,
    TOKEN_DOT,
    TOKEN_UNDEFINED,
    TOKEN_STRINGINIT_ERROR

} Token_type;

typedef enum
{
    KEYWORD_IF,    // [a] if
    KEYWORD_ELSE,  // [ ] else
    KEYWORD_FN,    // [ ] function definition
    KEYWORD_CONST, // [ ] constant declaration
    KEYWORD_I32,   // [ ] 32-bit integer type
    KEYWORD_F64,   // [ ] 64-bit float type
    KEYWORD_U8,    // [ ] 8-bit unsigned integer type
    KEYWORD_U8_ARRAY,
    KEYWORD_I32_NULL,      // [?i32]  can handle NULL
    KEYWORD_F64_NULL,      // [?f64]
    KEYWORD_U8_NULL,       // [?u8]
    KEYWORD_U8_ARRAY_NULL, // [?[]u8]
    KEYWORD_NULL,          // [ ] null value
    KEYWORD_PUB,           // [ ] public visibility modifier
    KEYWORD_RETURN,        // [ ] return statement
    KEYWORD_VAR,           // [ ] variable declaration
    KEYWORD_VOID,          // [ ] void return type
    KEYWORD_WHILE,         // [ ] while loop
} Keyword;

typedef enum
{
    // Starting state, the initial state for the FSM.
    sStart,

    // States for handling identifiers and keywords.
    sIdentifierorKeyword, // Identifier (variables, function names); case-sensitive
    sKeyword,             // Keyword (const, else, fn, if, etc.)
    sIdentifier,

    // State for identifiers with namespace (e.g., ifj.write)
    sNamespace, // Handles 'ifj' namespace and functions like ifj.write

    // States for handling numbers.
    sIntLiteral,   // Integer literal (non-negative, decimal format)
    sFloatLiteral, // Floating-point literal (including exponent)
    sExponent,     // Handles the exponent part of floating-point numbers

    // State for string literals.
    sStringLiteral,  // String literal (starts and ends with double quotes)
    sEscapeSequence, // Handles escape sequences inside string literals

    // States for operators and punctuation.
    sOperator,     // Arithmetic and logical operators (+, -, *)
    sBackSlash,    //
    sDivision,     // / as division
    sAssign,       // Assignment operator (=)
    sEqual,        // Equality comparison (==)
    sNotEqual,     // Not equal (!=)
    sLessEqual,    // Less than or equal (<=)
    sLessThan,     // Less than (<)
    sGreaterEqual, // Greater than or equal (>=)
    sGreaterThan,  // Greater than (>)

    // States for parentheses, brackets, and punctuation.
    sLeftParen,     // Left parenthesis '('
    sRightParen,    // Right parenthesis ')'
    sLeftBracket,   // Left bracket '[' for slices
    sRightBracket,  // Right bracket ']' for slices
    sCurlyLBracket, // Left bracket '[' for slices
    sCurlyRBracket, // Right bracket ']' for slices
    sComma,         // Comma ','
    sSemicolon,     // Semicolon ';'
    sExclamation,   // Exclamation mark '!', used in logical negation or operators
    sPipe,          // pipe '|' character

    // States for quotation marks.
    sSingleQuote,     // Single quote ('), used for character literals
    sLeftSingleQuote, // Left single quotation mark (‘), used in specific typography

    // State for whitespace (ignored between tokens).
    sWhitespace, // Spaces, tabs, newlines

    // State for handling comments.
    sComment, // Line comment starting with //
    sImport,
    // Error and end state.
    sError, // Error state for invalid characters or input
    sEOL,   // End of line state (newline character)
    sEnd    // End of file or input state
} State;

typedef union
{
    int intValue;               // Pre celočíselné literály
    double floatValue;          // Pre desatinné literály
    Dynamic_string valueString; // Dynamický reťazec pre hodnotu
    int boolValue;              // Pre booleans (0 alebo 1)
} Token_Value;

typedef struct
{
    Token_type type;   // Type of the token
    Token_Value value; // Value of the token (union)
    Keyword keyword_val;    // Flag to indicate if it's a keyword (0-x) or NULL (0)
    int lineX, LineY;  // Line number in the source code
} Token;

// Deklarácia funkcie get_token
Token get_token(FILE *file);

#endif // LEXICAL_ANALYSER_H