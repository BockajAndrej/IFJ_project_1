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

void process_var_declaration(BinaryTreeNode *node, SymbolStack *stack)
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
    BinaryTreeNode *varValue;
    if (varInit->left != NULL)
    {
        varValue = varInit->left;
    }
    else if (varInit->right != NULL)
    {
        varValue = varInit->right;
    }

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
        {
            long val = strtol(initValue, NULL, 10);
            insert_symbol_stack(stack, varIdenti, varType, &val);
            printf("VAR Declared/Scope Insert '%s' type '%s' value %ld.\n", varIdenti, value_type_to_string(varType), val);
        }
        break;
        case TYPE_FLOAT:
        {
            int val = strtol(initValue, NULL, 10);
            insert_symbol_stack(stack, varIdenti, varType, &val);
            printf("VAR Declared/Scope Insert '%s' type '%s' value %d.\n", varIdenti, value_type_to_string(varType), val);
        }
        break;
        case TYPE_STRING:
        {
            insert_symbol_stack(stack, varIdenti, varType, initValue);
            printf("VAR Declared/Scope Insert '%s' type '%s' value %s.\n", varIdenti, value_type_to_string(varType), initValue);
        }
        break;
        // Handle other types as needed
        default:
            printf("VAR Declared variable '%s' of type '%s'.\n", varIdenti, value_type_to_string(varType));
            break;
        }
    }
    else if (varValue->type == NODE_OP)
    {
        // DataType varExpressType = process_expression(varValue);
    }
    else if (varValue->type == NODE_FUNC_CALL)
    {
        BinaryTreeNode *funcReturn = process_assignFunc(varValue);
        printf("VAR Returned from func with value: %s, type:%s\n", funcReturn->strValue, token_type_to_string(funcReturn->tokenType));
    }
}

BinaryTreeNode *process_assignFunc(BinaryTreeNode *funcnode)
{
    BinaryTreeNode *funcName = funcnode;
    char *funcName_str = funcName->strValue;
    printf("FUNC function name for assign is %s\n", funcName_str);
    BinaryTreeNode *funcParamStart = move_left_until(funcName, TOKEN_LPAREN);
    BinaryTreeNode *param_x = funcParamStart->right;

    // TODO return from the called function calculated value and type
    // * now as placeholder
    BinaryTreeNode *ToReturn = funcnode;

    if (param_x->tokenType == TOKEN_RPAREN) // NO PARAMETERS
    {
        // TODO check return type from hashtable
        printf("FUNC function with no parameters.\n");
    }
    else
    {
        // TODO handle params, check type
        while (param_x->tokenType != TOKEN_RPAREN)
        {
            if (param_x->tokenType == TOKEN_COMMA)
            {
                param_x = param_x->right;
                continue;
            }

            printf("FUNC Handling param \"%s\"\n", param_x->strValue);
            param_x = param_x->right;
        }
    }
    return ToReturn;
}

void process_if(BinaryTreeNode *ConditionNode, SymbolStack *stack)
{
    if (!ConditionNode)
        return;
    printf("IF entering\n");
    BinaryTreeNode *auxnode = move_right_until(ConditionNode, TOKEN_EMPTY);

    // new scope for IF
    push_scope(stack);
    printf("IF New stack created \n");

    // go left from aux    "(" is conditionAndNonNull
    {
        BinaryTreeNode *conditionAndNonNull = auxnode->left;

        // conditionAndNonNull right is |nonnull|
        char *nonNullVal_str = NULL;
        if (conditionAndNonNull->right != NULL)
        {
            printf("IF Processing |nonNull|\n");
            BinaryTreeNode *nonNullVal = move_right_until(conditionAndNonNull, TOKEN_IDENTIFIER);
            nonNullVal_str = nonNullVal->strValue;
            insert_symbol_stack(stack, nonNullVal_str, TYPE_NONNULL, "");
            printf("IF Non null identifier is \"%s\" pushed to scope \n", nonNullVal_str);
        }

        // conditionAndNonNull left is condition
        //  TODO process conditon
    }

    // * go right from aux
    BinaryTreeNode *conditionbody = move_right_until(auxnode, TOKEN_CURLYL_BRACKET);
    conditionbody = move_left_until(conditionbody, TOKEN_EMPTY);
    printf("IF entering body\n");
    ProcessTree(conditionbody, stack);
    printf("IF leaving body\n");

    if (ConditionNode->left != NULL)
    {
        BinaryTreeNode *conditionelse = ConditionNode->left;
        BinaryTreeNode *elsebody = move_right_until(conditionelse, TOKEN_CURLYL_BRACKET)->left;
        printf("IF-else entering body, new stack\n");
        push_scope(stack);
        printf("IF-else New stack created\n");
        ProcessTree(elsebody, stack);
        printf("IF-else leaving body, stack pop\n");
        pop_scope(stack);
    }
}

void process_while(BinaryTreeNode *whileNode, SymbolStack *stack)
{
    if (!whileNode)
        return;
    printf("WHILE entering\n");
    BinaryTreeNode *conditionAndNonNullNode = move_left_until(whileNode, TOKEN_LPAREN);

    // left condition
    printf("WHILE processing condition NOT IMPLEMENTED YET\n");
    // BinaryTreeNode *conditionNode = conditionAndNonNullNode->left;
    //  TODO process expression

    // right nonnull value
    printf("WHILE processing NonNull\n");
    BinaryTreeNode *nonNullNode = move_right_until(conditionAndNonNullNode, TOKEN_IDENTIFIER);
    char *nonNullNode_str = nonNullNode->strValue;
    printf("WHILE NonNull variable is %s \n", nonNullNode_str);

    BinaryTreeNode *body = move_right_until(whileNode, TOKEN_CURLYL_BRACKET);
    body = body->left;
    printf("WHILE processing body\n");
    ProcessTree(body, stack);
    printf("WHILE exiting body\n");
}

void process_identifier_assign(BinaryTreeNode *node)
{
    if (node == NULL)
        return;

    BinaryTreeNode *nodeidentifier = node;
    char *nodeidentifier_str = nodeidentifier->strValue;
    printf("FUNC processing assign to identifier %s\n", nodeidentifier_str);

    BinaryTreeNode *funcname = move_right_until(nodeidentifier->right, TOKEN_IDENTIFIER);
    BinaryTreeNode *funcReturn = process_assignFunc(funcname);
    printf("FUNC Returned from %s with value: %s, type:%s\n", funcname->strValue, funcReturn->strValue, token_type_to_string(funcReturn->tokenType));
}

void process_const_declaration(BinaryTreeNode *node)
{
    if (node)
    {
        /* code */
    }

    return;
}

void process_func_def(BinaryTreeNode *funcDefNode, SymbolStack *stack)
{
    if (!funcDefNode)
        return;

    BinaryTreeNode *funcName_node = move_right_until(funcDefNode->right, TOKEN_IDENTIFIER);
    char *funcName_str = funcName_node->strValue;
    printf("FUNCTION name: %s\n", funcName_str);

    // procces parameters
    BinaryTreeNode *funcParams_start = move_left_until(funcName_node, TOKEN_LPAREN);
    parse_parameters(funcParams_start);

    // Return data type
    BinaryTreeNode *funcReturn_type = move_right_until(funcName_node, TOKEN_KEYWORD);
    DataType returnDef_type = value_string_to_type(funcReturn_type->strValue);

    // NODE_GENERAL > TOKEN_EMPTY
    // process return
    BinaryTreeNode *funcBody = move_left_until(funcReturn_type->right, TOKEN_EMPTY);
    printf("FUNCTION Entering body from %s-%s\n", funcBody->strValue, token_type_to_string(funcBody->tokenType));
    BinaryTreeNode *funcReturnExp_type = ProcessTree(funcBody, stack);
    printf("FUNCTION Leaving body\n");

    DataType returnExp_type = process_func_return(funcReturnExp_type);
    printf("FUNCTION return Defined Type %s\n", value_type_to_string(returnDef_type));
    printf("FUNCTION return Real Type %s\n", value_type_to_string(returnExp_type));

    if (are_types_compatible(returnDef_type, returnExp_type))
    {
        printf("FUNCTION Return def type and return expression type MATCH\n");
    }
    else
    {
        printf("FUNCTION Return def type and return expression type do NOT MATCH\n");
    }
    printf("FUNCTION Exiting function '%s' body.\n", funcName_str);
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

BinaryTreeNode *ProcessTree(BinaryTreeNode *root, SymbolStack *stack)
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
                    process_var_declaration(node, stack);
                }
                else if (strcmp(node->strValue, "const") == 0)
                {
                    process_var_declaration(node, stack);
                }
                else if (strcmp(node->strValue, "pub") == 0)
                {
                    process_func_def(node, stack);
                }
                else if (strcmp(node->strValue, "return") == 0)
                {
                    // ? return z funkcie
                    return node;
                    // process_func_return(node);
                }
                else if (strcmp(node->strValue, "if") == 0)
                {
                    process_if(node, stack);
                }
                else if (strcmp(node->strValue, "while") == 0)
                {
                    process_while(node, stack);
                }

                break;
            case TOKEN_IDENTIFIER:
                if (node->right->tokenType == TOKEN_ASSIGNMENT)
                {
                    process_identifier_assign(node);
                }
                else if (node->right->tokenType == TOKEN_LPAREN)
                {
                    process_assignFunc(node);
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