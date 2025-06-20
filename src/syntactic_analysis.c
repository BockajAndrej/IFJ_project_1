/**
 * @file syntactic_analysis.c
 * @author Bockaj Andrej
 * @category Syntactic analysis
 * @brief This file contains functions for process input file and verifies syntax  
 */
#include "syntactic_analysis.h"

static int infestNum = 0;
static int scopeNum = 0;
typedef enum
{
    sStartExc,
    sLParExc,
    sLParExc1,
    sNTerExc,
    sNTerExc1,
    sFinalExc,
    sErrorExc
} ExpressionFSM;

/** @brief Processes the FIRST rule in the syntactic analysis.
 *  @details First and the only one function called from main. Verifies part of code which is out of scope.
 *  @param file A pointer to the file being analyzed.
 *  @return If syntactic analysis pass return true otherwise false
 *  Use case:
 *  @code
 *  @import(t_string);
 *  EXPRESSIONS;
 *  @endcode
 */
bool FIRST(FILE *file)
{
    pmesg(" ------ FIRST ------\n");
    Token token;
    GET_TOKEN_RAW(token, file);
    // Ends at the end of file (not at end of line)
    if (token.type == TOKEN_EOF && token.type != TOKEN_EOL)
        return true;
    // Processes functions variables and constants
    switch (token.keyword_val)
    {
    case KEYWORD_PUB:
        insertRightMoveRight(currentNode, NODE_FUNC_DEF, token.type, token.value.valueString.str);
        infestNum++;
        if (!FN_DEF(file))
            return false;
        break;
    case KEYWORD_VAR:
        insertRightMoveRight(currentNode, NODE_VAR_DECL, token.type, token.value.valueString.str);
        infestNum++;
        if (!VAR_DEF(file))
            return false;
        break;
    case KEYWORD_CONST:
        insertRightMoveRight(currentNode, NODE_CONST, token.type, token.value.valueString.str);
        infestNum++;
        if (!CONST_DEF(file))
            return false;
        break;
    default:
        pmesg("UNDEFINED\n");
        return false;
        break;
    }
    moveUp(infestNum);
    infestNum = 0;
    insertLeftMoveLeft(currentNode, NODE_GENERAL, TOKEN_EMPTY, "");
    // Recursive calling itself for processing next code out of scope
    if (!FIRST(file))
        return false;
    return true;
}
/** @brief Processes the commands, coditions, ...
 *  @details Fucntion is called in scope for verification part of code which is inside scope.
 *  @param file A pointer to the file being analyzed.
 *  @return If syntactic analysis pass return true otherwise false
 *  Use case:
 *  @code
 *  @import(t_string);
 *  EXPRESSIONS;
 *  @endcode
 */
bool STATEMENT(FILE *file, int *infestNumLok)
{
    pmesg(" ------ STATEMENT ------\n");
    bool canShift = true;
    Token token;
    GET_TOKEN_RAW(token, file);
    // Recursion ends if scope ends
    if (token.type == TOKEN_CURLYR_BRACKET)
        return true;
    else if (token.type == TOKEN_IDENTIFIER)
    {
        insertRightMoveRight(currentNode, NODE_VAR, token.type, token.value.valueString.str);
        infestNum++;
        if (!CALL_DEF(file))
            return false;
    }
    else if(token.type == TOKEN_SEMICOLON)
        ;
    // For sure if it is not keyword to be return false
    else if (token.type != TOKEN_KEYWORD)
        return false;
    else
    {
        switch (token.keyword_val)
        {
        case KEYWORD_CONST:
            insertRightMoveRight(currentNode, NODE_CONST, token.type, token.value.valueString.str);
            infestNum++;
            if (!CONST_DEF(file))
                return false;
            break;
        case KEYWORD_VAR:
            insertRightMoveRight(currentNode, NODE_VAR, token.type, token.value.valueString.str);
            infestNum++;
            if (!VAR_DEF(file))
                return false;
            break;
        case KEYWORD_IF:
            insertRightMoveRight(currentNode, NODE_VAR, token.type, token.value.valueString.str);
            infestNum++;
            if (!IF_DEF(file))
                return false;
            break;
        case KEYWORD_ELSE:
            moveUp(1);
            moveDownRight(1);
            insertLeftMoveLeft(currentNode, NODE_VAR, token.type, token.value.valueString.str);
            infestNum++;
            if (!ELSE_DEF(file))
                return false;
            moveUp(infestNum + 1);
            infestNum = 0;
            canShift = false;
            moveDownLeft(1);
            break;
        case KEYWORD_WHILE:
            insertRightMoveRight(currentNode, NODE_VAR, token.type, token.value.valueString.str);
            infestNum++;
            if (!WHILE_DEF(file))
                return false;
            break;
        case KEYWORD_RETURN:
            insertRightMoveRight(currentNode, NODE_VAR, token.type, token.value.valueString.str);
            infestNum++;
            if (!RET_DEF(file))
                return false;
            break;
        default:
            return false;
            break;
        }
    }

    // Need to be in condition because token else should have diferent aproach (inside if subtree)
    if (canShift)
    {
        // Create new general_node for next command
        moveUp(infestNum);
        infestNum = 0;
        insertLeftMoveLeft(currentNode, NODE_GENERAL, TOKEN_EMPTY, "");
        // Number of all commands in SCOPE
        *infestNumLok = *infestNumLok + 1;
    }
    pmesg(" ------ END STATEMENT ------\n");
    // Recursive calling itself for processing next code out of scope
    if (!STATEMENT(file, infestNumLok))
        return false;
    return true;
}

/** @brief Processes command for variable declaration
 *  @param file A pointer to the file being analyzed.
 *  @return If syntactic analysis pass return true otherwise false
 *  Use case:
 *  @code
 *  @import(t_string);
 *  EXPRESSIONS;
 *  @endcode
 */
bool VAR_DEF(FILE *file)
{
    pmesg(" ------ VARDEF ------\n");
    Token token;
    // var id
    GET_TOKEN_RAW(token, file);
    insertRightMoveRight(currentNode, NODE_VAR, token.type, token.value.valueString.str);
    infestNum++;
    if (token.type != TOKEN_IDENTIFIER)
        return false;
    GET_TOKEN_RAW(token, file);
    switch (token.type)
    {
    // var id;
    case TOKEN_SEMICOLON:
        return true;
    // var id : ASSIGN_VAR
    case TOKEN_COLON:
        insertRightMoveRight(currentNode, NODE_VAR, token.type, token.value.valueString.str);
        infestNum++;
        if (!ASSIGN_VAR(file))
            return false;
        break;
    // var id = EXP;
    case TOKEN_ASSIGNMENT:
        insertRightMoveRight(currentNode, NODE_VAR, token.type, token.value.valueString.str);
        infestNum++;
        GET_TOKEN_RAW(token, file);
        if (!EXPRESSION(file, token))
            return false;
        break;
    default:
        return false;
        break;
    }
    pmesg(" ------ END VARDEF ------\n");
    return true;
}
/** @brief Processes command for constant declaration
 *  @param file A pointer to the file being analyzed.
 *  @return If syntactic analysis pass return true otherwise false
 *  Use case:
 *  @code
 *  @import(t_string);
 *  EXPRESSIONS;
 *  @endcode
 */
bool CONST_DEF(FILE *file)
{
    pmesg(" ------ CONST_DEF ------\n");
    Token token;
    // const id
    GET_TOKEN_RAW(token, file);
    insertRightMoveRight(currentNode, NODE_CONST, token.type, token.value.valueString.str);
    infestNum++;
    if (token.type != TOKEN_IDENTIFIER)
        return false;
    // const id =
    GET_TOKEN_RAW(token, file);
    insertRightMoveRight(currentNode, NODE_CONST, token.type, token.value.valueString.str);
    infestNum++;
    if (token.type == TOKEN_ASSIGNMENT)
    {
        if (!ASSIGN_CONST(file))
            return false;
    }
    // const id : i32 = EXP
    else if (token.type == TOKEN_COLON)
    {
        GET_TOKEN_RAW(token, file);
        insertRightMoveRight(currentNode, NODE_CONST, token.type, token.value.valueString.str);
        infestNum++;
        if (!VAL_TYPE(token))
            return false;
        GET_TOKEN_RAW(token, file);
        insertRightMoveRight(currentNode, NODE_CONST, token.type, token.value.valueString.str);
        infestNum++;
        if (token.type != TOKEN_ASSIGNMENT)
            return false;
        GET_TOKEN_RAW(token, file);
        if (!EXPRESSION(file, token))
            return false;
    }
    else
        return false;
    pmesg(" ------ END CONST_DEF ------\n");
    return true;
}
/** @brief Processes function declaration
 *  @param file A pointer to the file being analyzed.
 *  @return If syntactic analysis pass return true otherwise false
 *  Use case:
 *  @code
 *  @import(t_string);
 *  EXPRESSIONS;
 *  @endcode
 */
bool FN_DEF(FILE *file)
{
    pmesg(" ------ FN_DEF ------\n");
    Token token;
    // t_fn
    GET_TOKEN_RAW(token, file);
    insertRightMoveRight(currentNode, NODE_CONST, token.type, token.value.valueString.str);
    infestNum++;
    if (token.keyword_val != KEYWORD_FN)
        return false;
    // t_ID
    GET_TOKEN_RAW(token, file);
    insertRightMoveRight(currentNode, NODE_CONST, token.type, token.value.valueString.str);
    infestNum++;
    if (token.type != TOKEN_IDENTIFIER)
        return false;
    // t_(
    GET_TOKEN_RAW(token, file);
    insertLeftMoveLeft(currentNode, NODE_CONST, token.type, token.value.valueString.str);
    if (token.type != TOKEN_LPAREN)
        return false;
    // Parametre
    int tmpinf = infestNum;
    infestNum = 1;
    if (!PARAM(file))
        return false;
    moveUp(infestNum);
    infestNum = tmpinf;
    // Return type
    GET_TOKEN_RAW(token, file);
    insertRightMoveRight(currentNode, NODE_CONST, token.type, token.value.valueString.str);
    infestNum++;
    if (!FN_TYPE(token))
        return false;
    // t_{
    GET_TOKEN_RAW(token, file);
    insertRightMoveRight(currentNode, NODE_CONST, token.type, token.value.valueString.str);
    infestNum++;
    if (token.type == TOKEN_CURLYL_BRACKET)
    {
        // SCOPE
        if (!SCOPE(file))
            return false;
    }

    return true;
}
/** @brief Processes if condition
 *  @param file A pointer to the file being analyzed.
 *  @return If syntactic analysis pass return true otherwise false
 *  Use case:
 *  @code
 *  @import(t_string);
 *  EXPRESSIONS;
 *  @endcode
 */
bool IF_DEF(FILE *file)
{
    pmesg(" ------ IF_DEF ------\n");
    Token token;
    insertRightMoveRight(currentNode, NODE_IF, TOKEN_EMPTY, "AUX");
    infestNum++;
    // t_(
    GET_TOKEN_RAW(token, file);
    insertLeftMoveLeft(currentNode, NODE_IF, token.type, token.value.valueString.str);
    infestNum++;
    if (token.type != TOKEN_LPAREN)
        return false;
    // if(EXP)
    GET_TOKEN_RAW(token, file);
    if (!EXPRESSION(file, token))
        return false;
    // if(EXP)|ID|
    if (!IF_EXT(file))
        return false;
    pmesg(" ------ END IF_DEF ------\n");
    return true;
}
/** @brief Processes else condition
 *  @note Works as preprocessing function for IF_EXT()
 *  @param file A pointer to the file being analyzed.
 *  @return If syntactic analysis pass return true otherwise false
 *  Use case:
 *  @code
 *  @import(t_string);
 *  EXPRESSIONS;
 *  @endcode
 */
bool ELSE_DEF(FILE *file)
{
    pmesg(" ------ ELSE_DEF ------\n");
    Token token;
    // t_{
    GET_TOKEN_RAW(token, file);
    insertRightMoveRight(currentNode, NODE_VAR, token.type, token.value.valueString.str);
    infestNum++;
    if (token.type == TOKEN_CURLYL_BRACKET)
    {
        // SCOPE
        if (!SCOPE(file))
            return false;
    }
    pmesg(" ------ END ELSE_DEF ------\n");
    return true;
}
/** @brief Processes while loop
 *  @param file A pointer to the file being analyzed.
 *  @return If syntactic analysis pass return true otherwise false
 *  Use case:
 *  @code
 *  @import(t_string);
 *  EXPRESSIONS;
 *  @endcode
 */
bool WHILE_DEF(FILE *file)
{
    pmesg(" ------ WHILE_DEF ------\n");
    Token token;
    // t_(
    GET_TOKEN_RAW(token, file);
    insertLeftMoveLeft(currentNode, NODE_IF, token.type, token.value.valueString.str);
    infestNum++;
    if (token.type != TOKEN_LPAREN)
        return false;
    // while(EXP)
    GET_TOKEN_RAW(token, file);
    if (!EXPRESSION(file, token))
        return false;
    // while(EXP)|ID|
    if (!IF_EXT(file))
        return false;
    pmesg(" ------ END WHILE_DEF ------\n");
    return true;
}
/** @brief Processes return command
 *  @param file A pointer to the file being analyzed.
 *  @return If syntactic analysis pass return true otherwise false
 *  Use case:
 *  @code
 *  @import(t_string);
 *  EXPRESSIONS;
 *  @endcode
 */
bool RET_DEF(FILE *file)
{
    pmesg(" ------ RET_DEF ------\n");
    Token token;
    GET_TOKEN_RAW(token, file);
    if (!EXPRESSION(file, token))
        return false;
    pmesg(" ------ END RET_DEF ------\n");
    return true;
}
/** @brief Processes calling functions, variables
 *  @note Works as preprocessing function for IF_EXT()
 *  @param file A pointer to the file being analyzed.
 *  @return If syntactic analysis pass return true otherwise false
 *  Use case:
 *  @code
 *  @import(t_string);
 *  EXPRESSIONS;
 *  @endcode
 */
bool CALL_DEF(FILE *file)
{
    pmesg(" ------ CALL_DEF ------\n");
    if (!CALL_EXT(file, false))
        return false;
    pmesg(" ------ END CALL_DEF ------\n");
    return true;
}

/** @brief Extended function for CALL_DEF()
 *  @details Processes functions arguments or assignment to a variable (= EXPRESSION)
 *  @param file A pointer to the file being analyzed.
 *  @return If syntactic analysis pass return true otherwise false
 *  Use case:
 *  @code
 *  @import(t_string);
 *  EXPRESSIONS;
 *  @endcode
 */
bool IF_EXT(FILE *file)
{
    pmesg(" ------ IF_EXT ------\n");
    Token token;
    // t_| t_id t_|
    GET_TOKEN_RAW(token, file);
    if (token.type == TOKEN_PIPE)
    {
        moveDownRight(1);
        moveDownLeft(1);
        infestNum++;
        insertRightMoveRight(currentNode, NODE_IF, token.type, token.value.valueString.str);
        GET_TOKEN_RAW(token, file);
        insertRightMoveRight(currentNode, NODE_IF, token.type, token.value.valueString.str);
        if (token.type != TOKEN_IDENTIFIER)
            return false;
        GET_TOKEN_RAW(token, file);
        insertRightMoveRight(currentNode, NODE_IF, token.type, token.value.valueString.str);
        if (token.type != TOKEN_PIPE)
            return false;
        moveUp(4);
        GET_TOKEN_RAW(token, file);
    }
    else
    {
        moveDownRight(1);
        infestNum++;
    }
    // SCOPE
    // TODO posuvanie nizsie lebo nieco pravdepodobne v expressions to dava velmi vysoko
    insertRightMoveRight(currentNode, NODE_IF, token.type, token.value.valueString.str);
    infestNum++;
    if (token.type == TOKEN_CURLYL_BRACKET)
    {
        if (!SCOPE(file))
            return false;
    }
    else
        return false;
    pmesg(" ------ END IF_EXT ------\n");
    return true;
}
/** @brief Extended function for CALL_DEF()
 *  @details Processes functions arguments or assignment to a variable (= EXPRESSION)
 *  @param file A pointer to the file being analyzed.
 *  @return If syntactic analysis pass return true otherwise false
 *  Use case:
 *  @code
 *  @import(t_string);
 *  EXPRESSIONS;
 *  @endcode
 */
bool CALL_EXT(FILE *file, bool isAlreadyFn)
{
    pmesg(" ------ CALL_EXT ------\n");
    Token token;
    // Function call from Expression
    if (isAlreadyFn)
    {
        if (!ARG(file))
            return false;
        GET_TOKEN_RAW(token, file);
        if (token.type != TOKEN_SEMICOLON)
            return false;
    }
    else
    {
        GET_TOKEN_RAW(token, file);
        // Function call
        if (token.type == TOKEN_LPAREN)
        {
            insertLeftMoveLeft(currentNode, NODE_FUNC_CALL, token.type, token.value.valueString.str);
            infestNum++;
            if (!ARG(file))
                return false;
            GET_TOKEN_RAW(token, file);
            if (token.type != TOKEN_SEMICOLON)
                return false;
        }
        // Assignment to a variable
        else if (token.type == TOKEN_ASSIGNMENT)
        {
            insertRightMoveRight(currentNode, NODE_VAR, token.type, token.value.valueString.str);
            infestNum++;
            GET_TOKEN_RAW(token, file);
            if (!EXPRESSION(file, token))
                return false;
        }
        else if (token.type == TOKEN_COLON)
        {
            insertRightMoveRight(currentNode, NODE_FUNC_CALL, token.type, token.value.valueString.str);
            infestNum++;
            GET_TOKEN_RAW(token, file);
            insertRightMoveRight(currentNode, NODE_FUNC_CALL, token.type, token.value.valueString.str);
            infestNum++;
            if (!VAL_TYPE(token))
                return false;
            GET_TOKEN_RAW(token, file);
            insertRightMoveRight(currentNode, NODE_FUNC_CALL, token.type, token.value.valueString.str);
            infestNum++;
            if (token.type != TOKEN_ASSIGNMENT)
                return false;
            GET_TOKEN_RAW(token, file);
            if (!EXPRESSION(file, token))
                return false;
        }
        // Object function
        else if (token.type == TOKEN_DOT)
        {
            insertRightMoveRight(currentNode, NODE_FUNC_CALL, token.type, token.value.valueString.str);
            infestNum++;
            if (!CALL_OBJ(file))
                return false;
        }
        else
            return false;
    }
    pmesg(" ------ END CALL_EXT ------\n");
    return true;
}
/** @brief Extended function for CALL_EXT()
 *  @details Processes build in functions with object type 
 *  @param file A pointer to the file being analyzed.
 *  @return If syntactic analysis pass return true otherwise false
 *  Use case:
 *  @code
 *  @import(t_string);
 *  EXPRESSIONS;
 *  @endcode
 */
bool CALL_OBJ(FILE *file)
{
    pmesg(" ------ CALL_OBJ ------\n");
    Token token;
    // t_id
    GET_TOKEN_RAW(token, file);
    insertRightMoveRight(currentNode, NODE_VAR, token.type, token.value.valueString.str);
    infestNum++;
    if (token.type != TOKEN_IDENTIFIER)
        return false;
    // t_(
    GET_TOKEN_RAW(token, file);
    insertLeftMoveLeft(currentNode, NODE_VAR, token.type, token.value.valueString.str);
    infestNum++;
    if (token.type != TOKEN_LPAREN)
        return false;
    // Arguments
    if (!ARG(file))
        return false;
    // t_;
    GET_TOKEN_RAW(token, file);
    if (token.type != TOKEN_SEMICOLON)
        return false;
    pmesg(" ------ END CALL_OBJ ------\n");
    return true;
}

/** @brief Processes assigning expresions to the variable
 *  @details
 *  @param file A pointer to the file being analyzed.
 *  @return If syntactic analysis pass return true otherwise false
 *  Use case:
 *  @code
 *  @import(t_string);
 *  EXPRESSIONS;
 *  @endcode
 */
bool ASSIGN_VAR(FILE *file)
{
    pmesg(" ------ ASSIGN_VAR ------\n");
    Token token;
    // var result : i32 ...
    GET_TOKEN_RAW(token, file);
    insertRightMoveRight(currentNode, NODE_VAR, token.type, token.value.valueString.str);
    infestNum++;
    if (!VAL_TYPE(token))
        return false;
    // var result : i32 = ...
    GET_TOKEN_RAW(token, file);
    insertRightMoveRight(currentNode, NODE_VAR, token.type, token.value.valueString.str);
    infestNum++;
    if (token.type != TOKEN_ASSIGNMENT)
        return false;
    // var result : i32 = 0;
    GET_TOKEN_RAW(token, file);
    if (!EXPRESSION(file, token))
        return false;
    pmesg(" ------ END ASSIGN_VAR ------\n");
    return true;
}
/** @brief Processes assigning expresions to the constant
 *  @details Processes functions arguments or assignment to a variable (= EXPRESSION)
 *  @param file A pointer to the file being analyzed.
 *  @return If syntactic analysis pass return true otherwise false
 *  Use case:
 *  @code
 *  @import(t_string);
 *  EXPRESSIONS;
 *  @endcode
 */
bool ASSIGN_CONST(FILE *file)
{
    pmesg(" ------ ASSIGN_CONST ------\n");
    Token token;
    // Also for expression
    GET_TOKEN_RAW(token, file);
    switch (token.type)
    {
    // @import("ifj24.zig");
    case TOKEN_IMPORT:
        insertRightMoveRight(currentNode, NODE_CONST, token.type, token.value.valueString.str);
        infestNum++;
        GET_TOKEN_RAW(token, file);
        insertRightMoveRight(currentNode, NODE_CONST, token.type, token.value.valueString.str);
        infestNum++;
        if (token.type != TOKEN_LPAREN)
            return false;
        GET_TOKEN_RAW(token, file);
        insertRightMoveRight(currentNode, NODE_CONST, token.type, token.value.valueString.str);
        infestNum++;
        if (token.type != TOKEN_STRING_LITERAL)
            return false;
        GET_TOKEN_RAW(token, file);
        insertRightMoveRight(currentNode, NODE_CONST, token.type, token.value.valueString.str);
        infestNum++;
        if (token.type != TOKEN_RPAREN)
            return false;
        GET_TOKEN_RAW(token, file);
        insertRightMoveRight(currentNode, NODE_CONST, token.type, token.value.valueString.str);
        infestNum++;
        if (token.type != TOKEN_SEMICOLON)
            return false;
        break;
    default:
        if (!EXPRESSION(file, token))
            return false;
        break;
    }
    pmesg(" ------ END ASSIGN_CONST ------\n");
    return true;
}

/** @brief Function for determining the depth of infestation
 *  @details In scope are Statements. Everything which can be between {} except declaration of functions.
 *  @param file A pointer to the file being analyzed.
 *  @return If syntactic analysis pass return true otherwise false
 *  Use case:
 *  @code
 *  @import(t_string);
 *  EXPRESSIONS;
 *  @endcode
 *  @todo Nefunkcny scope len na jeden riadok (bez {})
 */
bool SCOPE(FILE *file)
{
    pmesg(" ------ SCOPE ------\n");
    insertLeftMoveLeft(currentNode, NODE_GENERAL, TOKEN_EMPTY, "");
    infestNum++;
    // Defined parametrers of infestation
    int tmp = infestNum;
    int tmpLokinfest = 0;
    infestNum = 0;
    scopeNum++;
    if (!STATEMENT(file, &tmpLokinfest))
        return false;
    // printf("SCOPE inf = %d\n", infestNum);
    // printf("SCOPE num = %d\n", scopeNum);
    moveUp(tmpLokinfest);
    scopeNum--;
    infestNum = tmp;
    pmesg(" ------ END SCOPE ------\n");
    return true;
}
/** @brief Function for processing parameters of declaraced function
 *  @param file A pointer to the file being analyzed.
 *  @return If syntactic analysis pass return true otherwise false
 *  Use case:
 *  @code
 *  t_)
 *  x : u8
 *  , y : u8
 *  @endcode
 *  @todo x : []u8, y : []u8
 */
bool PARAM(FILE *file)
{
    pmesg(" ------ PARAM ------\n");
    Token token;
    // t_) (end of recursion)
    GET_TOKEN_RAW(token, file);
    insertRightMoveRight(currentNode, NODE_CONST, token.type, token.value.valueString.str);
    infestNum++;
    if (token.type == TOKEN_RPAREN)
        return true;

    // x : []u8, y : []u8
    switch (token.type)
    {
    case TOKEN_IDENTIFIER:
        // : u8
        GET_TOKEN_RAW(token, file);
        insertRightMoveRight(currentNode, NODE_CONST, token.type, token.value.valueString.str);
        infestNum++;
        if (token.type != TOKEN_COLON)
            return false;
        GET_TOKEN_RAW(token, file);
        insertRightMoveRight(currentNode, NODE_CONST, token.type, token.value.valueString.str);
        infestNum++;
        if (!VAL_TYPE(token))
            return false;
        break;
    case TOKEN_COMMA:
        // , y : u8
        GET_TOKEN_RAW(token, file);
        insertRightMoveRight(currentNode, NODE_CONST, token.type, token.value.valueString.str);
        infestNum++;
        if (token.type != TOKEN_IDENTIFIER)
            return false;
        GET_TOKEN_RAW(token, file);
        insertRightMoveRight(currentNode, NODE_CONST, token.type, token.value.valueString.str);
        infestNum++;
        if (token.type != TOKEN_COLON)
            return false;
        GET_TOKEN_RAW(token, file);
        insertRightMoveRight(currentNode, NODE_CONST, token.type, token.value.valueString.str);
        infestNum++;
        if (!VAL_TYPE(token))
            return false;
        break;
    default:
        return false;
        break;
    }
    // Next parameter
    if (!PARAM(file))
        return false;
    pmesg(" ------ END PARAM ------\n");
    return true;
}

/** @brief Function for processing argument of called function
 *  @param file A pointer to the file being analyzed.
 *  @return If syntactic analysis pass return true otherwise false
 *  Use case:
 *  @code
 *  t_id
 *  t_string
 *  EXPRESSION
 *  @endcode
 */
bool ARG(FILE *file)
{
    pmesg(" ------ ARG ------\n");
    Token token;
    GET_TOKEN_RAW(token, file);
    insertRightMoveRight(currentNode, NODE_FUNC_CALL, token.type, token.value.valueString.str);
    infestNum++;
    if (token.type == TOKEN_IDENTIFIER || token.type == TOKEN_STRING_LITERAL || token.type == TOKEN_INT_LITERAL || token.type == TOKEN_FLOAT_LITERAL)
    {
        if (!ARGS(file))
            return false;
    }
    else if (token.type == TOKEN_RPAREN)
        ;
    else 
        return false;
    pmesg(" ------ END ARG ------\n");
    return true;
}
/** @brief Function for processing another argument which is in queue
 *  @details First argument is processed in ARG() but after that should be character ",". That sequence is repeaded unltil ")"
 *  @param file A pointer to the file being analyzed.
 *  @return If syntactic analysis pass return true otherwise false
 *  Use case:
 *  @code
 *  t_)
 *  t_,
 *  @endcode
 */
bool ARGS(FILE *file)
{
    pmesg(" ------ ARGS ------\n");
    Token token;
    GET_TOKEN_RAW(token, file);
    insertRightMoveRight(currentNode, NODE_FUNC_CALL, token.type, token.value.valueString.str);
    infestNum++;
    if (token.type == TOKEN_RPAREN)
        return true;
    if (token.type == TOKEN_COMMA)
    {
        if (!ARG(file))
            return false;
    }
    else
        return false;

    pmesg(" ------ END ARGS ------\n");
    return true;
}

/** @brief Function for processing another argument which is in queue
 *  @warning First token is passed by argument (FIX)
 *  @details It ends when ";" or ")" . Also works: ((2+3)*6)-9; Also works: factorial(256*(56-7));
 *  @note It can ends with ")" because of declaration of function
 *  @param file A pointer to the file being analyzed.
 *  @return If syntactic analysis pass return true otherwise false
 *  Use case:
 *  @code
 *  ...
 *  @endcode
 * @todo Musi obsahovat aspon 1 cislo (bude sa osetrovat v semantickej)
 * @todo Problem pri const a = fn(a) + fn(b); (program ocakava za fn() znak ';')
 * @todo Fix first token
 */
bool EXPRESSION(FILE *file, Token token)
{
    pmesg(" ------ EXPRESSION ------\n");
    Stack precStack; 
    Stack ruleStack; 
    Stack_item curPrecItem;
    Stack_item curRuleItem;

    // For inserting character that is not token 
    Dynamic_string varLexThan;
    if (!dynamic_string_init(&varLexThan))
    {
        fprintf(stderr, "Chyba: dynamic string init.\n");
        exit(EXIT_FAILURE);
    }
    dynamic_string_add_char(&varLexThan, '<');
    Dynamic_string varNotTerminal;
    if (!dynamic_string_init(&varNotTerminal))
    {
        fprintf(stderr, "Chyba: dynamic string init.\n");
        exit(EXIT_FAILURE);
    }
    dynamic_string_add_char(&varNotTerminal, 'E');

    curPrecItem.type = precedence;
    curRuleItem.type = rule;

    initStack(&precStack, curPrecItem.type);
    initStack(&ruleStack, curRuleItem.type);

    int numOfLPar = 0;
    int tmp_char = 0;
    const int N = 11;
    char table[11][11] = {
        {'X', '<', '<', '<', '<', '<', '<', '>', '<', '<', '>'},
        {'>', '>', '>', '<', '<', '<', '<', '>', '<', 'X', '>'},
        {'>', '>', '>', '<', '>', '<', '<', '>', '<', 'X', '>'},
        {'>', '>', '>', '>', '>', '<', '<', '>', '<', 'X', '>'},
        {'>', '>', '>', '>', '>', '<', '<', '>', '<', 'X', '>'},
        {'>', '>', '>', '>', '>', 'X', 'X', '>', 'X', 'X', '>'},
        {'<', '<', '<', '<', '<', '<', '<', '=', '<', '<', 'X'},
        {'>', '>', '>', '>', '>', 'X', 'X', '>', 'X', 'X', '>'},
        {'>', '>', '>', '>', '>', 'X', 'X', '>', 'X', 'X', '>'},
        {'>', 'X', 'X', 'X', 'X', 'X', 'X', '>', 'X', 'X', '>'},
        {'<', '<', '<', '<', '<', '<', '<', 'X', '<', '<', ' '}};

    find_OP(N, table, token, &precStack, &tmp_char, &numOfLPar);

    // Ends when it gen ' ' (means: $$)
    while (tmp_char != ' ')
    {
        bool isExpressionCorrect = false;

        if (tmp_char == EOF)
        {
            pmesg("ERROR FIND OP\n");
            return false;
        }
        // Reduction by the rule
        else if (tmp_char == '>')
        {
            // printf("- Reduction\n");
            ExpressionFSM state = sStartExc;
            isExpressionCorrect = false;
            do
            {
                RemoveTop(&precStack, &curPrecItem);
                switch (state)
                {
                case sStartExc:
                    if (curPrecItem.data.token_type == TOKEN_IDENTIFIER || curPrecItem.data.token_type == TOKEN_INT_LITERAL || curPrecItem.data.token_type == TOKEN_FLOAT_LITERAL || curPrecItem.data.token_type == TOKEN_NULL)
                    {
                        curRuleItem.type = rule;
                        curRuleItem.data.isPrec = false;
                        curRuleItem.data.token_val.valueString = curPrecItem.data.token_val.valueString;
                        curRuleItem.data.token_type = curPrecItem.data.token_type;
                        push(&ruleStack, curRuleItem);
                        state = sFinalExc;
                    }
                    else if (curPrecItem.data.token_type == TOKEN_RPAREN)
                        state = sLParExc;
                    else if (curPrecItem.data.token_type == TOKEN_NTERMINAL)
                        state = sNTerExc;
                    break;
                case sLParExc:
                    if (curPrecItem.data.token_type == TOKEN_NTERMINAL)
                        state = sLParExc1;
                    break;
                case sLParExc1:
                    if (curPrecItem.data.token_type == TOKEN_LPAREN)
                        state = sFinalExc;
                    break;
                case sNTerExc:
                    switch (curPrecItem.data.token_type)
                    {
                    case TOKEN_EQUAL:
                    case TOKEN_NOT_EQUAL:
                    case TOKEN_LESS_EQUAL:
                    case TOKEN_LESS_THAN:
                    case TOKEN_GREATER_EQUAL:
                    case TOKEN_GREATER_THAN:
                    case TOKEN_ADDITION:
                    case TOKEN_SUBTRACTION:
                    case TOKEN_MULTIPLY:
                    case TOKEN_DIVISION:
                        state = sNTerExc1;
                        break;
                    default:
                        state = sErrorExc;
                        break;
                    }
                    curRuleItem.type = rule;
                    curRuleItem.data.isPrec = false;
                    curRuleItem.data.token_val.valueString = curPrecItem.data.token_val.valueString;
                    curRuleItem.data.token_type = curPrecItem.data.token_type;
                    push(&ruleStack, curRuleItem);
                    break;
                case sNTerExc1:
                    if (curPrecItem.data.token_type == TOKEN_NTERMINAL)
                        state = sFinalExc;
                    break;
                case sFinalExc:
                    isExpressionCorrect = true;
                    break;
                case sErrorExc:
                default:
                    break;
                }
            } while (curPrecItem.data.isPrec);

            if (!isExpressionCorrect)
            {
                // printf("STATE = %d", state);
                return false;
            }
            // pushing N-terminal
            curPrecItem.type = precedence;
            curPrecItem.data.isPrec = true;
            curPrecItem.data.token_type = TOKEN_NTERMINAL;
            curPrecItem.data.token_val.valueString = varNotTerminal;
            push(&precStack, curPrecItem);
        }
        // push terminal into stack
        else if (tmp_char == '<')
        {
            // printf("- Shift\n");
            // Insert shift sign
            curPrecItem.data.isPrec = false;
            curPrecItem.data.token_type = TOKEN_EMPTY;
            curPrecItem.data.token_val.valueString = varLexThan;
            pushAfterTerminal(&precStack, curPrecItem);
            // Insert token
            curPrecItem.data.isPrec = true;
            curPrecItem.data.token_type = token.type;
            curPrecItem.data.token_val.valueString = token.value.valueString;
            if(token.type == TOKEN_RPAREN)
                numOfLPar = numOfLPar - 1;
            // printf("SHIFT = %s - type: %d\n", token.value.valueString.str, token.type);
            push(&precStack, curPrecItem);
        }
        else if (tmp_char == '=')
        {
            curPrecItem.data.isPrec = true;
            curPrecItem.data.token_type = token.type;
            curPrecItem.data.token_val.valueString = token.value.valueString;
            push(&precStack, curPrecItem);
        }
        else{
            // Deallocation memory
            freeStack(&precStack);
            freeStack(&ruleStack);
            return false;
        }

        // It means that reduction was made and we want to  process same token
        if (!isExpressionCorrect)
        {
            GET_TOKEN_RAW(token, file);
            Stack_item tmpLastItem;
            getElement(&precStack, &tmpLastItem);
            if (tmpLastItem.data.token_type == TOKEN_IDENTIFIER && token.type == TOKEN_DOT)
            {
                insertRightMoveRight(currentNode, NODE_FUNC_CALL, tmpLastItem.data.token_type, tmpLastItem.data.token_val.valueString.str);
                infestNum++;
                insertRightMoveRight(currentNode, NODE_FUNC_CALL, token.type, token.value.valueString.str);
                infestNum++;
                if (!CALL_OBJ(file))
                    return false;
                break;
            }
            else if (tmpLastItem.data.token_type == TOKEN_IDENTIFIER && token.type == TOKEN_LPAREN)
            {
                insertRightMoveRight(currentNode, NODE_FUNC_CALL, tmpLastItem.data.token_type, tmpLastItem.data.token_val.valueString.str);
                infestNum++;
                insertLeftMoveLeft(currentNode, NODE_FUNC_CALL, token.type, token.value.valueString.str);
                infestNum++;
                if (!CALL_EXT(file, true))
                    return false;
                break;
            }
        }
        // PrintAllStack(&precStack);
        find_OP(N, table, token, &precStack, &tmp_char, &numOfLPar);
    }

    // Inserting into tree
    bool dirRight = true;
    int i = 0;
    for (i = 0; !isEmpty(ruleStack.top); i++)
    {
        RemoveTop(&ruleStack, &curRuleItem);
        if (curRuleItem.data.token_type == TOKEN_IDENTIFIER || curRuleItem.data.token_type == TOKEN_INT_LITERAL || curRuleItem.data.token_type == TOKEN_FLOAT_LITERAL)
        {
            if (dirRight)
            {
                if (i == 0)
                    insertLeft(currentNode, NODE_VAR, curRuleItem.data.token_type, curRuleItem.data.token_val.valueString.str);
                else
                {
                    insertRight(currentNode, NODE_VAR, curRuleItem.data.token_type, curRuleItem.data.token_val.valueString.str);
                    dirRight = false;
                }
            }
            else
            {
                insertLeft(currentNode, NODE_VAR, curRuleItem.data.token_type, curRuleItem.data.token_val.valueString.str);
                while (!moveUp(0))
                {
                    // printf("_________HERE_______ : %s\n", curRuleItem.data.token_val.valueString.str);
                    moveUp(1);
                    infestNum--;
                }
                moveUp(1);
                infestNum--;
            }
        }
        else
        {
            if (i == 0)
                insertLeftMoveLeft(currentNode, NODE_OP, curRuleItem.data.token_type, curRuleItem.data.token_val.valueString.str);
            else if (dirRight)
                insertRightMoveRight(currentNode, NODE_OP, curRuleItem.data.token_type, curRuleItem.data.token_val.valueString.str);
            else
            {
                insertLeftMoveLeft(currentNode, NODE_OP, curRuleItem.data.token_type, curRuleItem.data.token_val.valueString.str);
                dirRight = true;
            }
            infestNum++;
        }
    }
    if (i == 1)
    {
        moveUp(1);
        infestNum--;
        if(infestNum != 0){
            moveUp(1);
            infestNum--;
        }
    }
    
    // Deallocation memory
    freeStack(&precStack);
    freeStack(&ruleStack);
    pmesg(" ------ END EXPRESSION ------\n");
    return true;
}

/** @brief Function for verifaction enabled data types (VARIABLE/CONST)
 *  @return If token is enabled data type return true otherwise false
 */
bool VAR_TYPE(Token t)
{
    if (t.keyword_val == KEYWORD_VAR || t.keyword_val == KEYWORD_CONST)
        return true;
    return false;
}
/** @brief Function for verifaction enabled data types (DATA TYPES)
 *  @return If token is enabled data type return true otherwise false
 */
bool VAL_TYPE(Token t)
{
    if (t.keyword_val == KEYWORD_I32 || t.keyword_val == KEYWORD_F64 || t.keyword_val == KEYWORD_U8 || t.keyword_val == KEYWORD_U8_ARRAY || t.keyword_val == KEYWORD_I32_NULL || t.keyword_val == KEYWORD_F64_NULL || t.keyword_val == KEYWORD_U8_NULL || t.keyword_val == KEYWORD_U8_ARRAY_NULL)
        return true;
    return false;
}
/** @brief Function for verifaction enabled data types (FUNCTIONS)
 *  @return If token is enabled data type return true otherwise false
 */
bool FN_TYPE(Token t)
{
    if (VAL_TYPE(t) || t.keyword_val == KEYWORD_VOID)
        return true;
    return false;
}

// Searching for operator index in precedence table
void find_OP(const int N, char table[N][N], Token token, Stack *precStack, int *character, int *numOfLPar)
{
    int x = -1, y = -1;

    switch (token.type)
    {
    case TOKEN_EQUAL:
    case TOKEN_NOT_EQUAL:
    case TOKEN_LESS_EQUAL:
    case TOKEN_LESS_THAN:
    case TOKEN_GREATER_EQUAL:
    case TOKEN_GREATER_THAN:
        x = 0;
        break;
    case TOKEN_ADDITION:
        x = 1;
        break;
    case TOKEN_SUBTRACTION:
        x = 2;
        break;
    case TOKEN_MULTIPLY:
        x = 3;
        break;
    case TOKEN_DIVISION:
        x = 4;
        break;
    case TOKEN_LPAREN:
        x = 6;
        *numOfLPar = *numOfLPar + 1;
        break;
    case TOKEN_RPAREN:
        x = 7;
        if (*numOfLPar <= 0)
            x = 10;            
        break;
    case TOKEN_IDENTIFIER:
        x = 5;
        break;
    case TOKEN_INT_LITERAL:
        x = 8;
        break;
    case TOKEN_FLOAT_LITERAL:
        x = 8;
        break;
    case TOKEN_NULL:
        x = 9;
        break;
    case TOKEN_SEMICOLON:
        x = 10;
        break;
    default:
        break;
    }

    switch (topTerminal(precStack))
    {
    case TOKEN_EQUAL:
    case TOKEN_NOT_EQUAL:
    case TOKEN_LESS_EQUAL:
    case TOKEN_LESS_THAN:
    case TOKEN_GREATER_EQUAL:
    case TOKEN_GREATER_THAN:
        y = 0;
        break;
    case TOKEN_ADDITION:
        y = 1;
        break;
    case TOKEN_SUBTRACTION:
        y = 2;
        break;
    case TOKEN_MULTIPLY:
        y = 3;
        break;
    case TOKEN_DIVISION:
        y = 4;
        break;
    case TOKEN_IDENTIFIER:
        y = 5;
        break;
    case TOKEN_LPAREN:
        y = 6;
        break;
    case TOKEN_RPAREN:
        y = 7;
        break;
    case TOKEN_INT_LITERAL:
    case TOKEN_FLOAT_LITERAL:
        y = 8;
        break;
    case TOKEN_NULL:
        y = 9;
        break;
    case TOKEN_EMPTY:
    case EOF:
        y = 10;
        break;
    default:
        break;
    }

    // printf("IN THAT: X = %d, Y = %d\n", x, y);
    if (x < 0 || y < 0)
        *character = EOF;
    else
        *character = table[y][x];
}