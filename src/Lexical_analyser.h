#include <stdio.h>

typedef enum
{
    TOKEN_EOF,        // End of file marker
    TOKEN_EOL,        // End of line character
    TOKEN_EMPTY,      // Represents an empty token
    TOKEN_IDENTIFIER, // Variable or function name
    TOKEN_KEYWORD,    // Reserved keyword in the language
    TOKEN_NEWLINE,
    TOKEN_TAB,

    TOKEN_INT_LITERAL,    // Integer literal (whole number)
    TOKEN_FLOAT_LITERAL,  // Floating-point number
    TOKEN_STRING_LITERAL, // String value
    TOKEN_BOOLEAN,        // Boolean value (true/false)
    TOKEN_CHAR_LITERAL,   // char after '\'

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
    TOKEN_BACKSLASH,   // '\'
    TOKEN_PIPE,        // | logic OR  or in the condition thingy if () ||
    TOKEN_EXCLAMATION, // '!' symbol

    TOKEN_LPAREN,        // Left parenthesis '('
    TOKEN_RPAREN,        // Right parenthesis ')'
    TOKEN_LEFT_BRACKET,  // left '['
    TOKEN_RIGHT_BRACKET, // left ']'
    TOKEN_COMMA,         // Comma ','
    TOKEN_SEMICOLON,     // Semicolon ';'

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
    int lineX, LineY;  // Line number in the source code
} Token;

typedef enum
{
    // Starting state, the initial state for the FSM.
    sStart,

    // States for handling identifiers and keywords.
    sIdentifier, // Identifier (variables, function names); case-sensitive
    sKeyword,    // Keyword (const, else, fn, if, etc.)

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
    sLeftParen,    // Left parenthesis '('
    sRightParen,   // Right parenthesis ')'
    sLeftBracket,  // Left bracket '[' for slices
    sRightBracket, // Right bracket ']' for slices
    sComma,        // Comma ','
    sSemicolon,    // Semicolon ';'
    sExclamation,  // Exclamation mark '!', used in logical negation or operators
    sPipe,         // pipe '|' character

    // States for quotation marks.
    sSingleQuote,     // Single quote ('), used for character literals
    sLeftSingleQuote, // Left single quotation mark (‘), used in specific typography

    // State for whitespace (ignored between tokens).
    sWhitespace, // Spaces, tabs, newlines

    // State for handling comments.
    sComment, // Line comment starting with //

    // Error and end state.
    sError, // Error state for invalid characters or input
    sEOL,   // End of line state (newline character)
    sEnd    // End of file or input state
} State;
