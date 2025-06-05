/**
 * @file Code_generator.c
 * @brief Implementation of code generation for the IFJ project.
 *
 * @details This file contains functions necessary for generating intermediate code (IFJcode24).
 *
 * @author Pavel Glvač <xglvacp00>
 *
 * @note This file is part of the IFJ2024 project.
 */

#include "Code_generator.h"

/**
 * @brief Generates the header for the IFJcode24 intermediate code.
 *
 * This function outputs the initial header line and defines the result variable in the global frame.
 */
void generateHeader(){
    printf(".IFJcode24\n");
}

/**
 * @brief Declares a global variable.
 *
 * @details Handles global variable declaration in IFJcode24. Generates code for built-in functions, function calls,
 * or literal assignments.
 *
 * @param node Pointer to the binary tree node representing the variable declaration.
 * @see generateBuildInFuncions(), generateFunctionCall()
 */
void generateGlobalVarDecl(BinaryTreeNode *node) {

    BinaryTreeNode *varNode = node->right;
    if (varNode->tokenType == TOKEN_ASSIGNMENT) {
        if (strcmp(varNode->right->strValue, "ifj") == 0) {
            generateBuildInFuncions(node);
            return;
        }
        if (varNode->right && varNode->right->type == NODE_FUNC_CALL) {
            generateFunctionCall(node);
            return;
        }
    }

    const char *varName = varNode->strValue;
    printf("DEFVAR GF@%s\n", varName);
    BinaryTreeNode *valueNode = varNode->right->right->right->left;
    if (!valueNode) {
        valueNode = varNode->right->right->right->right;
        if (strcmp(valueNode->strValue, "ifj") == 0) {
            if (strcmp(valueNode->right->right->strValue, "i2f") == 0) {
                printf("MOVE GF@%s  int@%s\n", varName, valueNode->right->right->left->right->strValue);
                printf("INT2FLOAT GF@result GF@%s  \n", varName);
                printf("MOVE GF@%s  GF@result\n", varName);
                return;
            } else if (strcmp(valueNode->right->right->strValue, "f2i") == 0) {
                printf("MOVE GF@%s  float@%s\n", varName, valueNode->right->right->left->right->strValue);
                printf("FLOAT@INT GF@result GF@%s  \n", varName);
                printf("MOVE GF@%s  GF@result\n", varName);
                return;
            } else if (strcmp(valueNode->right->right->strValue, "length") == 0) {
                printf("MOVE GF@result  string@%s\n", valueNode->right->right->left->right->strValue);
                printf("STRLEN GF@%s GF@result  \n", varName);
                return;
            }else if (strcmp(valueNode->right->right->strValue, "concat") == 0) {
                printf("CONCAT GF@result  string@%s string@%s\n", valueNode->right->right->left->right->strValue, valueNode->right->right->left->right->right->right->strValue);
                printf("MOVE GF@%s GF@result  \n", varName);
                return;
            } else {
                printf("MOVE GF@%s string@%s\n", varName, valueNode->right->right->left->right->strValue);
                return;
            }
        }
    }
        switch (valueNode->tokenType) {
            case TOKEN_INT_LITERAL:
                printf("MOVE GF@%s int@%s\n", varName, valueNode->strValue);
                break;

            case TOKEN_FLOAT_LITERAL:
                printf("MOVE GF@%s float@%s\n", varName, valueNode->strValue);
                break;

            default:
                printf("MOVE GF@%s string@%s\n", varName, valueNode->strValue);
                break;
        }

}

/**
 * @brief Declares local variables.
 *
 * @details Handles local variable declaration in IFJcode24. Generates code for built-in functions, function calls,
 * or literal assignments.
 *
 * @param node Pointer to the binary tree node representing the variable declaration.
 * @see generateBuildInFuncions(), generateFunctionCall()
 */
void generateLocalVarDecl(BinaryTreeNode *node) {

    BinaryTreeNode *varNode = node->right;

    if (varNode->tokenType == TOKEN_ASSIGNMENT) {
        if(varNode->right) {
            if (strcmp(varNode->right->strValue, "ifj") == 0) {
                generateBuildInFuncions(node);
                return;
            }

            if (varNode->right && varNode->right->type == NODE_FUNC_CALL) {
                generateFunctionCall(node);
                return;
            }
            return;
        }
        return;
    }

    const char *varName = varNode->strValue;
    printf("DEFVAR LF@%s\n", varName);
    BinaryTreeNode *valueNode = varNode->right->right->right->left;
    if (!valueNode) {
        valueNode = varNode->right->right->right->right;
        if (strcmp(valueNode->strValue, "ifj") == 0) {
            if (strcmp(valueNode->right->right->strValue, "i2f") == 0) {
                printf("MOVE LF@%s  int@%s\n", varName, valueNode->right->right->left->right->strValue);
                printf("INT2FLOAT GF@result LF@%s  \n", varName);
                printf("MOVE LF@%s  GF@result\n", varName);
                return;
            } else if (strcmp(valueNode->right->right->strValue, "f2i") == 0) {
                printf("MOVE LF@%s  float@%s\n", varName, valueNode->right->right->left->right->strValue);
                printf("FLOAT@INT GF@result LF@%s  \n", varName);
                printf("MOVE LF@%s  GF@result\n", varName);
                return;
            } else if (strcmp(valueNode->right->right->strValue, "length") == 0) {
                printf("MOVE GF@result  string@%s\n", valueNode->right->right->left->right->strValue);
                printf("STRLEN LF@%s GF@result  \n", varName);
                return;
            }else if (strcmp(valueNode->right->right->strValue, "concat") == 0) {
                printf("CONCAT GF@result  string@%s string@%s\n", valueNode->right->right->left->right->strValue, valueNode->right->right->left->right->right->right->strValue);
                printf("MOVE LF@%s GF@result  \n", varName);
                return;
            }  if (strcmp(valueNode->right->right->strValue, "strcmp") == 0) {
                printf("CONCAT GF@result  string@%s string@%s\n", valueNode->right->right->left->right->strValue, valueNode->right->right->left->right->right->right->strValue);
                printf("MOVE LF@%s GF@result  \n", varName);
                return;
            }else {
                printf("MOVE LF@%s string@%s\n", varName, valueNode->right->right->left->right->strValue);
                return;
            }
        }
        switch (valueNode->tokenType) {
            case TOKEN_INT_LITERAL:
                printf("MOVE LF@%s int@%s\n", varName, valueNode->strValue);
                break;

            case TOKEN_FLOAT_LITERAL:
                printf("MOVE LF@%s float@%s\n", varName, valueNode->strValue);
                break;

            default:
                printf("MOVE LF@%s string@%s\n", varName, valueNode->strValue);
                break;
        }

    }
}

/**
 * @brief Declares a global constant.
 *
 * @details Handles global constant declaration in IFJcode24. Generates code for built-in functions, function calls,
 * or literal assignments.
 *
 * @param node Pointer to the binary tree node representing the constant declaration.
 * @see generateHeader(), generateBuildInFuncions(), generateFunctionCall()
 */
void generateGlobalConstDecl(BinaryTreeNode *node) {

    BinaryTreeNode *varNode = node->right;

    if(varNode->right->right->tokenType == TOKEN_IMPORT) {
        generateHeader();
        return;
    }

    if (varNode->tokenType == TOKEN_ASSIGNMENT) {
        if (strcmp(varNode->right->strValue, "ifj") == 0) {
            generateBuildInFuncions(node);
            return;
        }
        if (varNode->right && varNode->right->type == NODE_FUNC_CALL) {
            generateFunctionCall(node);
            return;
        }
        return;
    }

    const char *varName = varNode->strValue;
    printf("CONST GF@%s\n", varName);

    BinaryTreeNode  *valueNode = varNode->right;

    if (valueNode->tokenType == TOKEN_ASSIGNMENT) {
        valueNode = valueNode->left;
        switch (valueNode->tokenType) {
            case TOKEN_INT_LITERAL:
                printf("MOVE GF@%s int@%s\n", varName, valueNode->strValue);
                break;

            case TOKEN_FLOAT_LITERAL:
                printf("MOVE GF@%s float@%s\n", varName, valueNode->strValue);
                break;

            default:
                printf("MOVE GF@%s string@%s\n", varName, valueNode->strValue);
                break;
        }
        return;
    }

    valueNode = varNode->right->right->right->left;

    if (!valueNode) {
        if (strcmp(varNode->right->right->right->right->strValue, "ifj") == 0) {
            printf("MOVE GF@%s string@%s\n", varName,
                varNode->right->right->right->right->right->right->left->right->strValue);
            return;
        } else if (strcmp(varNode->right->right->strValue, "ifj") == 0) {
            printf("MOVE GF@%s string@%s\n", varName, varNode->right->right->right->right->left->right->strValue);
            return;
        }
        return;
    }


    switch (valueNode->tokenType) {
        case TOKEN_INT_LITERAL:
            printf("MOVE LF@%s int@%s\n", varName, valueNode->strValue);
            break;

        case TOKEN_FLOAT_LITERAL:
            printf("MOVE LF@%s float@%s\n", varName, valueNode->strValue);
            break;

        default:
            printf("MOVE LF@%s string@%s\n", varName, valueNode->strValue);
            break;
    }
}

/**
 * @brief Declares a local constant.
 *
 * @details Handles local constant declaration in IFJcode24. Generates code for built-in functions, function calls,
 * or literal assignments.
 *
 * @param node Pointer to the binary tree node representing the constant declaration.
 * @see generateBuildInFuncions(), generateFunctionCall()
 */
void generateLocalConstDecl(BinaryTreeNode *node) {

        BinaryTreeNode *varNode = node->right;
        if (varNode->tokenType == TOKEN_ASSIGNMENT) {
            if (strcmp(varNode->right->strValue, "ifj") == 0) {
                generateBuildInFuncions(node);
                return;
            }
            if (varNode->right && varNode->right->type == NODE_FUNC_CALL) {
                generateFunctionCall(node);
                return;
            }
            return;
        }

        const char *varName = varNode->strValue;
        printf("CONST LF@%s\n", varName);

    BinaryTreeNode  *valueNode = varNode->right;

        if (valueNode->tokenType == TOKEN_ASSIGNMENT) {
            valueNode = valueNode->left;
            switch (valueNode->tokenType) {
                case TOKEN_INT_LITERAL:
                    printf("MOVE LF@%s int@%s\n", varName, valueNode->strValue);
                    break;

                case TOKEN_FLOAT_LITERAL:
                    printf("MOVE LF@%s float@%s\n", varName, valueNode->strValue);
                    break;

                default:
                    printf("MOVE LF@%s string@%s\n", varName, valueNode->strValue);
                    break;
            }
            return;
        }

        valueNode = varNode->right->right->right->left;

        if (!valueNode) {
            if (strcmp(varNode->right->right->right->right->strValue, "ifj") == 0) {
                printf("MOVE LF@%s string@%s\n", varName,
                    varNode->right->right->right->right->right->right->left->right->strValue);
                return;
            } else if (strcmp(varNode->right->right->strValue, "ifj") == 0) {
                printf("MOVE LF@%s string@%s\n", varName, varNode->right->right->right->right->left->right->strValue);
                return;
            }
            return;
        }

        switch (valueNode->tokenType) {
            case TOKEN_INT_LITERAL:
                printf("MOVE LF@%s int@%s\n", varName, valueNode->strValue);
                break;

            case TOKEN_FLOAT_LITERAL:
                printf("MOVE LF@%s float@%s\n", varName, valueNode->strValue);
                break;

            default:
                printf("MOVE LF@%s string@%s\n", varName, valueNode->strValue);
                break;
        }
    }

/**
 * @brief Generates built-in functions for the given node.
 *
 * @details Handles the generation of built-in functions such as write, read, type conversion, and string operations.
 *
 * @param node Pointer to the binary tree node representing the function call.
 * @see generateHeader(), generateLocalConstDecl()
 */
void generateBuildInFuncions(BinaryTreeNode *node) {
        
        BinaryTreeNode *writeNode = node->right->right;

        if (strcmp(writeNode->strValue, "write") == 0) {
            BinaryTreeNode *argNode = writeNode->left->right;
            printf("WRITE %s\n", argNode->strValue);
            return;
        }

        BinaryTreeNode *curretntNode = node->right->right->right->right;

        if (strcmp(curretntNode->strValue, "readi32") == 0) {
            printf("READ LF/GF@%s int\n", node->strValue);
            return;
        } else if (strcmp(curretntNode->strValue, "readf64") == 0) {
            printf("READ LF/GF@%s float\n", node->strValue);
            return;
        } else if (strcmp(curretntNode->strValue, "readstr") == 0) {
            printf("READ LF/GF@%s string\n", node->strValue);
            return;
        }

        if (strcmp(curretntNode->strValue, "i2f") == 0) {
            printf("INT2FLOAT GF@result  LF/GF@%s\n", node->strValue);
            printf("MOVE LF@%s GF@result\n", node->strValue);
            return;
        }


        if (strcmp(curretntNode->strValue, "f2i") == 0) {
            printf("FLOAT2INT GF@result  LF/GF@%s\n", node->strValue);
            printf("MOVE LF@%s GF@result\n", node->strValue);
            return;
        }



        if (strcmp(curretntNode->strValue, "length") == 0) {
            printf("MOVE GF@result  string@%s\n", curretntNode->left->right->strValue);
            printf("STRLEN LF@%s GF@result  \n", node->strValue);
            return;
        }


    if (strcmp(curretntNode->strValue ,"concat") == 0) {
        printf("CONCAT GF@result  string@%s string@%s\n", curretntNode->left->right->strValue, curretntNode->left->right->right->right->strValue);
        printf("MOVE LF@%s GF@result  \n", node->strValue);
        return;
    }


    }

/**
 * @brief Generates an if statement.
 *
 * @details Handles the generation of an if statement in the IFJcode24 intermediate code. It evaluates the condition
 * and generates the corresponding labels and code blocks.
 *
 * @param node Pointer to the binary tree node representing the if statement.
 */
void generateIfStatement(BinaryTreeNode *node) {

        const char *conditionVar = generateExpression(node->right->left->left);

        static int labelCounter = 0;
        int labelNumber = labelCounter++;

        printf("LABEL $if_start_%d\n", labelNumber);
        printf("JUMPIFEQ$if_%d LF@%s\n", labelNumber, conditionVar);
        if (node->left) {
            generateBody(node->left->right->left);
            printf("JUMP $if_end_%d\n", labelNumber);
        }

        generateBody(node->right->right->left);

        printf("LABEL $if_end_%d\n", labelNumber);
    }

/**
 * @brief Generates a while statement.
 *
 * @details Handles the generation of a while statement in the IFJcode24 intermediate code. It evaluates the condition
 * at the beginning of each iteration and generates the loop body and control flow.
 *
 * @param node Pointer to the binary tree node representing the while statement.
 */
void generateWhileStatement(BinaryTreeNode *node) {

        static int labelCounter = 0;
        int labelNumber = labelCounter++;
        printf("LABEL $while_start_%d\n", labelNumber);

        BinaryTreeNode *conditionNode = node->left->left;
        const char *conditionVar = generateExpression(conditionNode);
        
        printf("JUMPIFNOT  $while_end_%d LF@%s\n", labelNumber, conditionVar);
        generateBody(node->right->left);
        printf("JUMP  $while_end_%d LF@%s\n", labelNumber, conditionVar);
        printf("LABEL $while_end_%d\n", labelNumber);
    }

/**
 * @brief Generates function parameters.
 *
 * @details Handles the generation of function parameters and the function body in IFJcode24.
 *
 * @param node Pointer to the binary tree node representing the function.
 */
void generateFunctionParams(BinaryTreeNode *node) {

        BinaryTreeNode *fnNameNode = node->right->right;
        const char *functionName = fnNameNode->strValue;

        printf("LABEL $%s\n", functionName);
        printf("CREATEFRAME\n");
        printf("PUSHFRAME\n");

        BinaryTreeNode *paramNode = fnNameNode->left;

        paramNode = paramNode->right;
        while (paramNode && paramNode->tokenType != TOKEN_RPAREN) {
            if (paramNode->tokenType == TOKEN_IDENTIFIER) {
                const char *paramName = paramNode->strValue;
                printf("DEFVAR LF@%s\n", paramName);
            }
            paramNode = paramNode->right;
        }
        fnNameNode = node->right->right->right->right;
        generateBody(fnNameNode);
        if (strcmp(node->right->right->right->strValue, "void") == 0) {
            printf("POPFRAME\n");
            printf("RETURN\n");
        }
    }

/**
 * @brief Generates the end of a function.
 *
 * @details Handles the return value and cleans up the function's stack frame in IFJcode24.
 *
 * @param node Pointer to the binary tree node representing the function's return statement.
 */
void generateFunctionEnd(BinaryTreeNode *node) {

        BinaryTreeNode *returnNode = node->left;

        const char *returnValue = generateExpression(returnNode);
        if (!returnValue) {
            printf("RETURN\n");
            printf("POPFRAME\n");
            return;
        }

        printf("RETURN LF@%s\n", returnValue);
        printf("POPFRAME\n");
    }

/**
 * @brief Generates an expression.
 *
 * @details Evaluates an expression node and generates the corresponding intermediate code for the IFJcode24 language.
 * Supports variables, constants, and operations.
 *
 * @param node Pointer to the binary tree node representing the expression.
 * @return The name of the variable storing the result of the expression.
 */
const char *generateExpression(BinaryTreeNode *node) {

        if (!node) {
            return NULL;
        }

        if (node->type == NODE_VAR || node->type == NODE_CONST) {
            return node->strValue;
        }

        if (node->type == NODE_OP) {
            const char *leftOperand = generateExpression(node->left);
            const char *rightOperand = generateExpression(node->right);

            static int tempVarCounter = 0;
            char *resultVar = malloc(20);
            sprintf(resultVar, "temp_var_e%d", tempVarCounter++);

            printf("DEFVAR LF@%s\n", resultVar);

            switch (node->tokenType) {
                case TOKEN_ADDITION:
                    printf("ADD LF@%s LF@%s LF@%s\n", resultVar, leftOperand, rightOperand);
                    break;
                case TOKEN_SUBTRACTION:
                    printf("SUB LF@%s LF@%s LF@%s\n", resultVar, leftOperand, rightOperand);
                    break;
                case TOKEN_MULTIPLY:
                    printf("MUL LF@%s LF@%s LF@%s\n", resultVar, leftOperand, rightOperand);
                    break;
                case TOKEN_DIVISION:
                    printf("DIV LF@%s LF@%s LF@%s\n", resultVar, leftOperand, rightOperand);
                    break;
                case TOKEN_EQUAL:
                    printf("EQ LF@%s LF@%s LF@%s\n", resultVar, leftOperand, rightOperand);
                    break;
                case TOKEN_NOT_EQUAL:
                    printf("NEQ LF@%s LF@%s LF@%s\n", resultVar, leftOperand, rightOperand);
                    break;
                case TOKEN_LESS_THAN:
                    printf("LT LF@%s LF@%s LF@%s\n", resultVar, leftOperand, rightOperand);
                    break;
                case TOKEN_LESS_EQUAL:
                    printf("LEQ LF@%s LF@%s LF@%s\n", resultVar, leftOperand, rightOperand);
                    break;
                case TOKEN_GREATER_EQUAL:
                    printf("GEQ LF@%s LF@%s LF@%s\n", resultVar, leftOperand, rightOperand);
                    break;
                case TOKEN_GREATER_THAN:
                    printf("GT LF@%s LF@%s LF@%s\n", resultVar, leftOperand, rightOperand);
                    break;
                default:
                    free(resultVar);
                    return NULL;
            }
            return resultVar;
        }
        return NULL;
    }

/**
 * @brief Generates a function call.
 *
 * @details Handles the generation of a function call by pushing function parameters onto the stack,
 * calling the function, and handling the return value.
 *
 * @param node Pointer to the binary tree node representing the function call.
 */
void generateFunctionCall(BinaryTreeNode *node) {

        BinaryTreeNode *functionNameNode = node->right->right;
        const char *functionName = functionNameNode->strValue;
        BinaryTreeNode *functionParams = functionNameNode->left->right;
        while (functionParams->tokenType == TOKEN_IDENTIFIER) {
            printf("PUSH LF@%s\n", functionParams->strValue);
            functionParams = functionParams->right;
            if (functionParams->tokenType == TOKEN_COMMA) {
                functionParams = functionParams->right;
            }
        }
        printf("CALL  $%s\n", functionName);
        printf("POPS  LF@%s\n", node->strValue);
    }

/**
 * @brief Generates the body of a function or statement block.
 *
 * @details Handles the generation of different types of nodes including variables, constants, loops,
 * and conditional statements in IFJcode24.
 *
 * @param node Pointer to the binary tree node representing the body.
 */
void generateBody(BinaryTreeNode *node) {
        if (!node) {
            return;
        }
        switch (node->type) {
            case NODE_VAR:
                if (strcmp(node->strValue, "return") == 0) {
                    generateFunctionEnd(node);
                } else if (strcmp(node->strValue, "while") == 0) {
                    generateWhileStatement(node);
                } else if (strcmp(node->strValue, "ifj") == 0) {
                    generateBuildInFuncions(node);
                } else if (strcmp(node->strValue, "if") == 0) {
                    generateIfStatement(node);
                } else {
                    generateLocalVarDecl(node);
                }
                break;
            case NODE_CONST:
                if (node->tokenType == TOKEN_KEYWORD) {
                    generateLocalConstDecl(node);
                }
                break;
            case NODE_OP:
                generateExpression(node);
                break;
            case NODE_GENERAL:
                if (node->tokenType == TOKEN_EMPTY) {
                    if (!node->right) {
                        return;
                    }
                    generateBody(node->right);
                }
                break;
            case NODE_FUNC_DEF:
                generateFunctionParams(node);
                break;
            default:
                break;
        }
        if (node->left && !strcmp(node->strValue, "while") == 0 && !strcmp(node->strValue, "return") == 0 &&
            !strcmp(node->strValue, "if") == 0) {
            generateBody(node->left);
        }
    }

/**
 * @brief Processes the token type of a given binary tree node.
 *
 * @details This function processes the type of the binary tree node, handling different cases such as variable declarations,
 * function definitions, constants, control flow structures, and general nodes.
 *
 * @param node Pointer to the binary tree node to be processed.
 */
void processTokenType(BinaryTreeNode *node) {
        if (!node) {
            return; 
        }
        switch (node->type) {
            case NODE_VAR_DECL:
                generateGlobalVarDecl(node);
                break;
            case NODE_VAR:
                if (node->tokenType == TOKEN_KEYWORD) {
                    generateLocalVarDecl(node);
                }
                break;
            case NODE_FUNC_DEF:
                generateFunctionParams(node);
                break;
            case NODE_CONST:
                generateGlobalConstDecl(node); 
                break;
            case NODE_IF:
                generateIfStatement(node);
                break;
            case NODE_WHILE:
                generateWhileStatement(node);
                break;
            case NODE_OP:
                generateExpression(node);
                break;
            case NODE_GENERAL:
                if (node->right) processTokenType(node->right);
                break;
            default:
                printf("Unhandled node type: %s\n", node->strValue ? node->strValue : "NULL");
                break;
        }
        if (!node->left) {
            return;
        }
        processTokenType(node->left);
    }
