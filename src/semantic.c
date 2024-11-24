/**
 * @file semantic.c
 * @author Jakub Filo
 * @category Semantic analysis
 * @brief This file contains functions for semantic analysis, processing binary tree, checking everything.
 */

#include "semantic.h"

void ast_valdef()
{
    // i32 a = 3;
    BinaryTreeNode *root = createBinaryNode(NODE_VAR_DECL, TOKEN_KEYWORD, "i32");
    setStartNode(root); // Nastavenie koreňa ako počiatočného uzla
    insertLeft(root, NODE_VAR, TOKEN_IDENTIFIER, "a");
    insertRightMoveRight(root, NODE_ASSIGN, TOKEN_ASSIGNMENT, "=");
    insertLeft(currentNode, NODE_CONST, TOKEN_INT_LITERAL, "3");

    {
        printf("\nComplete Tree Structure:\n");
        printBinaryTree(root);
        freeBinaryTree(root);
        printf("\nTree successfully freed.\n");
    }
}

void ast_val_expression()
{
    // i32 a = 3 + C;
    BinaryTreeNode *root = createBinaryNode(NODE_VAR_DECL, TOKEN_KEYWORD, "i32");
    setStartNode(root);
    insertLeft(root, NODE_VAR, TOKEN_IDENTIFIER, "a");
    insertRightMoveRight(root, NODE_ASSIGN, TOKEN_ASSIGNMENT, "=");
    insertRightMoveRight(currentNode, NODE_OP, TOKEN_ADDITION, "+");
    insertLeft(currentNode, NODE_CONST, TOKEN_INT_LITERAL, "3");
    insertRight(currentNode, NODE_VAR, TOKEN_IDENTIFIER, "c");

    {
        printf("\nComplete Tree Structure:\n");
        printBinaryTree(root);
        freeBinaryTree(root);
        printf("\nTree successfully freed.\n");
    }
}

void ast_while_1()
{
    /*
        while (c != NULL) |INP|
        {
            c = 3;
        }
    */
    BinaryTreeNode *root = createBinaryNode(NODE_WHILE, TOKEN_KEYWORD, "while");
    setStartNode(root);
    insertRight(root, NODE_PROG, TOKEN_EMPTY, "");
    insertLeftMoveLeft(root, NODE_WHILE_PREP, TOKEN_EMPTY, "");
    insertRight(currentNode, NODE_NONNULL, TOKEN_IDENTIFIER, "INP");
    insertLeftMoveLeft(currentNode, NODE_OP, TOKEN_NOT_EQUAL, "!=");
    insertLeft(currentNode, NODE_VAR, TOKEN_IDENTIFIER, "c");
    insertRight(currentNode, NODE_CONST, TOKEN_EMPTY, "NULL"); //! NULL ako token?
    moveUp(2);
    moveDownRight();
    insertRightMoveRight(currentNode, NODE_ASSIGN, TOKEN_ASSIGNMENT, "=");
    insertLeft(currentNode, NODE_VAR, TOKEN_IDENTIFIER, "c");
    insertRight(currentNode, NODE_CONST, TOKEN_INT_LITERAL, "3");

    {
        printf("\nComplete Tree Structure:\n");
        printBinaryTree(root);
        freeBinaryTree(root);
        printf("\nTree successfully freed.\n");
    }
}

void ast_IfElse_1()
{

    // if (a < 0) |val|
    //     i32 a = 5;
    // else if (a == 0)
    //     f64 b = 9;
    // else
    //     []u8 c = "nieco";
    BinaryTreeNode *root = createBinaryNode(NODE_IF, TOKEN_KEYWORD, "if");
    setStartNode(root);
    insertRightMoveRight(currentNode, NODE_PREP_IF, TOKEN_EMPTY, "");
    insertLeftMoveLeft(currentNode, NODE_PREP2_IF, TOKEN_EMPTY, "");
    insertRight(currentNode, NODE_NONNULL, TOKEN_IDENTIFIER, "val");
    insertLeftMoveLeft(currentNode, NODE_OP, TOKEN_LESS_THAN, "<");
    insertLeft(currentNode, NODE_VAR, TOKEN_IDENTIFIER, "a");
    insertRight(currentNode, NODE_CONST, TOKEN_INT_LITERAL, "0");
    moveUp(2);
    insertRightMoveRight(currentNode, NODE_PROG, TOKEN_EMPTY, "");
    insertRightMoveRight(currentNode, NODE_VAR_DECL, TOKEN_KEYWORD, "i32");
    insertLeft(currentNode, NODE_VAR, TOKEN_IDENTIFIER, "a");
    insertRightMoveRight(currentNode, NODE_ASSIGN, TOKEN_ASSIGNMENT, "");
    insertRight(currentNode, NODE_CONST, TOKEN_INT_LITERAL, "3");
    moveUp(4);
    insertLeftMoveLeft(currentNode, NODE_ELSIF, TOKEN_EMPTY, "");
    insertRightMoveRight(currentNode, NODE_ELSIF_PREP, TOKEN_EMPTY, "");
    insertLeftMoveLeft(currentNode, NODE_OP, TOKEN_EQUAL, "==");
    insertLeft(currentNode, NODE_VAR, TOKEN_IDENTIFIER, "a");
    insertRight(currentNode, NODE_CONST, TOKEN_INT_LITERAL, "0");
    moveUp(1);
    insertRightMoveRight(currentNode, NODE_PROG, TOKEN_EMPTY, "");
    insertRightMoveRight(currentNode, NODE_VAR_DECL, TOKEN_KEYWORD, "f64");
    insertLeft(currentNode, NODE_VAR, TOKEN_IDENTIFIER, "b");
    insertRightMoveRight(currentNode, NODE_ASSIGN, TOKEN_ASSIGNMENT, "=");
    insertRight(currentNode, NODE_CONST, TOKEN_FLOAT_LITERAL, "9.6");
    moveUp(4);
    insertLeftMoveLeft(currentNode, NODE_ELSE, TOKEN_EMPTY, "");
    insertRightMoveRight(currentNode, NODE_PROG, TOKEN_EMPTY, "");
    insertRightMoveRight(currentNode, NODE_VAR_DECL, TOKEN_KEYWORD, "[]u8");
    insertLeft(currentNode, NODE_VAR, TOKEN_IDENTIFIER, "c");
    insertRightMoveRight(currentNode, NODE_ASSIGN, TOKEN_ASSIGNMENT, "=");
    insertRight(currentNode, NODE_CONST, TOKEN_STRING_LITERAL, "nieco");

    {
        printf("\nComplete Tree Structure:\n");
        printBinaryTree(root);
        freeBinaryTree(root);
        printf("\nTree successfully freed.\n");
    }
}

DataType value_string_to_type(const char *typeStr)
{
    if (strcmp(typeStr, "i32") == 0)
        return TYPE_INT;
    if (strcmp(typeStr, "f64") == 0)
        return TYPE_FLOAT;
    if (strcmp(typeStr, "bool") == 0)
        return TYPE_BOOL;
    if (strcmp(typeStr, "string") == 0)
        return TYPE_STRING;
    if (strcmp(typeStr, "[]u8") == 0)
        return TYPE_U8_ARRAY;
    // Add more types as needed
    return TOKEN_UNKNOWN;
}

// Convert DataType to string for error messages
const char *value_type_to_string(DataType type)
{
    switch (type)
    {
    case TYPE_INT:
        return "int";
    case TYPE_FLOAT:
        return "float";
    case TYPE_STRING:
        return "string";
    case TYPE_BOOL:
        return "bool";
    case TYPE_VOID:
        return "void";
    case TYPE_U8_ARRAY:
        return "[]u8";
    default:
        return "unknown";
    }
}

bool is_type_compatible(DataType declaredType, Token_type valueType)
{
    switch (declaredType)
    {
    case TYPE_INT:
        return valueType == TOKEN_INT_LITERAL;
    case TYPE_FLOAT:
        return valueType == TOKEN_FLOAT_LITERAL || valueType == TOKEN_INT_LITERAL; // Allow implicit int to float
    case TYPE_STRING:
        return valueType == TOKEN_STRING_LITERAL;
    // Add more type compatibility rules as needed
    default:
        return false;
    }
}

bool are_types_compatible(DataType actual, DataType expected)
{
    if (actual == expected)
        return true;
    // Example: Allow implicit conversion from int to float
    if (actual == TYPE_INT && expected == TYPE_FLOAT)
        return true;
    if (actual == TYPE_FLOAT && expected == TYPE_INT)
        return false;
    return false;
}

void process_var_declaration(BinaryTreeNode *node)
{
    // * IDENTIFIER LOGIC
    BinaryTreeNode *varnode = node->right;
    if (!varnode || varnode->tokenType != TOKEN_IDENTIFIER)
    {
        printf("Error: Expected variable name after 'var'.\n");
        return;
    }

    char *varIdenti = varnode->strValue;
    // ? DataType varType = TYPE_UNDEFINED; zo symtable urcit?

    DataType varType;
    Token_type initType = TOKEN_EMPTY;
    char *initValue = NULL;

    BinaryTreeNode *varColon = varnode->right;
    // * COLON ":" LOGIC
    if (varColon && varColon->tokenType == TOKEN_COLON)
    {
        BinaryTreeNode *varSpeci = varColon->right;
        // * DATATYPE LOGIC
        if (varSpeci && varSpeci->tokenType == TOKEN_KEYWORD)
        {
            varType = value_string_to_type(varSpeci->strValue);
            if (varType == TOKEN_UNKNOWN)
            {
                printf("Error: Unknown type '%s' for variable '%s'.\n", varSpeci->strValue, varIdenti);
                return;
            }
        }
        else
        {
            printf("Error: Invalid type specifier for variable '%s'.\n", varIdenti);
            return;
        }

        // * ASSIGN "=" LOGIC
        BinaryTreeNode *varInit = varSpeci->right;
        if (varInit && varInit->tokenType == TOKEN_ASSIGNMENT)
        {
            BinaryTreeNode *varValue = varInit->left;
            if (varValue != NULL)
            {
                initType = varValue->tokenType;
                initValue = varValue->strValue;

                if (!is_type_compatible(varType, initType))
                {
                    printf("Type Error: Cannot assign '%s' to variable '%s' of type '%s'.\n", token_type_to_string(initType), varIdenti, value_type_to_string(varType));
                    return;
                }
                // * TESTING OUTPUT
                // Extract the value based on type
                switch (varType)
                {
                case TYPE_INT:
                    printf("Declared variable '%s' of type '%s' with value %d.\n", varIdenti, value_type_to_string(varType), atoi(initValue));
                    break;
                case TYPE_FLOAT:
                    printf("Declared variable '%s' of type '%s' with value %f.\n", varIdenti, value_type_to_string(varType), atof(initValue));
                    break;
                case TYPE_STRING:
                    printf("Declared variable '%s' of type '%s' with value '%s'.\n", varIdenti, value_type_to_string(varType), initValue);
                    break;
                // Handle other types as needed
                default:
                    printf("Declared variable '%s' of type '%s'.\n", varIdenti, value_type_to_string(varType));
                    break;
                }
            }
        }
        else
        {
            // Declaration without initialization
            printf("Declared variable '%s' of type '%s' without initialization.\n", varIdenti, value_type_to_string(varType));
        }
    }
    else
    {
        // Declaration without type annotation
        printf("Warning: Variable '%s' declared without type annotation.\n", varIdenti);
        return;
    }
}

void process_const_declaration(BinaryTreeNode *node)
{
    if (node)
    {
        /* code */
    }

    return;
}

void process_func_def(BinaryTreeNode *funcDefNode)
{
    if (!funcDefNode)
        return;

    BinaryTreeNode *fnKeywordNode = funcDefNode->right;
    if (!fnKeywordNode || strcmp(fnKeywordNode->strValue, "fn") != 0)
    {
        printf("Error: Expected 'fn' keyword after 'pub'.\n");
        return;
    }

    // Function name
    BinaryTreeNode *funcNameNode = fnKeywordNode->right;
    if (!funcNameNode || funcNameNode->tokenType != TOKEN_IDENTIFIER)
    {
        printf("Error: Expected function name identifier.\n");
        return;
    }
    //char *funcName = funcNameNode->strValue;
}

// ! mal by som traversovat strom cez moveright alebo moveleft?
void parse_parameters(BinaryTreeNode *paramsListNode)
{
    if (!paramsListNode)
        return;

    BinaryTreeNode *current = paramsListNode;

    while (current != NULL)
    {
        if (current->tokenType == TOKEN_IDENTIFIER)
        {
            char *paramName = current->strValue;
            BinaryTreeNode *colonNode = current->right;
            if (!colonNode || colonNode->tokenType != TOKEN_COLON)
            {
                printf("Error: Expected ':' after parameter name '%s'.\n", paramName);
                return;
            }

            BinaryTreeNode *typeNode = colonNode->left;
            if (!typeNode || typeNode->tokenType != TOKEN_KEYWORD)
            {
                printf("Error: Expected type specifier after ':' for parameter '%s'.\n", paramName);
                return;
            }

            char *typeStr = typeNode->strValue;
            DataType paramType = value_string_to_type(typeStr);
            if (paramType == TYPE_EMPTY)
            {
                printf("Error: Unknown type '%s' for parameter '%s'.\n", typeStr, paramName);
                return;
            }

            // Declare the parameter as a variable in the current scope
            printf("Declared parameter '%s' of type '%s'.\n", paramName, value_type_to_string(paramType));

            // Here you can insert the parameter into the symbol table if implemented
            // insert_symbol(current_scope, paramName, paramType);

            // Move to the next parameter (assuming separated by commas)
            current = colonNode->right; // Move past the colon and type
            if (current != NULL && current->tokenType == TOKEN_COMMA)
            {
                current = current->right; // Move to the next parameter identifier
            }
        }
        else
        {
            printf("Error: Unexpected token in parameters list.\n");
            return;
        }
    }
}

void ProcessTree(BinaryTreeNode *root)
{
    if (root == NULL)
        return;

    BinaryTreeNode *node = root;

    // check na token empty alebo node_general ? spytat sa bockyho
    while (node != NULL)
    {
        if (node->type == NODE_GENERAL)
        {
            node = node->right;
            switch (node->tokenType)
            {
            case TOKEN_KEYWORD:
                if (strcmp(node->strValue, "var") == 0)
                {
                    process_var_declaration(node);
                }
                else if (strcmp(node->strValue, "const") == 0)
                {
                    process_var_declaration(node);
                }
                else if (strcmp(node->strValue, "pub") == 0)
                {
                    process_func_def(node);
                }
                break;
            default:
                break;
            }
            node = node->parent;
            node = node->left;
        }
        else if (node->type == NODE_GENERAL && node->left != NULL)
        {
            return;
        }
        else
        {
            return;
        }
    }
}

/*
? 3 - sémantická chyba v programu – nedefinovaná funkce či proměnná.
? 4 - sémantická chyba v programu – špatný počet/typ parametrů u volání funkce;
špatný typ či nepovolené zahození návratové hodnoty z funkce.
? 5 - sémantická chyba v programu – redefinice proměnné nebo funkce; přiřazení do
nemodifikovatelné proměnné.
? 6 - sémantická chyba v programu – chybějící/přebývající výraz v příkazu návratu
z funkce.
? 7 - sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních
výrazech; nekompatibilní typ výrazu (např. při přiřazení).
? 8 - sémantická chyba odvození typu – typ proměnné není uveden a nelze odvodit od
použitého výrazu.
? 9 - sémantická chyba nevyužité proměnné v jejím rozsahu platnosti; modifikovatelná
proměnná bez možnosti změny po její inicializaci.
? 10 - ostatní sémantické chyby.
*/