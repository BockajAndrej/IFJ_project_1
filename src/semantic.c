/**
 * @file semantic.c
 * @author Jakub Filo
 * @category Semantic analysis
 * @brief This file contains functions for semantic analysis, processing binary tree, checking everything.
 */

#include "semantic.h"

const char *token_to_type(Token_type value)
{
    switch (value)
    {
    case TOKEN_INT_LITERAL:
        return "i32";
    case TOKEN_FLOAT_LITERAL:
        return "f64";
    case TOKEN_STRING_LITERAL:
        return "[]u8";
    default:
        break;
    }
    return "";
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
    bool isGlobal = false;
    bool isConst = false;
    if (strcmp(node->strValue, "const") == 0)
    {
        isConst = true;
    }

    // * IDENTIFIER LOGIC
    BinaryTreeNode *varnode = move_right_until(node, TOKEN_IDENTIFIER);
    char *varIdenti = varnode->strValue;

    DataType varType;
    Token_type initType = TOKEN_EMPTY;
    char *initValue = NULL;

    BinaryTreeNode *varSpeci = move_right_until(varnode, TOKEN_KEYWORD);

    // * DATATYPE LOGIC
    bool isNull = false;
    if (varSpeci == NULL) // * skrateny vyraz typu const a = 5; napr
    {
        varType = TYPE_UNKNOWN;
    }
    else
    {
        varType = value_string_to_type(varSpeci->strValue);
    }

    /*
        TODO Speciální hodnotu null mohou nabývat všechny
        TODO proměnné, parametry a návratové hodnoty typované s předponou ’?’, tzv. typ zahrnující null.
        TODO  Z hodnoty null nelze odvodit konkrétní typ.
    */

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
            // TODO check the implementation to guees the data type?
            varType = process_expression(varValue, stack);
        }

        if (!are_types_compatible(value_string_to_type(token_to_type(initType)), varType))
        {
            // printf("Type Error: Cannot assign '%s' to variable '%s' of type '%s'.\n", token_type_to_string(initType), varIdenti, value_type_to_string(varType));
            // printf("type is %s-%s\n", token_to_type(initType), value_type_to_string(varType));
            handle_error(ERR_TYPE_COMPAT);
            return;
        }
        if (stack->top->next == NULL)
        {
            printf("SOM GLOVAL;\n");
            isGlobal = true;
        }

        // * TESTING OUTPUT
        // Extract the value based on type
        switch (varType)
        {
        case TYPE_INT:
        case TYPE_INT_NULL:
        {
            long val = strtol(initValue, NULL, 10);
            insert_symbol_stack(stack, varIdenti, varType, &val, isConst, isNull, isGlobal);
            printf("VAR Insert '%s' type '%s' value %ld const %s global %s.\n", varIdenti,
             value_type_to_string(varType), val, isConst ? "true" : "false",
             isGlobal ? "true" : "false");
        }
        break;
        case TYPE_FLOAT:
        case TYPE_FLOAT_NULL:
        {
            float val = strtof(initValue, NULL);
            insert_symbol_stack(stack, varIdenti, varType, &val, isConst, isNull, isGlobal);
            printf("VAR Insert '%s' type '%s' value %f const %s.\n", varIdenti, value_type_to_string(varType), val, isConst ? "true" : "false");
        }
        break;
        case TYPE_STRING:
        {
            insert_symbol_stack(stack, varIdenti, varType, initValue, isConst, isNull, isGlobal);
            printf("VAR '%s' type '%s' value %s const %s.\n", varIdenti, value_type_to_string(varType), initValue, isConst ? "true" : "false");
        }
        break;
        // Handle other types as needed
        default:
            printf("VAR '%s' of type '%s'.\n", varIdenti, value_type_to_string(varType));
            break;
        }
    }
    else if (varValue->type == NODE_OP)
    {
        // volam ked "var result : i32 = 1 + 2 * 5;"
        process_expression(varValue, stack);
    }
    else if (varValue->type == NODE_FUNC_CALL)
    {
        DataType funcReturnType = process_validate_func_call(varValue, stack);
        printf("VAR Returned from func with type:%s\n", value_type_to_string(funcReturnType));
        // TODO check return type againt the var
    }
}

void process_if(BinaryTreeNode *ConditionNode, SymbolStack *stack)
{
    if (!ConditionNode)
        return;
    BinaryTreeNode *auxnode = move_right_until(ConditionNode, TOKEN_EMPTY);

    // new scope for IF
    push_scope(stack);
    printf("IF pushscope >>>>>>> \n");

    // go left from aux    "(" is conditionAndNonNull
    {
        BinaryTreeNode *conditionAndNonNull = auxnode->left;

        // conditionAndNonNull right is |nonnull|
        char *nonNullVal_str = NULL;
        if (conditionAndNonNull->right != NULL)
        {
            BinaryTreeNode *nonNullVal = move_right_until(conditionAndNonNull, TOKEN_IDENTIFIER);
            nonNullVal_str = nonNullVal->strValue;
            insert_symbol_stack(stack, nonNullVal_str, TYPE_NONNULL, "", false, false, false);
            printf("IF NonNull \"%s\"\n", nonNullVal_str);
        }

        // conditionAndNonNull left is condition
        //  TODO process conditon
    }

    // * go right from aux
    BinaryTreeNode *conditionbody = move_right_until(auxnode, TOKEN_CURLYL_BRACKET);
    conditionbody = move_left_until(conditionbody, TOKEN_EMPTY);
    printf("IF entering body\n");
    ProcessTree(conditionbody, stack);
    printf("IF leaving body \n");
    pop_scope(stack);

    if (ConditionNode->left != NULL)
    {
        BinaryTreeNode *conditionelse = ConditionNode->left;
        BinaryTreeNode *elsebody = move_right_until(conditionelse, TOKEN_CURLYL_BRACKET)->left;
        printf("IF-else entering body, new stack\n");
        push_scope(stack);
        ProcessTree(elsebody, stack);
        printf("IF-else leaving body, stack pop\n");
        pop_scope(stack);
    }
    printf("IF <<<<<<<<<<");
}

void process_while(BinaryTreeNode *whileNode, SymbolStack *stack)
{
    if (!whileNode)
        return;
    printf("WHILE >>>>>>>>\n");
    BinaryTreeNode *conditionAndNonNullNode = move_left_until(whileNode, TOKEN_LPAREN);

    // left condition
    printf("WHILE  condition \n");
    // BinaryTreeNode *conditionNode = conditionAndNonNullNode->left;
    //  TODO process expression

    // right nonnull value
    BinaryTreeNode *nonNullNode = move_right_until(conditionAndNonNullNode, TOKEN_IDENTIFIER);
    char *nonNullNode_str = nonNullNode->strValue;
    insert_symbol_stack(stack, nonNullNode_str, TYPE_NONNULL, "", false, false, false);
    printf("WHILE NonNull %s put in stack\n", nonNullNode_str);

    BinaryTreeNode *body = move_right_until(whileNode, TOKEN_CURLYL_BRACKET);
    body = body->left;
    printf("WHILE processing body push stack\n");
    push_scope(stack);
    ProcessTree(body, stack);
    pop_scope(stack);
    printf("WHILE stack pop <<<<<<\n");
}

void process_identifier_assign(BinaryTreeNode *node, SymbolStack *stack)
{
    if (node == NULL)
        return;

    BinaryTreeNode *nodeidentifier = node;
    char *nodeidentifier_str = nodeidentifier->strValue;
    printf("ASSIGN processing identifier %s\n", nodeidentifier_str);
    Symbol *identifier = search_symbol_stack(stack, nodeidentifier_str);
    if (identifier)
    {
        printf("SYMBOL '%s' Type '%s' FOUND.\n", identifier->name, value_type_to_string(identifier->type));
    }
    else
    {
        printf("SYMBOL '%s' NOT FOUND.\n", nodeidentifier_str);
        // ? 3 - sémantická chyba v programu – nedefinovaná funkce či proměnná.
    }

    BinaryTreeNode *expressionOrFunc = move_right_until(nodeidentifier->right, TOKEN_ASSIGNMENT);

    if (expressionOrFunc->right->type == NODE_FUNC_CALL)
    {
        BinaryTreeNode *functoAssign = move_right_until(expressionOrFunc, TOKEN_IDENTIFIER);
        printf("ASSIGN function %s\n", functoAssign->strValue);
        DataType funcTypeReturn = process_validate_func_call(functoAssign, stack);
        if (funcTypeReturn == TYPE_UNKNOWN)
        {
            /* code */
        }
    }
    else if (expressionOrFunc->right->type == NODE_OP)
    {
        // !volam ked napr "result = 5 * 3;"

        BinaryTreeNode *expresstoAssign = expressionOrFunc->left;
        printf("ASSIGN expression %s\n", expresstoAssign->strValue);
        // DataType expression = process_expression(expresstoAssign);
        // TODO datatype do symtable
        exit(0);
    }
    else
    {
        // ! volam ked napr "result = 3;"

        BinaryTreeNode *valuetoAssign = expressionOrFunc->left;
        printf("ASSIGN value %s\n", valuetoAssign->strValue);
        // DataType value = process_expression(valuetoAssign);
        // TODO datatype do symtable
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

void process_func_def(BinaryTreeNode *funcDefNode, SymbolStack *stack)
{
    if (!funcDefNode)
        return;

    BinaryTreeNode *funcName_node = move_right_until(funcDefNode->right, TOKEN_IDENTIFIER);
    printf("FUNCTION name: %s\n", funcName_node->strValue);

    // * Temporarily switch to the global scope for the function definition
    Scope *currentScope = stack->top; // Save the current scope
    while (stack->top->next != NULL)
    {
        stack->top = stack->top->next; // Move to the global scope
    }

    // procces parameters
    BinaryTreeNode *funcParams_start = move_left_until(funcName_node, TOKEN_LPAREN);
    Symbol *paramChain = parse_parameters(funcParams_start);
    // Debugging Output
    Symbol *currentParam = paramChain;
    while (currentParam != NULL)
    {
        printf("Parameter: %s, Type: %s\n", currentParam->name, value_type_to_string(currentParam->type));
        currentParam = currentParam->next;
    }

    // Return data type
    BinaryTreeNode *funcReturn_type = move_right_until(funcName_node, TOKEN_KEYWORD);
    DataType returnDef_type = value_string_to_type(funcReturn_type->strValue);

    // Insert function symbol with its parameters as a chain
    Symbol *funcSymbol = malloc(sizeof(Symbol));
    funcSymbol->name = copy_string(funcName_node->strValue);
    funcSymbol->type = TYPE_FUNCTION;
    funcSymbol->value.params = paramChain; // Chain of parameters
    funcSymbol->next = NULL;
    insert_symbol_stack(stack, funcSymbol->name, funcSymbol->type, funcSymbol, false, false, true);
    printf("FUNCTION '%s' stored in global scope with parameters.\n", funcName_node->strValue);

    // Restore original scope
    stack->top = currentScope;

    // Push new scope for function body
    push_scope(stack);

    // process return
    BinaryTreeNode *funcBody = move_left_until(funcReturn_type->right, TOKEN_EMPTY);
    printf("FUNCTION Entering body \n");
    BinaryTreeNode *funcReturnExp_type = ProcessTree(funcBody, stack);
    printf("FUNCTION Leaving body\n");

    DataType returnExp_type = process_func_return(funcReturnExp_type, stack);

    // TODO void funckia nemusi mat return;
    if (are_types_compatible(returnExp_type, returnDef_type))
    {
        printf("FUNCTION Return type and return expression  type MATCH\n");
    }
    else
    {
        printf("FUNCTION Return type and return expression type do NOT MATCH\n");
    }
    printf("FUNCTION Exiting function '%s' body.\n", funcName_node->strValue);
    pop_scope(stack);
}

Symbol *parse_parameters(BinaryTreeNode *paramsListNode)
{
    if (!paramsListNode)
        return NULL;

    Symbol *paramChain = NULL; // Head of the parameter chain
    Symbol *lastParam = NULL;  // Pointer to the last parameter in the chain

    BinaryTreeNode *params_node = paramsListNode->right;
    if (params_node->right == NULL || params_node->right->tokenType == TOKEN_RPAREN)
    {
        // No parameters return
        printf("NO PARAMETERS\n");
        return NULL;
    }

    while (params_node != NULL && params_node->tokenType != TOKEN_RPAREN)
    {
        if (params_node->tokenType == TOKEN_IDENTIFIER)
        {
            // * Identifier after (
            BinaryTreeNode *param_ident = params_node;
            BinaryTreeNode *param_type = move_right_until(param_ident, TOKEN_KEYWORD);
            DataType param_datatype = value_string_to_type(param_type->strValue);

            // Create a new symbol for the parameter
            Symbol *paramSymbol = malloc(sizeof(Symbol));
            paramSymbol->name = copy_string(param_ident->strValue);
            paramSymbol->type = param_datatype;
            paramSymbol->next = NULL;

            // Add to the parameter chain
            if (lastParam == NULL)
            {
                paramChain = paramSymbol; // Set the head
            }
            else
            {
                lastParam->next = paramSymbol; // Append to the chain
            }
            lastParam = paramSymbol;

            printf("FUNC param '%s' of type '%s' added to parameter chain.\n", param_ident->strValue, value_type_to_string(param_datatype));

            // Move to the next parameter
            if (param_type->right && param_type->right->tokenType == TOKEN_COMMA)
            {
                params_node = move_right_until(param_type->right, TOKEN_IDENTIFIER);
            }
            else
            {
                params_node = param_type->right;
            }
        }
    }

    return paramChain; // Return the head of the parameter chain}
}

DataType process_validate_func_call(BinaryTreeNode *funcnode, SymbolStack *stack)
{
    if (!funcnode)
        return TYPE_UNKNOWN;

    char *funcName_str = funcnode->strValue;

    // Search for the function in the global scope
    Scope *currentScope = stack->top;
    while (stack->top->next != NULL)
    {
        stack->top = stack->top->next; // Move to global scope
    }

    Symbol *funcSymbol = search_symbol_stack(stack, funcName_str);
    stack->top = currentScope; // Restore the original scope

    if (!funcSymbol)
    {
        fprintf(stderr, "Error: Function '%s' is not defined.\n", funcName_str);
        return TYPE_UNKNOWN;
    }

    if (funcSymbol->type != TYPE_FUNCTION)
    {
        fprintf(stderr, "Error: '%s' is not a function.\n", funcName_str);
        return TYPE_UNKNOWN;
    }

    printf("FUNC function name for call is '%s'.\n", funcName_str);

    // Traverse the parameter chain
    Symbol *paramSymbol = funcSymbol->value.params; // Start with the parameter chain
    BinaryTreeNode *funcParams_start = move_left_until(funcnode, TOKEN_LPAREN);
    BinaryTreeNode *argNode = funcParams_start->right;
    int argCount = 0;
    int paramCount = 0;

    // Iterate through arguments until TOKEN_RPAREN
    while (argNode && argNode->tokenType != TOKEN_RPAREN)
    {
        if (argNode->tokenType == TOKEN_COMMA)
        {
            // Skip comma and move to the next argument
            argNode = argNode->right;
            continue;
        }

        if (paramSymbol)
        {
            // Process the argument against the current parameter
            // Get the argument's type
            Symbol *argNode_tofind = search_symbol_stack(stack, argNode->strValue);
            DataType argType;
            if (argNode_tofind != NULL)
            {
                argType = find_return_datatype(argNode_tofind->name);
            }
            else
            {
                argType = find_return_datatype(argNode->strValue);
            }

            // Validate argument type
            if (!are_types_compatible(paramSymbol->type, argType))
            {
                fprintf(stderr, "Error: Type mismatch for parameter '%s' in function '%s'. Expected '%s', got '%s'.\n",
                        paramSymbol->name, funcName_str,
                        value_type_to_string(paramSymbol->type),
                        value_type_to_string(argType));
            }
            else
            {
                printf("Argument type for parameter '%s' in function '%s' matches. Type: '%s', Value: '%s'.\n",
                       paramSymbol->name, funcName_str, value_type_to_string(argType), argNode->strValue);
            }

            // Move to the next parameter
            paramSymbol = paramSymbol->next;
            paramCount++;
        }
        else
        {
            // Extra argument detected
            fprintf(stderr, "Error: Too many arguments for function '%s'. Expected %d, got more than %d.\n",
                    funcName_str, paramCount, argCount + 1);
        }

        // Move to the next argument
        argNode = argNode->right;
        argCount++;
    }

    // After processing all arguments, check for missing parameters
    Symbol *missingParam = paramSymbol;
    while (missingParam)
    {
        fprintf(stderr, "Error: Missing argument for parameter '%s' in function '%s'.\n",
                missingParam->name, funcName_str);
        missingParam = missingParam->next;
        paramCount++;
    }

    printf("Function call validation completed for '%s'.\nExpected parameters: %d, Provided arguments: %d.\n",
           funcName_str, paramCount, argCount);

    return funcSymbol->type;
}

DataType find_return_datatype(char *value)
{
    DataType toReturn;

    int is_float = 0;
    int is_string = 0;

    // Check each character in the string
    for (int i = 0; value[i] != '\0'; i++)
    {
        if (value[i] == '.')
        {
            if (is_float || is_string)
            {
                is_string = 1; // Multiple dots or invalid format mean it's a string
                break;
            }
            is_float = 1; // Mark as a potential float
        }
        else if (!isdigit(value[i]))
        {
            is_string = 1; // Non-numeric characters mean it's a string
            break;
        }
    }

    if (is_string)
        toReturn = TYPE_STRING; // Assume STRING_TYPE is defined for strings
    else if (is_float)
        toReturn = TYPE_FLOAT; // Assume FLOAT_TYPE is defined for floats
    else
        toReturn = TYPE_INT; // If all characters are digits, it's an integer

    return toReturn;
}

// ! volam ked je return z funkcie "return a * 9;"
DataType process_func_return(BinaryTreeNode *returnNode, SymbolStack *stack)
{
    if (returnNode->left == NULL && returnNode->right == NULL)
    {
        return TYPE_VOID;
    }

    // ! DELETE AFTER process_expression IS IMPLEMENTED
    return TYPE_FLOAT_NULL;
    // ! DELETE AFTER process_expression IS IMPLEMENTED

    return process_expression(returnNode, stack);
}

DataType process_expression(BinaryTreeNode *returnNode, SymbolStack *stack)
{
    Stack s;
    initStack(&s, rule);
    setStartNodeInOrder(returnNode);
    InOrder(curInOrderNode, &s);

    PrintAllStack(&s);

    if (returnNode == NULL)
    {
        return TYPE_EMPTY;
    }
    if (stack)
    {
    }

    // ? BinaryTreeNode *node = returnNode;

    // ?  SymbolStack *test;

    // ak nie je vrati null,
    // ?   test = search_symbol_stack(stack, "result");

    // ak mas napr return 5 * c; tak symbol ti hodi null a vies ze v tabulky nie je
    // mozem pouzit find_return_datatype() na zistenie datatype
    // ?  test = search_symbol_stack(stack, "5");

    // na check ci je type vhodny vies pouzit is_type_compatible(), pozri sa ako funguje func

    return TYPE_INT;
}

void process_voidFunc(BinaryTreeNode *node, SymbolStack *stack)
{
    BinaryTreeNode *paramsstart = node->left;
    paramsstart = move_right_until(node, TOKEN_IDENTIFIER);
    DataType funcreturntype = process_validate_func_call(paramsstart, stack);
    if (funcreturntype == TYPE_VOID)
    {
        printf("Return value for function %s is valid\n", node->strValue);
    }
    else
    {
        printf("Return value for function %s is NOT valid, expected TYPE_VOID\n", node->strValue);
    }
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
                    // ? return do funkcie
                    return node;
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

                if (node->right && node->right->tokenType == TOKEN_ASSIGNMENT)
                {
                    process_identifier_assign(node, stack);
                }
                else if (node->left && node->left->tokenType == TOKEN_LPAREN)
                {
                    process_voidFunc(node, stack);
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