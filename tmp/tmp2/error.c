/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 *
 * @brief Error codes.
 *
 * @author >Jakub Fiľo <xfiloja00@stud.fit.vutbr.cz>
 */

#include "error.h"

// Function to handle errors, free allocated memory, and exit
void handle_error(ErrorCode error_code)
{
    // Print the error number and corresponding message
    switch (error_code)
    {
    case ERR_LEX:
        fprintf(stderr, "Error %d: Lexical error - Invalid structure of the current lexeme.\n", ERR_LEX);
        break;
    case ERR_SYNTAX:
        fprintf(stderr, "Error %d: Syntax error - Invalid program syntax (missing header, etc.).\n", ERR_SYNTAX);
        break;
    case ERR_UNDEFINED_ID:
        fprintf(stderr, "Error %d: Semantic error - Undefined function or variable.\n", ERR_UNDEFINED_ID);
        break;
    case ERR_FUNC_PARAM:
        fprintf(stderr, "Error %d: Semantic error - Incorrect number or type of function parameters; invalid return value.\n", ERR_FUNC_PARAM);
        break;
    case ERR_REDEF:
        fprintf(stderr, "Error %d: Semantic error - Redefinition of variable or function; assignment to non-modifiable variable.\n", ERR_REDEF);
        break;
    case ERR_RETURN_EXPR:
        fprintf(stderr, "Error %d: Semantic error - Missing or extra expression in return statement.\n", ERR_RETURN_EXPR);
        break;
    case ERR_TYPE_COMPAT:
        fprintf(stderr, "Error %d: Semantic error - Type compatibility error in arithmetic, string, or relational expressions.\n", ERR_TYPE_COMPAT);
        break;
    case ERR_TYPE_INFER:
        fprintf(stderr, "Error %d: Semantic error - Type cannot be inferred from the expression.\n", ERR_TYPE_INFER);
        break;
    case ERR_UNUSED_VAR:
        fprintf(stderr, "Error %d: Semantic error - Unused variable in its scope; a variable that cannot be modified after initialization.\n", ERR_UNUSED_VAR);
        break;
    case ERR_SEMANTIC:
        fprintf(stderr, "Error %d: Semantic error - Generic semantic error.\n", ERR_SEMANTIC);
        break;
    case ERR_MEM:
        fprintf(stderr, "Error %d: Memory-related error - Out of memory or failed memory allocation.\n", ERR_MEM);
        break;
    case ERR_IO:
        fprintf(stderr, "Error %d: Input/Output error - Failed to read or write a file.\n", ERR_IO);
        break;
    case ERR_NULL:
        fprintf(stderr, "Error %d: Null pointer error - Dereferencing a null pointer.\n", ERR_NULL);
        break;
    case ERR_FILE:
        fprintf(stderr, "Error %d: File error - Error in file handling (e.g., cannot open or read from file).\n", ERR_FILE);
        break;
    case ERR_COMPILER_INTERNAL:
        fprintf(stderr, "Error %d: Compiler internal error - Internal error unrelated to input program (e.g., memory allocation failure).\n", ERR_COMPILER_INTERNAL);
        break;
    case ERR_SUCCESSFULL:
        fprintf(stderr, "Success %d: Program finished successfully.\n", ERR_SUCCESSFULL);
        break;
    default:
        fprintf(stderr, "Error %d: Unknown error code.\n", error_code);
        break;
    }

    // Exit the program with the error code
    exit(error_code);
}