/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 *
 * @brief Error definitions.
 *
 * @author >Jakub Fiľo <xfiloja00@stud.fit.vutbr.cz>
 */

#ifndef _ERROR_H
#define _ERROR_H

// Error codes.
#define LEX_ERR 1                // Lexical error: Invalid structure of the current lexeme
#define SYNTAX_ERR 2             // Syntax error: Invalid program syntax
#define UNDEFINED_ID_ERR 3       // Undefined identifier error: Undefined function or variable
#define FUNC_PARAM_ERR 4         // Function parameter error: Incorrect number/type of function parameters
#define REDEF_ERR 5              // Redefinition error: Redefinition of variable or function
#define RETURN_EXPR_ERR 6        // Return expression error: Missing or extra expression in return statement
#define TYPE_COMPAT_ERR 7        // Type compatibility error: Incompatible type in expressions
#define TYPE_INFER_ERR 8         // Type inference error: Type cannot be inferred
#define UNUSED_VAR_ERR 9         // Unused variable error: Variable declared but not used
#define SEMANTIC_ERR 10          // Generic semantic error: Other semantic errors
#define COMPILER_INTERNAL_ERR 99 // Compiler internal error: Internal error unrelated to input program

#endif //_ERROR_H