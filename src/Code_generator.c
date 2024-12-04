#include "Code_generator.h"

void generateHeader(){
    printf(".IFJcode24\n");
}


//-------------------VAR DEC----------------------
void generateGlobalVarDecl(BinaryTreeNode *node) {
    //printf("------------VAR DECLARATION------------\n");
    if (!node || node->type != NODE_VAR_DECL) {
        printf("Error: Invalid global variable declaration.\n");
        return;
    }

    // Extract variable name
    BinaryTreeNode *varNode = node->right; // Should be NODE_VAR ("d")

    if(varNode->tokenType == TOKEN_ASSIGNMENT){
        if(varNode->right && varNode->right->type == NODE_FUNC_CALL){
            generateFunctionCall(node);
            return;
        }
        generateAssignment(node);
        return;
    }

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

    // Extract variable name
    BinaryTreeNode *varNode = node->right; // Should be NODE_VAR ("d")

    if(varNode->tokenType == TOKEN_ASSIGNMENT){
        if(varNode->right && varNode->right->type == NODE_FUNC_CALL){
            generateFunctionCall(node);
            return;
        }
        generateAssignment(node);
        return;
    }

    const char *varName = varNode->strValue;
    printf("DEFVAR LF@%s\n", varName); // Declare local variable
    // Navigate to the colon (:) and then the type
    BinaryTreeNode *colonNode = varNode->right; // Should be NODE_VAR (":")
    BinaryTreeNode *typeNode = colonNode->right; // Should be NODE_VAR ("f64")
    // Navigate to the assignment (=) and then the value
    BinaryTreeNode *assignNode = typeNode->right; // Should be NODE_VAR ("=")
    BinaryTreeNode *valueNode = assignNode->left; // Should be NODE_VAR ("0.123")
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

//TODO: pozriet sa na const nieco nefunguje
//-------------------CONST DEC----------------------
void generateGlobalConstDecl(BinaryTreeNode *node) {
    //printf("------------CONST DECLARATION------------\n");



    // Extrakcia názvu konštanty
    BinaryTreeNode *nameNode = node->right; // Malo by byť meno konštanty


    const char *constName = nameNode->strValue;
    printf("CONST GF@%s\n", constName); // Deklarujeme konštantu v lokálnom rámci

    // Skontrolujeme, či za názvom konštanty nasleduje "="
    BinaryTreeNode *assignNode = nameNode->right; // Malo by byť "="


    // Extrakcia hodnoty priradenej konštante
    BinaryTreeNode *valueNode = assignNode->left; // Malo by byť hodnota


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

        default:
            printf("Chyba: Nepodporovaný typ hodnoty pre konštantu.\n");
            break;
    }
}

void generateLocalConstDecl(BinaryTreeNode *node) {
    //printf("------------CONST DECLARATION------------\n");

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
//todo: dorobit podporovane ifj.funkcie
//-------------------BUILD IN FUNCTIONS----------------------
void generateBuildInFuncions(BinaryTreeNode *node){
    BinaryTreeNode *writeNode=node->right->right;
    if (strcmp(writeNode->strValue, "write") == 0) {
        // Prejdeme argumenty funkcie "write"
        BinaryTreeNode *argNode = writeNode->left->right; // Prvý argument je vľavo

        switch (argNode->tokenType)
        {       //TODO: problem pre vypis ifj.write kvoli tomu ze premenne to berie ako fuc call a nie ako int/float premennu
            case TOKEN_INT_LITERAL:
                printf("WRITE int@%s\n", argNode->strValue);
                break;
            case TOKEN_FLOAT_LITERAL:
                printf("WRITE float@%s\n", argNode->strValue);
                break;
            default:
                printf("WRITE string@%s\n", argNode->strValue);
                break;
        }
    }
    //BinaryTreeNode *readNode = node->right->right;
    //printf("%s\n",readNode->strValue);
    //if(strcmp(readNode->strValue, "readInt") == 0){
    //    printf("READ int@%s\n", readNode->left->strValue);
    //}else if(strcmp(readNode->strValue, "readString") == 0){
    //    printf("READ string@%s\n", readNode->left->strValue);
    //}else if(strcmp(readNode->strValue, "readFloat") == 0){
    //    printf("READ float@%s\n", readNode->left->strValue);
    // }
}

//-------------------IF DEC----------------------
void generateIfStatement(BinaryTreeNode *node) {
    //printf("------------IF DEF------------\n");

    // Generovanie podmienky - využite `generateExpression`
    const char *conditionVar = generateExpression(node->right->left->left);


    // Generovanie kódu pre podmienku
    static int labelCounter = 0;
    int labelNumber = labelCounter++;

    printf("LABEL $if_start_%d\n", labelNumber);
    printf("JUMPIFEQ$if_%d LF@%s\n", labelNumber, conditionVar);
    //printf("------------ELSE  BODY------------\n");
    generateBody(node->left->right->left);
    printf("JUMP $if_end_%d\n", labelNumber);
    // Generovanie tela podmienky
    //printf("------------IF BODY------------\n");

    //printf("%s\n",node->right->right->left->strValue);
    generateBody(node->right->right->left);

    // Generovanie štítku pre else a koniec if
    //printf("LABEL $if_else_%d\n", labelNumber);
    printf("LABEL $if_end_%d\n", labelNumber);
}


//-------------------WHILE DEC----------------------
void generateWhileStatement(BinaryTreeNode *node) {
    //printf("------------WHILE DEF------------\n");
    //printf("%s\n",node->strValue);
    if (!node || node->type != NODE_VAR || node->tokenType != TOKEN_KEYWORD) {
        printf( "Error: Invalid node for while statement.\n");
        return;
    }

    // Generovanie štítkov pre začiatok a koniec cyklu
    static int labelCounter = 0;
    int labelNumber = labelCounter++;
    printf("LABEL $while_start_%d\n", labelNumber);


    //printf("------------WHILE STATEMENT------------\n");
    // Generovanie podmienky - spracovanie uzla obsahujúceho podmienku
    BinaryTreeNode *conditionNode = node->left->left; // Predpokladáme, že ľavý poduzol obsahuje podmienku
    const char *conditionVar = generateExpression(conditionNode);
    if (!conditionVar) {
        printf( "Error: Failed to generate condition for while statement.\n");
        return;
    }
    printf("JUMPIFNOT  $while_end_%d LF@%s\n", labelNumber, conditionVar);

    //printf("------------WHILE BODY------------\n");
    generateBody(node->right->left);

    printf("JUMP  $while_end_%d LF@%s\n", labelNumber, conditionVar);
    //printf("------------WHILE END------------\n");
    printf("LABEL $while_end_%d\n", labelNumber);
}


//-------------------FUNC DEC----------------------
void generateFunctionParams(BinaryTreeNode *node) {
    //printf("------------FUNCTION DEF------------\n");

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

    //printf("------------FUNCTION PARAMS------------\n");
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
    //printf("------------FUNCTION BODY------------\n");
    generateBody(fnNameNode);
    if(strcmp(node->right->right->right->strValue, "void")==0){
        printf("POPFRAME\n");
        printf("RETURN\n");
    }
    //TODO: ked je void funkcia treba spravit popframe

}

void generateFunctionEnd(BinaryTreeNode *node) {
        //printf("------------RETURN------------\n");
        BinaryTreeNode *returnNode = node->left;
        //printf("%s\n",returnNode->strValue);

        const char *returnValue = generateExpression(returnNode);
        if (!returnValue) {
            printf("RETURN\n");
            printf("POPFRAME\n");
            return;
        }

        printf("RETURN LF@%s\n", returnValue);
        printf("POPFRAME\n");
    }

//-------------------EXPRESIONS/ASSIGMENTS/FUNC CALL----------------------
const char* generateExpression(BinaryTreeNode *node) {

    //printf("%s\n",node->strValue);
    if (!node) {
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

//TODO: ARITMETICKA LOGIKA
void generateAssignment(BinaryTreeNode *node) {

    //printf("%s start of assigment\n", node->strValue);
    //printf("%s start of assigment\n", node->right->strValue);
    //BinaryTreeNode *varNode = node->right->left;

    Stack stack;
    Stack_item  curretItem;
    initStack(&stack, rule);


    setStartNodeInOrder(node->right->left);
    InOrder(curInOrderNode, &stack);

    //printf("printing stack\n");
    //PrintAllStack(&stack);

    RemoveTop(&stack,  &curretItem);
    //curretItem.data.token_val.valueString.str;
    //curretItem.data.token_type;

}

void generateFunctionCall(BinaryTreeNode *node) {


    // Názov funkcie, ktorú voláme
    BinaryTreeNode *functionNameNode = node->right->right;

    const char *functionName = functionNameNode->strValue;

    BinaryTreeNode *functionParams = functionNameNode->left->right;
    while(functionParams->tokenType == TOKEN_IDENTIFIER){
        printf("PUSH LF@%s\n",functionParams->strValue);
        functionParams=functionParams->right;
        if(functionParams->tokenType == TOKEN_COMMA){
            functionParams=functionParams->right;
        }
    }

    printf("CALL  $%s\n",functionName);
    printf("POPS  LF@%s\n",node->strValue);


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
                generateFunctionEnd(node);
            } else if (strcmp(node->strValue, "while") == 0) {
                generateWhileStatement(node);
            }else if (strcmp(node->strValue, "ifj") == 0) {
                generateBuildInFuncions(node);
            } else  if (strcmp(node->strValue, "if") == 0){
                generateIfStatement(node);
            }else{
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
                    return;
                }
                generateBody(node->right);
            }
            break;

        case NODE_FUNC_DEF:
            generateFunctionParams(node);
            break;

        default:
            printf( "Unhandled node type: %s\n", NodeTypeToString(node->type));
            break;
    }

    // Rekurzívne spracovanie podstromov

    if (node->left  && !strcmp(node->strValue, "while")==0 && !strcmp(node->strValue, "return")==0 && !strcmp(node->strValue, "if")==0) {

        generateBody(node->left);
    }

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
        return;
    }
    processTokenType(node->left);


}
//TODO: spravit scope premennych cez symtable :D
