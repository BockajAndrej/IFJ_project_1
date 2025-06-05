/**
 * @file semantic.c
 * @author Jakub Filo
 * @category Semantic analysis
 * @brief This file contains functions for semantic analysis, processing binary tree, checking everything.
 */

#include "semantic.h"

void process_var_declaration(BinaryTreeNode *node, SymbolStack *stack)
{
    bool isGlobal = false;
    bool isConst = false;

    // * Check if the variable is constant
    if (strcmp(node->strValue, "const") == 0)
    {
        isConst = true;
    }

    // * IDENTIFIER LOGIC: Get variable identifier
    BinaryTreeNode *varnode = move_right_until(node, TOKEN_IDENTIFIER);
    char *varIdenti = varnode->strValue;

    DataType varType;
    Token_type initType = TOKEN_EMPTY;
    char *initValue = NULL;

    BinaryTreeNode *varSpeci = move_right_until(varnode, TOKEN_KEYWORD);

    // * DATATYPE LOGIC: Determine the variable type
    bool isNull = false;
    if (varSpeci == NULL) // * Handle cases like "const a = 5;" where type is omitted
    {
        varType = TYPE_UNKNOWN;
    }
    else
    {
        varType = value_string_to_type(varSpeci->strValue);
    }

    // * ASSIGN "=" LOGIC: Get the assigned value
    BinaryTreeNode *varInit = move_right_until(varnode, TOKEN_ASSIGNMENT);
    BinaryTreeNode *varValue;

    // * Determine which child contains the value of the assignment
    if (varInit->left != NULL)
    {
        varValue = varInit->left;
    }
    else if (varInit->right != NULL)
    {
        varValue = varInit->right;
    }

    // * If the value is a variable
    if (varValue->type == NODE_VAR)
    {
        initType = varValue->tokenType;
        initValue = varValue->strValue;

        // * If type is unknown, process the expression to deduce the type
        if (varType == TYPE_UNKNOWN)
        {
            varType = process_expression(varValue, stack);
        }

        // * Check type compatibility between the value and the declared type
        if (!are_types_compatible(value_string_to_type(token_to_type(initType)), varType))
        {
            freeTreeFromAnyNode(node);
            free_symbol_stack(stack);
            handle_error(ERR_TYPE_COMPAT);
            return;
        }

        // * Check if the variable is global (top of stack is empty)
        if (stack->top->next == NULL)
        {
            isGlobal = true;
        }

        // * Check for redeclaration of the variable
        if (search_hash_table(stack->top->table, varIdenti))
        {
            freeTreeFromAnyNode(node);
            free_symbol_stack(stack);
            handle_error(ERR_REDEF);
        }

        // * TESTING OUTPUT: Insert the variable into the symbol table
        switch (varType)
        {
        case TYPE_INT:
        case TYPE_INT_NULL:
        {
            int val = atoi(initValue); // Convert to integer using atoi
            insert_symbol_stack(stack, varIdenti, varType, &val, isConst, isNull, isGlobal, TYPE_EMPTY);
        }
        break;
        case TYPE_FLOAT:
        case TYPE_FLOAT_NULL:
        {
            float val = strtof(initValue, NULL); // Convert to float using strtof
            insert_symbol_stack(stack, varIdenti, varType, &val, isConst, isNull, isGlobal, TYPE_EMPTY);
        }
        break;
        case TYPE_STRING:
        {
            insert_symbol_stack(stack, varIdenti, varType, initValue, isConst, isNull, isGlobal, TYPE_EMPTY);
        }
        break;
        // * Handle other types as needed
        default:
            break;
        }
    }
    // * If the value is an operation (e.g., a complex expression)
    else if (varValue->type == NODE_OP)
    {
        // Process the expression for complex initializations (e.g., "a = 1 + 2 * 5")
        process_expression(varValue, stack);
    }
    // * If the value is a function call
    else if (varValue->type == NODE_FUNC_CALL)
    {
        DataType funcReturnType = process_validate_func_call(varValue, stack);
        if (funcReturnType)
        {
            /* code */
        }

        //  TODO: Check the return type against the variable's type
    }
}

void process_if(BinaryTreeNode *ConditionNode, SymbolStack *stack)
{
    if (!ConditionNode)
        return;

    // * Move right to the empty token to get the condition and non-null value
    BinaryTreeNode *auxnode = move_right_until(ConditionNode, TOKEN_EMPTY);

    // * Create a new scope for the IF statement
    push_scope(stack);

    // * Handle condition and non-null logic

    BinaryTreeNode *conditionAndNonNull = auxnode->left;

    // * If there is a non-null value, insert it into the symbol stack
    char *nonNullVal_str = NULL;
    if (conditionAndNonNull->right != NULL)
    {
        BinaryTreeNode *nonNullVal = move_right_until(conditionAndNonNull, TOKEN_IDENTIFIER);
        nonNullVal_str = nonNullVal->strValue;
        insert_symbol_stack(stack, nonNullVal_str, TYPE_NONNULL, "", false, false, false, TYPE_EMPTY);
    }

    // * Process the actual condition expression (TODO: implement condition processing)

    // * Handle the body of the IF statement (code inside curly brackets)
    BinaryTreeNode *conditionbody = move_right_until(auxnode, TOKEN_CURLYL_BRACKET);
    conditionbody = move_left_until(conditionbody, TOKEN_EMPTY); // move to the inner content
    ProcessTree(conditionbody, stack);                           // Process the body of the IF
    pop_scope(stack);                                            // End the IF scope

    // * Handle the ELSE block if it exists
    if (ConditionNode->left != NULL)
    {
        BinaryTreeNode *conditionelse = ConditionNode->left;
        BinaryTreeNode *elsebody = move_right_until(conditionelse, TOKEN_CURLYL_BRACKET)->left;

        // * Create a new scope for the ELSE block
        push_scope(stack);
        ProcessTree(elsebody, stack); // Process the body of the ELSE
        pop_scope(stack);             // End the ELSE scope
    }
}

void process_while(BinaryTreeNode *whileNode, SymbolStack *stack)
{
    if (!whileNode)
        return;

    // * Move left to find the opening parenthesis for the condition and non-null value
    BinaryTreeNode *conditionAndNonNullNode = move_left_until(whileNode, TOKEN_LPAREN);

    // * Process the condition part (left side of the condition)
    BinaryTreeNode *conditionNode = conditionAndNonNullNode->left;
    DataType expression_while = process_expression(conditionNode, stack);
    if (expression_while == TYPE_INT)
    {
        /* code */
    }

    // * If there's a non-null value on the right side of the condition, insert it into the symbol stack
    if (conditionAndNonNullNode->right != NULL)
    {
        BinaryTreeNode *nonNullNode = move_right_until(conditionAndNonNullNode, TOKEN_IDENTIFIER);
        char *nonNullNode_str = nonNullNode->strValue;
        insert_symbol_stack(stack, nonNullNode_str, TYPE_NONNULL, "", false, false, false, TYPE_EMPTY);
    }

    // * Process the body of the while loop (code inside curly brackets)
    BinaryTreeNode *body = move_right_until(whileNode, TOKEN_CURLYL_BRACKET);
    body = body->left; // Move to the actual body of the loop

    // * Create a new scope for the loop body and process the body code
    push_scope(stack);
    ProcessTree(body, stack); // Process the statements inside the while loop
    pop_scope(stack);         // End the scope after processing the body
}

void process_identifier_assign(BinaryTreeNode *node, SymbolStack *stack)
{
    if (node == NULL)
        return;

    // * Retrieve the identifier and check if it's defined in the symbol stack
    BinaryTreeNode *nodeidentifier = node;
    char *nodeidentifier_str = nodeidentifier->strValue;
    Symbol *identifier = search_symbol_stack(stack, nodeidentifier_str);
    if (!identifier)
    {
        // Handle error if identifier is not defined
        freeTreeFromAnyNode(node);
        free_symbol_stack(stack);
        handle_error(ERR_UNDEFINED_ID);
    }

    // * Move to the right to find the assignment operator and the expression to assign
    BinaryTreeNode *expressionOrFunc = move_right_until(nodeidentifier->right, TOKEN_ASSIGNMENT);
    if (expressionOrFunc->left != NULL)
    {
        expressionOrFunc = expressionOrFunc->left;
    }
    else if (expressionOrFunc->right != NULL)
    {
        expressionOrFunc = expressionOrFunc->right;
    }

    // * If the assigned value is a function call, validate the function return type
    if (expressionOrFunc->type == NODE_FUNC_CALL)
    {
        BinaryTreeNode *functoAssign = move_right_until(expressionOrFunc, TOKEN_IDENTIFIER);
        DataType funcTypeReturn = process_validate_func_call(functoAssign, stack);
        if (funcTypeReturn == TYPE_UNKNOWN)
        {
            /* code */
        }
        else if (are_types_compatible(funcTypeReturn, identifier->freturn_type))
        {
            // Error: function return type does not match variable type
            freeTreeFromAnyNode(node);
            free_symbol_stack(stack);
            handle_error(ERR_TYPE_COMPAT);
        }
    }
    // * If the assigned value is an operation (expression), handle it here
    else if (expressionOrFunc->type == NODE_OP && (strcmp(expressionOrFunc->strValue, "null") != 0))
    {
    }
    else
    {
        // * If it's a regular value, check compatibility with the variable's type
        BinaryTreeNode *valuetoAssign = expressionOrFunc;
        DataType valuetoAssign_type = find_return_datatype(valuetoAssign->strValue);

        Symbol *variable = search_symbol_stack(stack, nodeidentifier_str);
        DataType variable_type = variable->type;

        // * Ensure the variable exists and types are compatible
        if (variable == NULL)
        {
            freeTreeFromAnyNode(node);
            free_symbol_stack(stack);
            handle_error(ERR_UNDEFINED_ID);
        }
        else if (!are_types_compatible(valuetoAssign_type, variable_type))
        {
            // Error: type mismatch between assigned value and variable
            freeTreeFromAnyNode(node);
            free_symbol_stack(stack);
            handle_error(ERR_TYPE_COMPAT);
        }
        else if (variable->isConst)
        {
            // Error: cannot modify constant variable
            freeTreeFromAnyNode(node);
            free_symbol_stack(stack);
            handle_error(ERR_TYPE_COMPAT);
        }

        // * Update the variable with the new value (integer type conversion)
        int value_to_assign = atoi(valuetoAssign->strValue); // Use strtol for more control if needed
        upd_var_symbol_stack(stack, variable, (void *)&value_to_assign, valuetoAssign_type);
    }
}

void process_func_def(BinaryTreeNode *funcDefNode, SymbolStack *stack)
{
    if (!funcDefNode)
        return;

    // * Get function name from the node
    BinaryTreeNode *funcName_node = move_right_until(funcDefNode->right, TOKEN_IDENTIFIER);

    // * Temporarily switch to global scope for function definition
    Scope *currentScope = stack->top; // Save the current scope
    while (stack->top->next != NULL)
    {
        stack->top = stack->top->next; // Move to global scope
    }

    // * Process function parameters (if any)
    BinaryTreeNode *funcParams_start = move_left_until(funcName_node, TOKEN_LPAREN);
    Symbol *paramChain = parse_parameters(funcParams_start);

    // * Process the return type of the function
    BinaryTreeNode *funcReturn_type = move_right_until(funcName_node, TOKEN_KEYWORD);
    DataType returnDef_type = value_string_to_type(funcReturn_type->strValue);

    // * Check if the function is `main` and validate its constraints
    if (strcmp(funcName_node->strValue, "main") == 0)
    {
        // Ensure `main` has no parameters and returns `void`
        if (paramChain != NULL)
        {
            // Error: Main should not have parameters
            freeTreeFromAnyNode(funcDefNode);
            free_symbol_stack(stack);
            handle_error(ERR_FUNC_PARAM);
        }

        if (returnDef_type != TYPE_VOID)
        {
            // Error: Main should return `void`
            freeTreeFromAnyNode(funcDefNode);
            free_symbol_stack(stack);
            handle_error(ERR_FUNC_PARAM);
        }
    }

    // * Insert function symbol with its parameters into the symbol stack
    Symbol *funcSymbol = malloc(sizeof(Symbol));
    funcSymbol->name = copy_string(funcName_node->strValue);
    funcSymbol->type = TYPE_FUNCTION;
    funcSymbol->value.params = paramChain; // Set function parameters
    funcSymbol->next = NULL;

    // * Check for function redefinition
    if (search_hash_table(stack->top->table, funcSymbol->name))
    {
        freeTreeFromAnyNode(funcDefNode);
        free_symbol_stack(stack);
        handle_error(ERR_REDEF);
    }

    // Insert function into the symbol table
    insert_symbol_stack(stack, funcSymbol->name, funcSymbol->type, funcSymbol->value.params, false, false, true, returnDef_type);

    // * Restore original scope
    stack->top = currentScope;

    // * Push a new scope for the function body
    push_scope(stack);

    // * Process the function body and the return statement
    BinaryTreeNode *funcBody = move_left_until(funcReturn_type->right, TOKEN_EMPTY);
    BinaryTreeNode *funcReturnExp_type = ProcessTree(funcBody, stack);

    // * Process return type and check for valid return statement
    DataType returnExp_type = process_func_return(funcReturnExp_type, stack);
    bool hasReturn = (strcmp(funcReturnExp_type->strValue, "return") == 0);

    // * Handle void function return constraints
    if (returnDef_type == TYPE_VOID)
    {
        // Void function should not have a return expression
        if (hasReturn)
        {
            if (funcReturnExp_type->left != NULL || funcReturnExp_type->right != NULL)
            {
                freeTreeFromAnyNode(funcDefNode);
                free_symbol_stack(stack);
                handle_error(ERR_FUNC_PARAM);
            }
        }
    }
    else
    {
        // Non-void functions must have a return statement with matching return type
        if (!hasReturn)
        {
            freeTreeFromAnyNode(funcDefNode);
            free_symbol_stack(stack);
            handle_error(ERR_RETURN_EXPR);
        }
        else if (!are_types_compatible(returnExp_type, returnDef_type))
        {
            freeTreeFromAnyNode(funcDefNode);
            free_symbol_stack(stack);
            handle_error(ERR_TYPE_COMPAT);
        }
    }

    // * Pop the function body scope after processing
    pop_scope(stack);
}

Symbol *parse_parameters(BinaryTreeNode *paramsListNode)
{
    if (!paramsListNode)
        return NULL;

    // * Initialize the parameter chain and pointer to last parameter
    Symbol *paramChain = NULL; // Head of the parameter chain
    Symbol *lastParam = NULL;  // Pointer to the last parameter in the chain

    BinaryTreeNode *params_node = paramsListNode->right;

    // * Check if there are no parameters (empty or only closing parenthesis)
    if (params_node->right == NULL || params_node->right->tokenType == TOKEN_RPAREN)
    {
        // No parameters, return NULL
        return NULL;
    }

    // * Iterate through the parameters
    while (params_node != NULL && params_node->tokenType != TOKEN_RPAREN)
    {
        if (params_node->tokenType == TOKEN_IDENTIFIER)
        {
            // * If token is an identifier (parameter name)
            BinaryTreeNode *param_ident = params_node;
            BinaryTreeNode *param_type = move_right_until(param_ident, TOKEN_KEYWORD);
            DataType param_datatype = value_string_to_type(param_type->strValue); // Get the parameter's type

            // * Create a new symbol for the parameter
            Symbol *paramSymbol = malloc(sizeof(Symbol));
            paramSymbol->name = copy_string(param_ident->strValue); // Copy the parameter's name
            paramSymbol->type = param_datatype;
            paramSymbol->next = NULL;

            // * Add the parameter to the parameter chain
            if (lastParam == NULL)
                paramChain = paramSymbol; // Set the head of the chain if it's the first parameter
            else
                lastParam->next = paramSymbol; // Append to the chain
            lastParam = paramSymbol;           // Move the lastParam pointer to the new parameter

            // * Move to the next parameter (skip the comma if present)
            if (param_type->right && param_type->right->tokenType == TOKEN_COMMA)
                params_node = move_right_until(param_type->right, TOKEN_IDENTIFIER); // Move after comma
            else
                params_node = param_type->right; // Move to the next parameter (or end)
        }
    }

    // * Return the head of the parameter chain
    return paramChain;
}

DataType process_validate_func_call(BinaryTreeNode *funcnode, SymbolStack *stack)
{
    // * Check if the function node is NULL
    if (!funcnode)
    {
        printf("Function node is NULL.\n");
        return TYPE_UNKNOWN;
    }

    char *funcName_str = funcnode->strValue; // Extract the function name

    // * Save the current scope and move to the global scope
    Scope *currentScope = stack->top;
    while (stack->top->next != NULL)
    {
        stack->top = stack->top->next; // Move to global scope
    }

    // * Search for the function symbol in the symbol stack
    Symbol *funcSymbol = search_symbol_stack(stack, funcName_str);
    stack->top = currentScope; // Restore the original scope

    // * Check if the function is defined
    if (!funcSymbol)
    {
        fprintf(stderr, "Error: Function '%s' is not defined.\n", funcName_str);
        freeTreeFromAnyNode(funcnode);
        free_symbol_stack(stack);
        handle_error(ERR_UNDEFINED_ID);
    }

    // * Traverse the parameter chain for the function
    Symbol *paramSymbol = funcSymbol->value.params; // Start with the parameter chain

    // * Get the function parameters (arguments) from the function call node
    BinaryTreeNode *funcParams_start = move_left_until(funcnode, TOKEN_LPAREN);
    BinaryTreeNode *argNode = funcParams_start->right;
    int argCount = 0;   // Count of arguments passed in the function call
    int paramCount = 0; // Count of parameters expected by the function

    // * Iterate through the arguments in the function call
    while (argNode && argNode->tokenType != TOKEN_RPAREN)
    {
        if (argNode->tokenType == TOKEN_COMMA)
        {
            // Skip the comma and move to the next argument
            argNode = argNode->right;
            continue;
        }

        if (paramSymbol)
        {
            // * Process the argument and match it with the current parameter
            Symbol *argNode_tofind = search_symbol_stack(stack, argNode->strValue);
            DataType argType;

            // * Find the argument type (either from the symbol table or directly from the node)
            if (argNode_tofind != NULL)
                argType = find_return_datatype(argNode_tofind->name);
            else
                argType = find_return_datatype(argNode->strValue);

            // * Validate that the argument type matches the parameter type
            if (!are_types_compatible(argType, paramSymbol->type))
            {
                fprintf(stderr, "Error: Type mismatch for parameter '%s' in function '%s'. Expected '%s', got '%s'.\n",
                        paramSymbol->name, funcName_str,
                        value_type_to_string(paramSymbol->type),
                        value_type_to_string(argType));
                freeTreeFromAnyNode(funcnode);
                free_symbol_stack(stack);
                handle_error(ERR_FUNC_PARAM);
            }

            // * Move to the next parameter in the chain
            paramSymbol = paramSymbol->next;
            paramCount++;
        }
        else
        {
            // * If there are more arguments than parameters
            fprintf(stderr, "Error: Too many arguments for function '%s'. Expected %d, got more than %d.\n",
                    funcName_str, paramCount, argCount + 1);
            freeTreeFromAnyNode(funcnode);
            free_symbol_stack(stack);
            handle_error(ERR_FUNC_PARAM);
        }

        // * Move to the next argument node
        argNode = argNode->right;
        argCount++;
    }

    // * After processing all arguments, check if there are any missing parameters
    Symbol *missingParam = paramSymbol;
    while (missingParam)
    {
        // * If a parameter is missing, throw an error
        fprintf(stderr, "Error: Missing argument for parameter '%s' in function '%s'.\n",
                missingParam->name, funcName_str);
        freeTreeFromAnyNode(funcnode);
        free_symbol_stack(stack);
        handle_error(ERR_FUNC_PARAM);
        missingParam = missingParam->next;
        paramCount++;
    }

    // * Return the function's return type
    return funcSymbol->freturn_type;
}

DataType process_func_return(BinaryTreeNode *returnNode, SymbolStack *stack)
{
    // * Check if there is no return expression (i.e., both left and right are NULL)
    if (returnNode->left == NULL && returnNode->right == NULL)
    {
        return TYPE_VOID;
    }

    // ! DELETE AFTER process_expression IS IMPLEMENTED
    return TYPE_INT;
    // ! DELETE AFTER process_expression IS IMPLEMENTED
    // * Process the return expression using the process_expression function
    return process_expression(returnNode, stack);
}

DataType process_expression(BinaryTreeNode *returnNode, SymbolStack *stack)
{
    Stack s;
    initStack(&s, rule);
    setStartNodeInOrder(returnNode);
    InOrder(curInOrderNode, &s);

    // PrintAllStack(&s);

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
    // * Get the start of function parameters (move to the right until a token identifier is found)
    BinaryTreeNode *paramsstart = node->left;
    paramsstart = move_right_until(node, TOKEN_IDENTIFIER);

    // * Validate function call (checking the return type of the function)
    DataType funcreturntype = process_validate_func_call(paramsstart, stack);

    // * Check if the return type of the function is 'void'
    if (funcreturntype == TYPE_VOID)
    {
        // Function return type is valid as 'void'
        // printf("Return value for function %s is valid\n", node->strValue);
    }
    else
    {
        // Function return type is not valid (should be 'void')
        // printf("Return value for function %s is NOT valid, expected TYPE_VOID\n", node->strValue);
    }
}

BinaryTreeNode *ProcessTree(BinaryTreeNode *root, SymbolStack *stack)
{
    // * Base condition: If the root is NULL, return immediately
    if (root == NULL)
        return root;

    // Start with the root node of the tree
    BinaryTreeNode *node = root;

    // * Traverse the tree until node becomes NULL
    while (node != NULL)
    {
        // * Check if the node is a general node (not a leaf node)
        if (node->type == NODE_GENERAL)
        {
            node = node->right;

            switch (node->tokenType)
            {
            case TOKEN_KEYWORD:
                if (strcmp(node->strValue, "var") == 0) // Variable declaration
                {
                    process_var_declaration(node, stack);
                }
                else if (strcmp(node->strValue, "const") == 0) // Constant declaration
                {
                    process_var_declaration(node, stack);
                }
                else if (strcmp(node->strValue, "pub") == 0) // Function declaration
                {
                    process_func_def(node, stack);
                }
                else if (strcmp(node->strValue, "return") == 0) // Return statement
                {
                    return node; // Exit early if it's a return statement
                }
                else if (strcmp(node->strValue, "if") == 0) // If statement
                {
                    process_if(node, stack);
                }
                else if (strcmp(node->strValue, "while") == 0) // While loop
                {
                    process_while(node, stack);
                }
                break;

            case TOKEN_IDENTIFIER:
                if (node->right && node->right->tokenType == TOKEN_ASSIGNMENT) // Assignment operator found
                {
                    process_identifier_assign(node, stack);
                }
                else if (node->left && node->left->tokenType == TOKEN_LPAREN) // Function call (parentheses found)
                {
                    process_voidFunc(node, stack); // Process void function call
                }
                break;

            default:
                break;
            }

            // Move to the parent and left node to continue traversal
            node = node->parent;
            node = node->left;
        }

        // * Exit if we've reached the leaf nodes (both left and right children are NULL)
        if (node->left == NULL && node->right == NULL)
        {
            break;
        }
    }

    // Return the root of the tree (possibly modified during traversal)
    return root;
}
