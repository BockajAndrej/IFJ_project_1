/**
 * @file error.h
 * @author Jakub Filo
 * @category Error codes
 * @brief This file contains enum that has error numbers for specific errors.
 */

#ifndef _LOC_ERROR_H
#define _LOC_ERROR_H

#include <stdio.h>
#include <stdlib.h>

typedef enum
{
    ERR_SUCCESSFULL = 0,       // When program ends with designed purpose
    ERR_LEX = 1,               // Lexical error: Invalid structure of the current lexeme
    ERR_SYNTAX = 2,            // Syntax error: Invalid program syntax
    ERR_UNDEFINED_ID = 3,      // Undefined identifier error: Undefined function or variable
    ERR_FUNC_PARAM = 4,        // Function parameter error: Incorrect number/type of function parameters
    ERR_REDEF = 5,             // Redefinition error: Redefinition of variable or function
    ERR_RETURN_EXPR = 6,       // Return expression error: Missing or extra expression in return statement
    ERR_TYPE_COMPAT = 7,       // Type compatibility error: Incompatible type in expressions
    ERR_TYPE_INFER = 8,        // Type inference error: Type cannot be inferred
    ERR_UNUSED_VAR = 9,        // Unused variable error: Variable declared but not used
    ERR_SEMANTIC = 10,         // Generic semantic error: Other semantic errors
    ERR_MEM = 11,              // Memory-related errors
    ERR_IO = 12,               // Input/Output errors
    ERR_NULL = 13,             // Null pointer errors
    ERR_FILE = 14,             // File errors
    ERR_COMPILER_INTERNAL = 99 // Compiler internal error: Internal error unrelated to input program
} ErrorCode;

void handle_error(ErrorCode error_code);

#endif //_LOC_ERROR_H