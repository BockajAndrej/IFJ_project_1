#include "Code_generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#include <limits.h>

#define INTMAX INT_MAX
#define INTMIN INT_MIN

void generateHeader(){
    printf(".IFJcode24\n");
}


//-------------------VAR DEC----------------------
void generateGlobalVarDecl(BinaryTreeNode *node) {
    printf("------------VAR DECLARATION------------\n");
    if (!node || node->type != NODE_VAR_DECL) {
        printf("Error: Invalid global variable declaration.\n");
        return;
    }

    // Extract variable name
    BinaryTreeNode *varNode = node->right; // Should be NODE_VAR ("d")

    const char *varName = varNode->strValue;
    printf("DEFVAR GF@%s\n", varName); // Declare global variable

    // Navigate to the colon (:) and then the type

    BinaryTreeNode *colonNode = varNode->right; // Should be NODE_VAR (":")
    BinaryTreeNode *typeNode = colonNode->right; // Should be NODE_VAR ("f64")
    // Navigate to the assignment (=) and then the value
    BinaryTreeNode *assignNode = typeNode->right; // Should be NODE_VAR ("=")
    BinaryTreeNode *valueNode = assignNode->left; // Should be NODE_VAR ("0.123")


    // Handle the assigned value based on its type
    switch (valueNode->tokenType) {
        case TOKEN_INT_LITERAL:
            printf("MOVE GF@%s int@%s\n", varName, valueNode->strValue);
            break;

        case TOKEN_FLOAT_LITERAL:
            printf("MOVE GF@%s float@%s\n", varName, valueNode->strValue);
            break;

        case TOKEN_STRING_LITERAL:
            printf("MOVE GF@%s string@%s\n", varName, valueNode->strValue);
            break;

        default:
            printf("Error: Unsupported value type for assignment.\n");
            break;
    }
}

void generateLocalVarDecl(BinaryTreeNode *node) {
    if (!node || node->type != NODE_VAR) {
        printf("Error: Invalid local variable declaration.\n");
        return;
    }

    // Extract variable name
    BinaryTreeNode *varNode = node->right; // Should be NODE_VAR ("d")
    if(varNode->tokenType == TOKEN_ASSIGNMENT){
        generateAssignment(node);
    }
    if (!varNode || varNode->tokenType != TOKEN_IDENTIFIER) {
        printf("Error: Invalid variable name.\n");
        return;
    }

    const char *varName = varNode->strValue;
    printf("DEFVAR LF@%s\n", varName); // Declare local variable

    // Navigate to the colon (:) and then the type
    BinaryTreeNode *colonNode = varNode->right; // Should be NODE_VAR (":")
    if (!colonNode || colonNode->tokenType != TOKEN_COLON) {
        printf("Error: Missing colon after variable name.\n");
        return;
    }

    BinaryTreeNode *typeNode = colonNode->right; // Should be NODE_VAR ("f64")
    if (!typeNode || typeNode->tokenType != TOKEN_KEYWORD) {
        printf("Error: Missing or invalid type declaration.\n");
        return;
    }

    // Navigate to the assignment (=) and then the value
    BinaryTreeNode *assignNode = typeNode->right; // Should be NODE_VAR ("=")
    if (!assignNode || assignNode->tokenType != TOKEN_ASSIGNMENT) {
        printf("Error: Missing assignment '=' after type.\n");
        return;
    }

    BinaryTreeNode *valueNode = assignNode->left; // Should be NODE_VAR ("0.123")
    if (!valueNode) {
        printf("Error: Missing value for assignment.\n");
        return;
    }

    // Handle the assigned value based on its type
    switch (valueNode->tokenType) {
        case TOKEN_INT_LITERAL:
            printf("MOVE LF@%s int@%s\n", varName, valueNode->strValue);
            break;

        case TOKEN_FLOAT_LITERAL:
            printf("MOVE LF@%s float@%s\n", varName, valueNode->strValue);
            break;

        case TOKEN_STRING_LITERAL:
            printf("MOVE LF@%s string@%s\n", varName, valueNode->strValue);
            break;

        default:
            printf("Error: Unsupported value type for assignment.\n");
            break;
    }
}


//-------------------CONST DEC----------------------
void generateGlobalConstDecl(BinaryTreeNode *node) {
    printf("------------CONST DECLARATION------------\n");

    // Overenie, či ide o uzol konštanty
    if (!node || node->type != NODE_CONST) {
        printf("Chyba: Neplatná deklarácia konštanty.\n");
        return;
    }

    // Extrakcia názvu konštanty
    BinaryTreeNode *nameNode = node->right; // Malo by byť meno konštanty
    if (!nameNode || nameNode->tokenType != TOKEN_IDENTIFIER) {
        printf("Chyba: Neplatné meno konštanty.\n");
        return;
    }

    const char *constName = nameNode->strValue;
    printf("CONST GF@%s\n", constName); // Deklarujeme konštantu v lokálnom rámci

    // Skontrolujeme, či za názvom konštanty nasleduje "="
    BinaryTreeNode *assignNode = nameNode->right; // Malo by byť "="
    if (!assignNode || assignNode->tokenType != TOKEN_ASSIGNMENT) {
        printf("Chyba: Očakáva sa '=' po mene konštanty.\n");
        return;
    }

    // Extrakcia hodnoty priradenej konštante
    BinaryTreeNode *valueNode = assignNode->left; // Malo by byť hodnota
    if (!valueNode) {
        printf("Chyba: Chýba hodnota pre konštantu.\n");
        return;
    }

    // Generovanie priradenia na základe typu hodnoty
    switch (valueNode->tokenType) {
        case TOKEN_INT_LITERAL:
            printf("MOVE GF@%s int@%s\n", constName, valueNode->strValue);
            break;

        case TOKEN_FLOAT_LITERAL:
            printf("MOVE GF@%s float@%s\n", constName, valueNode->strValue);
            break;

        case TOKEN_STRING_LITERAL:
            printf("MOVE GF@%s string@%s\n", constName, valueNode->strValue);
            break;

        case TOKEN_IDENTIFIER: // Ak hodnota je výsledok funkcie alebo premennej
            printf("MOVE GF@%s GF@%s\n", constName, valueNode->strValue);
            break;

        default:
            printf("Chyba: Nepodporovaný typ hodnoty pre konštantu.\n");
            break;
    }
}

void generateLocalConstDecl(BinaryTreeNode *node) {
    printf("------------CONST DECLARATION------------\n");

    // Overenie, či ide o uzol konštanty
    if (!node || node->type != NODE_CONST) {
        printf("Chyba: Neplatná deklarácia konštanty.\n");
        return;
    }

    // Extrakcia názvu konštanty
    BinaryTreeNode *nameNode = node->right; // Malo by byť meno konštanty
    if (!nameNode || nameNode->tokenType != TOKEN_IDENTIFIER) {
        printf("Chyba: Neplatné meno konštanty.\n");
        return;
    }

    const char *constName = nameNode->strValue;
    printf("CONST LF@%s\n", constName); // Deklarujeme konštantu v lokálnom rámci

    // Skontrolujeme, či za názvom konštanty nasleduje "="
    BinaryTreeNode *assignNode = nameNode->right; // Malo by byť "="
    if (!assignNode || assignNode->tokenType != TOKEN_ASSIGNMENT) {
        printf("Chyba: Očakáva sa '=' po mene konštanty.\n");
        return;
    }

    // Extrakcia hodnoty priradenej konštante
    BinaryTreeNode *valueNode = assignNode->left; // Malo by byť hodnota
    if (!valueNode) {
        printf("Chyba: Chýba hodnota pre konštantu.\n");
        return;
    }

    // Generovanie priradenia na základe typu hodnoty
    switch (valueNode->tokenType) {
        case TOKEN_INT_LITERAL:
            printf("MOVE LF@%s int@%s\n", constName, valueNode->strValue);
            break;

        case TOKEN_FLOAT_LITERAL:
            printf("MOVE LF@%s float@%s\n", constName, valueNode->strValue);
            break;

        case TOKEN_STRING_LITERAL:
            printf("MOVE LF@%s string@%s\n", constName, valueNode->strValue);
            break;

        case TOKEN_IDENTIFIER: // Ak hodnota je výsledok funkcie alebo premennej
            printf("MOVE LF@%s LF@%s\n", constName, valueNode->strValue);
            break;

        default:
            printf("Chyba: Nepodporovaný typ hodnoty pre konštantu.\n");
            break;
    }
}

//-------------------BUILD IN FUNCTIONS----------------------
void generateBuildInFuncions(BinaryTreeNode *node){
    if (!node || node->type != NODE_VAR || node->tokenType != TOKEN_IDENTIFIER) {
        printf( "Error: Invalid node for build in function.\n");
        return;
    }
    BinaryTreeNode *dotNode=node->right;
    if (!dotNode || dotNode->type != NODE_FUNC_CALL || dotNode->tokenType != TOKEN_DOT) {
        printf( "Error: Invalid node for build in function.\n");
        return;
    }

    BinaryTreeNode *fnName=dotNode->right;
    if (strcmp(fnName->strValue, "write") == 0) {
        printf("WRITE %s\n",fnName->left->right->strValue);    //TODO: doplnit typ
    }
}

//-------------------IF DEC----------------------
void generateIfStatement(BinaryTreeNode *node) {
    if (!node || node->type != NODE_IF) {
        printf( "Error: Invalid node for if statement.\n");
        return;
    }

    // Generovanie podmienky - využite `generateExpression`
    const char *conditionVar = generateExpression(node->left);
    if (!conditionVar) {
        printf( "Error: Failed to generate condition for if statement.\n");
        return;
    }

    // Generovanie kódu pre podmienku
    static int labelCounter = 0;
    int labelNumber = labelCounter++;

    printf("LABEL $if_start_%d\n", labelNumber);
    printf("JUMPIFNOT $if_else_%d LF@%s\n", labelNumber, conditionVar);

    // Generovanie tela podmienky
    generateBody(node->right);

    // Generovanie štítku pre else a koniec if
    //printf("LABEL $if_else_%d\n", labelNumber);
    printf("LABEL $if_end_%d\n", labelNumber);
}


//-------------------WHILE DEC----------------------
void generateWhileStatement(BinaryTreeNode *node) {
    printf("------------WHILE DEF------------\n");
    //printf("%s\n",node->strValue);
    if (!node || node->type != NODE_VAR || node->tokenType != TOKEN_KEYWORD) {
        printf( "Error: Invalid node for while statement.\n");
        return;
    }

    // Generovanie štítkov pre začiatok a koniec cyklu
    static int labelCounter = 0;
    int labelNumber = labelCounter++;
    printf("LABEL $while_start_%d\n", labelNumber);


    printf("------------WHILE STATEMENT------------\n");
    // Generovanie podmienky - spracovanie uzla obsahujúceho podmienku
    BinaryTreeNode *conditionNode = node->left->left; // Predpokladáme, že ľavý poduzol obsahuje podmienku
    const char *conditionVar = generateExpression(conditionNode);
    if (!conditionVar) {
        printf( "Error: Failed to generate condition for while statement.\n");
        return;
    }
    printf("JUMPIFNOT  $while_end_%d LF@%s\n", labelNumber, conditionVar);

    printf("------------WHILE BODY------------\n");
    generateBody(node->right->left);

    printf("JUMP  $while_end_%d LF@%s\n", labelNumber, conditionVar);
    printf("------------WHILE END------------\n");
    printf("LABEL $while_end_%d\n", labelNumber);
}


//-------------------FUNC DEC----------------------
void generateFunctionParams(BinaryTreeNode *node) {
    printf("------------FUNCTION DEF------------\n");

    if (!node || node->type != NODE_FUNC_DEF || node->tokenType != TOKEN_KEYWORD) {
        printf( "Error: Invalid function definition node.\n");
        return;
    }

    // Extrahovanie názvu funkcie
    BinaryTreeNode *fnNameNode = node->right->right;
    if (!fnNameNode || fnNameNode->tokenType != TOKEN_IDENTIFIER) {
        printf( "Error: Missing or invalid function name.\n");
        return;
    }
    const char *functionName = fnNameNode->strValue;

    // Generovanie LABEL pre funkciu
    printf("LABEL $%s\n", functionName);
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    printf("------------FUNCTION PARAMS------------\n");
    // Extrahovanie parametrov
    BinaryTreeNode *paramNode = fnNameNode->left; // Doľava na parametre
    if (!paramNode || paramNode->tokenType != TOKEN_LPAREN) {
        printf( "Error: Missing function parameters.\n");
        return;
    }

    // Prechádzame parametre
    //int paramIndex = 1; // Index parametra (začína od 1)
    paramNode = paramNode->right; // Presunieme sa na prvý parameter
    while (paramNode && paramNode->tokenType != TOKEN_RPAREN) {
        if (paramNode->tokenType == TOKEN_IDENTIFIER) {
            const char *paramName = paramNode->strValue;

            // Generovanie príkazu pre parameter
            printf("DEFVAR LF@%s\n", paramName);
            //printf("MOVE LF@%s LF@%d\n", paramName, paramIndex);
            //paramIndex++;
        }
        paramNode = paramNode->right; // Posun na ďalší parameter
    }

    fnNameNode = node->right->right->right->right;
    //  if (!fnNameNode || fnNameNode->tokenType != TOKEN_CURLYL_BRACKET) {
    // printf( "Error: Missing or invalid function name.\n");
    // return;
    //}
    //printf("token %s\n",fnNameNode->strValue);
    printf("------------FUNCTION BODY------------\n");
    generateBody(fnNameNode);
    printf("------------FUNCTION END------------\n");
    generateFunctionEnd(); //pozriet Return

}

void generateFunctionEnd() {
    printf("POPFRAME\n");
    printf("RETURN\n");
}

//-------------------RETURN DEC----------------------
void generateReturn(BinaryTreeNode *node){
    if (!node || node->type != NODE_VAR || node->tokenType != TOKEN_KEYWORD) {
        printf( "Error: Invalid node for return statement.\n");
        return;
    }

    BinaryTreeNode *returnNode = node->right;
    if (!returnNode) {
        printf( "Error: Missing return value.\n");
        return;
    }

    const char *returnValue = generateExpression(returnNode);
    if (!returnValue) {
        printf( "Error: Failed to generate return value.\n");
        return;
    }

    printf("RETURN LF@%s\n", returnValue);
}

//-------------------BODY DEC----------------------
void generateBody(BinaryTreeNode *node) {
    if (!node) {
        return; // Ak je uzol NULL, nič nerob
    }

    //printf("------------BODY GEN------------\n");
    //printf("Spracovávam uzol s tokenom: %s, typ uzla: %s\n",
    //    token_type_to_string(node->tokenType),
    //    NodeTypeToString(node->type));

    // Spracovanie na základe typu tokenu
    switch (node->type) {
        case NODE_VAR:
            if (strcmp(node->strValue, "return") == 0) {
                printf("return\n");
                generateReturn(node);
            } else if (strcmp(node->strValue, "while") == 0) {
                generateWhileStatement(node);
            }else if (strcmp(node->strValue, "ifj") == 0) {
                generateBuildInFuncions(node);
            } else {
                generateLocalVarDecl(node);
            }
            break;
        case NODE_CONST:
            if(node->tokenType==TOKEN_IDENTIFIER){
                generateLocalConstDecl(node);
            }
            break;

        case NODE_ASSIGN:
            generateAssignment(node);
            break;

        case NODE_OP:
            generateExpression(node);
            break;

        case NODE_GENERAL:
            if (node->tokenType == TOKEN_EMPTY) {
                if (!node->right) {
                    printf("konecRbody\n");
                    return;
                }
                generateBody(node->right);
            }
            break;

        case NODE_FUNC_DEF:
            generateFunctionParams(node);
            break;

        case NODE_IF:

            break;

        default:
            printf( "Unhandled node type: %s\n", NodeTypeToString(node->type));
            break;
    }

    // Rekurzívne spracovanie podstromov

    if (node->left  && !strcmp(node->strValue, "while")==0) {

        generateBody(node->left);
    }
}


//-------------------EXPRESIONS/ASSIGMENTS/FUNC CALL----------------------
const char* generateExpression(BinaryTreeNode *node) {

    //printf("%s\n",node->strValue);
    if (!node) {
        printf( "Error: Null node in expression.\n");
        return NULL;
    }

    // Ak je uzol typu VAR alebo LITERAL, priamo vrátime jeho hodnotu
    if (node->type == NODE_VAR || node->type == NODE_CONST) {
        return node->strValue;
    }

    // Ak ide o operáciu, spracujeme jej operandy
    if (node->type == NODE_OP) {
        const char *leftOperand = generateExpression(node->left);
        const char *rightOperand = generateExpression(node->right);

        if (!leftOperand || !rightOperand) {
            printf( "Error: Failed to generate operands for expression.\n");
            return NULL;
        }

        // Výstupná premenná
        static int tempVarCounter = 0;
        char *resultVar = malloc(20);
        sprintf(resultVar, "temp_var_%d", tempVarCounter++);

        printf("DEFVAR LF@%s\n", resultVar);

        // Generovanie kódu na základe operácie
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
                printf( "Error: Unsupported operation '%s'.\n", token_type_to_string(node->tokenType));
                free(resultVar);
                return NULL;
        }

        return resultVar;
    }

    printf( "Error: Unsupported node type in expression.\n");
    return NULL;
}

void generateAssignment(BinaryTreeNode *node) {
    printf("%s start of assigment\n", node->strValue);

    // Cieľová premenná (ľavý poduzol)
    BinaryTreeNode *targetNode = node->left;
    if (!targetNode || targetNode->type != NODE_VAR || targetNode->tokenType != TOKEN_IDENTIFIER) {
    printf( "Error: Invalid target for assignment. Expected a variable.\n");
        return;
    }

    const char *targetVar = targetNode->strValue;
    if (!targetVar) {
        printf( "Error: Missing target variable for assignment.\n");
        return;
    }

    // Generovanie výrazu alebo funkčného volania pre pravú stranu
    BinaryTreeNode *rightNode = node->right;
    const char *exprVar = NULL;

    if (rightNode->type == NODE_FUNC_CALL) {
        // Spracovanie funkčného volania na pravej strane
        exprVar = generateFunctionCall(rightNode);
    } else {
        // Spracovanie výrazu na pravej strane
        exprVar = generateExpression(rightNode);
    }

    // Generovanie priradenia
    if (exprVar) {
        printf("MOVE LF@%s LF@%s\n", targetVar, exprVar);
    } else {
        printf( "Error: Failed to generate right-hand side for assignment.\n");
    }
}

const char* generateFunctionCall(BinaryTreeNode *node) {
    if (!node || node->type != NODE_FUNC_CALL) {
        printf( "Error: Invalid node for function call.\n");
        return NULL;
    }

    // Názov funkcie, ktorú voláme
    BinaryTreeNode *functionNameNode = node->left;
    if (!functionNameNode || functionNameNode->tokenType != TOKEN_IDENTIFIER) {
        printf( "Error: Invalid function name for function call.\n");
        return NULL;
    }
    const char *functionName = functionNameNode->strValue;

    // Pripravíme dočasnú premennú na uloženie návratovej hodnoty
    static int tempVarCounter = 0;
    char *resultVar = malloc(20);
    sprintf(resultVar, "temp_var_%d", tempVarCounter++);

    printf("DEFVAR LF@%s\n", resultVar);

    // Generovanie parametrov funkcie
    BinaryTreeNode *paramNode = node->right;
    int paramCounter = 1;
    while (paramNode) {
        const char *paramVar = generateExpression(paramNode);
        if (paramVar) {
            printf("PUSHS LF@%s\n", paramVar);
        } else {
            printf( "Error: Failed to generate function parameter.\n");
        }
        paramNode = paramNode->right; // Predpokladáme, že parametre sú v zozname
        paramCounter++;
    }

    // Generovanie samotného volania funkcie
    printf("CALL %s\n", functionName);

    // Uloženie návratovej hodnoty z funkcie do dočasnej premennej
    printf("POPS LF@%s\n", resultVar);

    return resultVar;
}




//-------------------PROCES DEC----------------------
void processTokenType(BinaryTreeNode *node) {
    if (!node) {
        return; // Ak je uzol NULL, nič nerob
    }
    //printf("aktualny token vchadzajuci do switchu %s\n",token_type_to_string(node->tokenType));
    // Spracovanie uzla na základe tokenType

    switch (node->type) {
        case NODE_VAR_DECL:
            generateGlobalVarDecl(node);
            break;

        case NODE_VAR:
            if (node->tokenType == TOKEN_KEYWORD) {
                generateLocalVarDecl(node);
            } else if (node->tokenType == TOKEN_IDENTIFIER) {
                printf("// Identifier: %s\n", node->strValue); // Debug alebo iné spracovanie
            }
            break;

        case NODE_FUNC_DEF:
            generateFunctionParams(node);
            break;

        case NODE_CONST:
            generateGlobalConstDecl(node);      //mozny problem s konstantami
            break;

        case NODE_ASSIGN:
            generateAssignment(node);
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
            // Pokračovanie v prechode stromom, ak uzol nemá špecifické spracovanie
            if (node->right) processTokenType(node->right);
            break;

        default:
            printf("Unhandled node type: %s\n", node->strValue ? node->strValue : "NULL");
            break;
    }
    if(!node->left){
        printf("koncnaL\n");
        return;
    }
    processTokenType(node->left);


}
//TODO: vyriesit pomuzivanie zadeklarovanych premennych  napr pouzitie z parametrov funkcie...atd