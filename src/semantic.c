/**
 * @file semantic.c
 * @author Jakub Filo
 * @category Semantic analysis
 * @brief This file contains functions for semantic analysis, processing binary tree, checking everything.
 */

#include "semantic.h"

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
    if (strcmp(typeStr, "void") == 0)
        return TYPE_VOID;
    // Add more types as needed
    return TYPE_UNKNOWN;
}

// Convert DataType to string for error messages
const char *value_type_to_string(DataType type)
{
    switch (type)
    {
    case TYPE_INT:
        return "i32";
    case TYPE_FLOAT:
        return "f64";
    case TYPE_STRING:
        return "string";
    case TYPE_BOOL:
        return "bool";
    case TYPE_VOID:
        return "void";
    case TYPE_U8_ARRAY:
        return "[]u8";
    case TYPE_EMPTY:
        return "empty";
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

BinaryTreeNode *move_left_until(BinaryTreeNode *node, Token_type dest)
{
    while (node != NULL)
    {
        if (node->tokenType == dest)
            return node;
        node = node->left;
    }
    return NULL;
}

BinaryTreeNode *move_right_until(BinaryTreeNode *node, Token_type dest)
{
    while (node != NULL)
    {
        if (node->tokenType == dest)
            return node;
        node = node->right;
    }
    return NULL;
}

void process_var_declaration(BinaryTreeNode *node)
{
    // * IDENTIFIER LOGIC
    BinaryTreeNode *varnode = move_right_until(node, TOKEN_IDENTIFIER);
    char *varIdenti = varnode->strValue;
    // ? DataType varType = TYPE_UNDEFINED; zo symtable urcit?

    DataType varType;
    Token_type initType = TOKEN_EMPTY;
    char *initValue = NULL;

    BinaryTreeNode *varSpeci = move_right_until(varnode, TOKEN_KEYWORD);

    // * DATATYPE LOGIC
    if (varSpeci == NULL) // * skrateny vyraz typu const a = 5; napr
    {
        varType = TYPE_UNKNOWN;
    }
    else
    {
        varType = value_string_to_type(varSpeci->strValue);
        if (varType == TYPE_UNKNOWN)
        {
            printf("Error: Unknown type '%s' for variable '%s'.\n", varSpeci->strValue, varIdenti);
            return;
        }
    }

    // * ASSIGN "=" LOGIC
    BinaryTreeNode *varInit = move_right_until(varnode, TOKEN_ASSIGNMENT);
    BinaryTreeNode *varValue = varInit->left;
    if (varValue->type == NODE_VAR)
    {
        initType = varValue->tokenType;
        initValue = varValue->strValue;
        if (varType == TYPE_UNKNOWN)
        {
            // TODO function append type to variable
            
        }
        

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
    else if (varValue->type == NODE_OP)
    {
        // DataType varExpressType = process_expression(varValue);
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

    BinaryTreeNode *funcName_node = move_right_until(funcDefNode->right, TOKEN_IDENTIFIER);
    char *funcName_str = funcName_node->strValue;
    printf("Function name: %s\n", funcName_str);

    // procces parameters
    BinaryTreeNode *funcParams_start = move_left_until(funcName_node, TOKEN_LPAREN);
    parse_parameters(funcParams_start);

    // Return data type
    BinaryTreeNode *funcReturn_type = move_right_until(funcName_node, TOKEN_KEYWORD);
    DataType returnDef_type = value_string_to_type(funcReturn_type->strValue);

    // NODE_GENERAL > TOKEN_EMPTY
    // process return
    BinaryTreeNode *funcBody = move_left_until(funcReturn_type->right, TOKEN_EMPTY);
    BinaryTreeNode *funcReturnExp_type = ProcessTree(funcBody);

    DataType returnExp_type = process_func_return(funcReturnExp_type);
    printf("return Defined Type %s\n", value_type_to_string(returnDef_type));
    printf("return Real Type %s\n", value_type_to_string(returnExp_type));

    if (are_types_compatible(returnDef_type, returnExp_type))
    {
        printf("Return def type and return expression type MATCH\n");
    }
    else
    {
        printf("Return def type and return expression type do NOT MATCH\n");
    }
    printf("Exiting function '%s' body.\n", funcName_str);
    return;
}

void parse_parameters(BinaryTreeNode *paramsListNode)
{
    if (!paramsListNode)
        return;

    BinaryTreeNode *params_node = paramsListNode->right;
    if (params_node->right == NULL || params_node->right->tokenType == TOKEN_RPAREN)
    {
        // No parameters return
        return;
    }

    while (params_node != NULL && params_node->tokenType != TOKEN_RPAREN)
    {
        if (params_node->tokenType == TOKEN_IDENTIFIER)
        {
            // * Identifier after (
            BinaryTreeNode *param_ident = params_node;
            char *param_ident_str = param_ident->strValue;

            BinaryTreeNode *param_type = move_right_until(param_ident, TOKEN_KEYWORD);
            char *param_type_str = param_type->strValue;
            DataType param_datatype = value_string_to_type(param_type_str);

            // Declare the parameter as a variable in the current scope
            printf("Declared parameter '%s' of type '%s'.\n", param_ident_str, value_type_to_string(param_datatype));

            // Here you can insert the parameter into the symbol table if implemented
            // insert_symbol(current_scope, paramName, paramType);

            // Move to the next token after the current parameter
            if (param_type->right && param_type->right->tokenType == TOKEN_COMMA)
            {
                params_node = move_right_until(param_type->right, TOKEN_IDENTIFIER);
            }
            else
            {
                params_node = param_type->right; // Move to the next node
            }
        }
    }
    return;
}

DataType process_func_return(BinaryTreeNode *returnNode)
{
    if (returnNode->left == NULL && returnNode->right == NULL)
    {
        return TYPE_VOID;
    }

    return process_expression(returnNode);
}

DataType process_expression(BinaryTreeNode *returnNode)
{
    if (returnNode == NULL)
    {
        return TYPE_EMPTY;
    }

    return TYPE_BOOL;
}

BinaryTreeNode *ProcessTree(BinaryTreeNode *root)
{

    if (root == NULL)
        return root;

    BinaryTreeNode *node = root;

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
                else if (strcmp(node->strValue, "return") == 0)
                {
                    // ? return z funkcie
                    return node;
                    // process_func_return(node);
                }
                break;
            default:
                break;
            }

            node = node->parent;
            node = node->left;
        }

        if (node->left == NULL && node->right == NULL)
        {
            break;
        }
    }

    return root; // ?
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