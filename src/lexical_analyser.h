/**
 * @file lexical_analyser.h
 * @author Jakub Filo
 * @category Lexical analysis
 * @brief This file contains functions for lexical analysis, processing input file, and extracting tokens.
 */

#ifndef LEXICAL_ANALYSER_H
#define LEXICAL_ANALYSER_H

#include <stdio.h>
#include <error.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "newstring.h"
#include "error.h"

/**
 * @enum Token_type
 * @brief Enumeration of possible token types.
 *
 * This enumeration defines the various types of tokens that can be identified during lexical analysis.
 * Each token type represents a specific category of syntactic element in the language.
 */
typedef enum
{
    TOKEN_NTERMINAL,
    /**
     * @brief End-of-file marker.
     * Indicates that the end of the input file has been reached.
     */
    TOKEN_EOF,
    /**
     * @brief End-of-line marker.
     * Represents a newline character in the source code.
     */
    TOKEN_EOL,
    /**
     * @brief Identifier token.
     * Represents variable names, function names, or other user-defined identifiers.
     */
    TOKEN_IDENTIFIER,
    /**
     * @brief Keyword token.
     * Represents a reserved keyword in the language (e.g., if, else, return).
     */
    TOKEN_KEYWORD,
    /**
     * @brief Newline character token.
     * Specifically used for handling newline characters (`\n`) in the input.
     */
    TOKEN_NEWLINE,
    /**
     * @brief Integer literal token.
     * Represents whole numbers (e.g., 42, -7).
     */
    TOKEN_INT_LITERAL,
    /**
     * @brief Floating-point literal token.
     * Represents decimal or exponential numbers (e.g., 3.14, 2.5e-10).
     */
    TOKEN_FLOAT_LITERAL,
    /**
     * @brief String literal token.
     * Represents a sequence of characters enclosed in double quotes.
     */
    TOKEN_STRING_LITERAL,
    /**
     * @brief Equality comparison operator.
     * Represents the equality operator (`==`).
     */
    TOKEN_EQUAL,
    /**
     * @brief Inequality comparison operator.
     * Represents the not-equal-to operator (`!=`).
     */
    TOKEN_NOT_EQUAL,
    /**
     * @brief Less-than-or-equal-to operator.
     * Represents the operator (`<=`).
     */
    TOKEN_LESS_EQUAL,
    /**
     * @brief Less-than operator.
     * Represents the operator (`<`).
     */
    TOKEN_LESS_THAN,
    /**
     * @brief Greater-than-or-equal-to operator.
     * Represents the operator (`>=`).
     */
    TOKEN_GREATER_EQUAL,
    /**
     * @brief Greater-than operator.
     * Represents the operator (`>`).
     */
    TOKEN_GREATER_THAN,
    /**
     * @brief Assignment operator.
     * Represents the assignment operator (`=`).
     */
    TOKEN_ASSIGNMENT,
    /**
     * @brief Addition operator.
     * Represents the addition operator (`+`).
     */
    TOKEN_ADDITION,
    /**
     * @brief Subtraction operator.
     * Represents the subtraction operator (`-`).
     */
    TOKEN_SUBTRACTION,
    /**
     * @brief Multiplication operator.
     * Represents the multiplication operator (`*`).
     */
    TOKEN_MULTIPLY,
    /**
     * @brief Division operator.
     * Represents the division operator (`/`) with a result as a floating-point number.
     */
    TOKEN_DIVISION,
    /**
     * @brief Backslash character token.
     * Represents the backslash character (`\`).
     */
    TOKEN_BACKSLASH,
    /**
     * @brief Logical OR operator.
     * Represents the logical OR operator (`||`).
     */
    TOKEN_PIPE,
    /**
     * @brief Exclamation mark token.
     * Represents the exclamation mark (`!`), typically used in logical negation.
     */
    TOKEN_EXCLAMATION,
    /**
     * @brief Left parenthesis token.
     * Represents the opening parenthesis (`(`).
     */
    TOKEN_LPAREN,
    /**
     * @brief Right parenthesis token.
     * Represents the closing parenthesis (`)`).
     */
    TOKEN_RPAREN,
    /**
     * @brief Left square bracket token.
     * Represents the opening square bracket (`[`).
     */
    TOKEN_LEFT_BRACKET,
    /**
     * @brief Right square bracket token.
     * Represents the closing square bracket (`]`).
     */
    TOKEN_RIGHT_BRACKET,
    /**
     * @brief Left curly bracket token.
     * Represents the opening curly bracket (`{`).
     */
    TOKEN_CURLYL_BRACKET,
    /**
     * @brief Right curly bracket token.
     * Represents the closing curly bracket (`}`).
     */
    TOKEN_CURLYR_BRACKET,
    /**
     * @brief Comma token.
     * Represents the comma (`,`) used for separating elements.
     */
    TOKEN_COMMA,
    /**
     * @brief Semicolon token.
     * Represents the semicolon (`;`) used for statement termination.
     */
    TOKEN_SEMICOLON,
    /**
     * @brief Comment token.
     * Represents comments in the code (e.g., `// This is a comment`).
     */
    TOKEN_COMMENT,
    /**
     * @brief Import token.
     * Represents the `@import` statement.
     */
    TOKEN_IMPORT,
    /**
     * @brief Discard token.
     * Represents the underscore (`_`), used for discarding values.
     */
    TOKEN_DISCARD,
    /**
     * @brief Colon token.
     * Represents the colon (`:`).
     */
    TOKEN_COLON,
    /**
     * @brief Dot token.
     * Represents the dot (`.`), often used for member access or namespaces.
     */
    TOKEN_DOT,
    /**
     * @brief Question mark token.
     * Represents the question mark (`?`), often used in conditional expressions.
     */
    TOKEN_QUESTION_MARK,
    /**
     * @brief Undefined token.
     * Represents an unrecognized or invalid token.
     */
    TOKEN_UNDEFINED,
    /**
     * @brief String initialization error token.
     * Represents an error encountered during string initialization.
     */
    TOKEN_STRINGINIT_ERROR,
    /**
     * @brief Represents a null token.
     */
    TOKEN_NULL,

    /**
     * @brief Represents an empty token.
     */
    TOKEN_EMPTY

} Token_type;

/**
 * @brief Enum representing various keywords in the language.
 *
 * This enumeration lists the reserved keywords used in the language's syntax.
 */
typedef enum
{
    /**
     * @brief Represents the `if` keyword.
     *
     * Used for conditional branching to execute code based on a boolean condition.
     */
    KEYWORD_IF,

    /**
     * @brief Represents the `else` keyword.
     *
     * Specifies the block of code to execute if the condition in the corresponding `if` statement is false.
     */
    KEYWORD_ELSE,

    /**
     * @brief Represents the `fn` keyword.
     *
     * Used to define a function in the language.
     */
    KEYWORD_FN,

    /**
     * @brief Represents the `const` keyword.
     *
     * Declares a constant value that cannot be modified after initialization.
     */
    KEYWORD_CONST,

    /**
     * @brief Represents the `i32` keyword.
     *
     * Specifies a 32-bit signed integer type.
     */
    KEYWORD_I32,

    /**
     * @brief Represents the `f64` keyword.
     *
     * Specifies a 64-bit floating-point type.
     */
    KEYWORD_F64,

    /**
     * @brief Represents the `u8` keyword.
     *
     * Specifies an 8-bit unsigned integer type.
     */
    KEYWORD_U8,

    /**
     * @brief Represents the `u8[]` keyword.
     *
     * Specifies an array of 8-bit unsigned integers.
     */
    KEYWORD_U8_ARRAY,

    /**
     * @brief Represents the `?i32` keyword.
     *
     * Specifies an optional 32-bit signed integer type that can be NULL.
     */
    KEYWORD_I32_NULL,

    /**
     * @brief Represents the `?f64` keyword.
     *
     * Specifies an optional 64-bit floating-point type that can be NULL.
     */
    KEYWORD_F64_NULL,

    /**
     * @brief Represents the `?u8` keyword.
     *
     * Specifies an optional 8-bit unsigned integer type that can be NULL.
     */
    KEYWORD_U8_NULL,

    /**
     * @brief Represents the `?[]u8` keyword.
     *
     * Specifies an optional array of 8-bit unsigned integers that can be NULL.
     */
    KEYWORD_U8_ARRAY_NULL,

    /**
     * @brief Represents the `pub` keyword.
     *
     * Indicates public visibility for functions,
     * allowing access from outside their scope.
     */
    KEYWORD_PUB,

    /**
     * @brief Represents the `return` keyword.
     *
     * Used to exit a function and optionally provide a return value.
     */
    KEYWORD_RETURN,

    /**
     * @brief Represents the `var` keyword.
     *
     * Declares a variable that can be reassigned during its lifetime.
     */
    KEYWORD_VAR,

    /**
     * @brief Represents the `void` keyword.
     *
     * Specifies that a function does not return a value.
     */
    KEYWORD_VOID,

    /**
     * @brief Represents the `while` keyword.
     *
     * Creates a loop that repeatedly executes code as long as a boolean condition evaluates to true.
     */
    KEYWORD_WHILE

} Keyword;

/**
 * @enum State
 * @brief Enumeration representing the states of a finite state machine (FSM) for lexical analysis.
 *
 * This FSM is used to tokenize input by transitioning between states based on the input character.
 * Each state corresponds to a specific type of token or behavior during tokenization.
 */
typedef enum
{
    /**
     * @brief The initial state of the FSM.
     * This state represents the starting point before processing any input character.
     */
    sStart,

    /**
     * @brief States for handling identifiers and keywords.
     */
    sIdentifierorKeyword, ///< Transition state for detecting identifiers or keywords; case-sensitive.
    sKeyword,             ///< Indicates the token is a reserved keyword (e.g., const, else, fn, if, etc.).
    sIdentifier,          ///< Indicates the token is an identifier (e.g., variable or function name).

    /**
     * @brief States for handling numeric literals.
     */
    sIntLiteral,    ///< Represents an integer literal (e.g., "123"), parsed before encountering a '.' or 'e/E'.
    sFloatLiteral,  ///< Represents a floating-point literal, including numbers with a fractional part (e.g., "123.45").
    sExponentStart, ///< Represents the transition state after encountering 'e' or 'E', awaiting an exponent or sign.
    sDot,           ///< Represents the state after encountering '.', ensuring a digit follows to form a valid float.
    sExponent,      ///< Handles the parsing of the numeric exponent part (e.g., "1.23e4"), ensuring valid digits follow.
    sExponentSign,  ///< Represents the sign ('+' or '-') immediately following 'e/E' in a floating-point literal.

    /**
     * @brief States for handling string literals.
     */
    sStringLiteral,  ///< Represents a string literal enclosed in double quotes.
    sEscapeSequence, ///< Handles escape sequences (e.g., \n, \t) within string literals.

    /**
     * @brief States for handling operators and punctuation.
     */
    sBackSlash,    ///< Handles the backslash character ('\').
    sDivision,     ///< Represents the division operator ('/').
    sAssign,       ///< Represents the assignment operator ('=').
    sEqual,        ///< Represents the equality comparison operator ('==').
    sNotEqual,     ///< Represents the inequality operator ('!=').
    sLessEqual,    ///< Represents the less than or equal to operator ('<=').
    sLessThan,     ///< Represents the less than operator ('<').
    sGreaterEqual, ///< Represents the greater than or equal to operator ('>=').
    sGreaterThan,  ///< Represents the greater than operator ('>').
    sQuestionmark,
    sExclamation, ///< Represents the exclamation mark ('!'), used in logical negation or other operators.

    /**
     * @brief State for handling comments.
     */
    sComment, ///< Represents a line comment starting with "//".
    sImport,  ///< Represents the '@' symbol for import statements.

    /**
     * @brief States indicating errors or the end of input.
     */
    sError, ///< Indicates an error state due to invalid input or characters.
    sEnd    ///< End-of-file state or input completion.
} State;

/**
 * @union Token_Value
 * @brief Union for storing token values.
 */
typedef union
{
    int intValue;               // Pre celočíselné literály
    double floatValue;          // Pre desatinné literály
    Dynamic_string valueString; // Dynamický reťazec pre hodnotu
    int boolValue;              // Pre booleans (0 alebo 1)
} Token_Value;

/**
 * @struct Token
 * @brief Structure representing a token.
 */
typedef struct
{
    Token_type type;     // Type of the token
    Token_Value value;   // Value of the token (union)
    Keyword keyword_val; // Flag to indicate if it's a keyword (0-x) or NULL (0)
    int lineX, LineY;    // Line number in the source code
} Token;

/**
 * @brief Converts a token type to its string representation.
 * @param type The token type to convert.
 * @return String representation of the token type.
 */
const char *token_type_to_string(Token_type type);

/**
 * @brief Prints the details of a token.
 * @param token The token to print.
 */
void print_token(Token token);

/**
 * @brief Retrieves the next token from the input file.
 * @param file The file to read from.
 * @return The next token.
 */
Token get_token(FILE *file);

/**
 * @brief Converts a token type to its corresponding string representation.
 *
 * @param value The token type to be converted.
 * @return A constant character pointer to the string representation of the token type.
 */
const char *token_to_type(Token_type value);

/**
 * @brief Validates whether a given string is a valid identifier.
 *
 * @param str The string to be validated.
 */
void validate_identifier(const char *str);

#endif // LEXICAL_ANALYSER_H