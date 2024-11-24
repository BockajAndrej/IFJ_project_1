#include "syntactic_analysis.h"

// TODO : 1. Pocitanie tokenov (kazdy riadok resetuje)
// TODO : 2. Nefunkcny scope len na jeden riadok (bez {}) vytvorit vo funkcii ktora funkciu scope vola
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
bool STATEMENT(FILE *file)
{
    pmesg(" ------ STATEMENT ------\n");
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
            insertRightMoveRight(currentNode, NODE_VAR, token.type, token.value.valueString.str);
            infestNum++;
            if (!ELSE_DEF(file))
                return false;
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
    moveUp(infestNum);
    infestNum = 0;
    insertLeftMoveLeft(currentNode, NODE_GENERAL, TOKEN_EMPTY, "");
    pmesg(" ------ END STATEMENT ------\n");
    // Recursive calling itself for processing next code out of scope
    if (!STATEMENT(file))
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
    if (token.type != TOKEN_ASSIGNMENT)
        return false;
    if (!ASSIGN_CONST(file))
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
    // t_(
    GET_TOKEN_RAW(token, file);
    insertRightMoveRight(currentNode, NODE_CONST, token.type, token.value.valueString.str);
    infestNum++;
    if (token.type != TOKEN_LPAREN)
        return false;

    GET_TOKEN_RAW(token, file);
    insertRightMoveRight(currentNode, NODE_CONST, token.type, token.value.valueString.str);
    infestNum++;
    if (!EXPRESSION(file, token))
        return false;

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
    // t_(
    Token token;
    GET_TOKEN_RAW(token, file);
    if (token.type != TOKEN_LPAREN)
        return false;

    GET_TOKEN_RAW(token, file);
    if (!EXPRESSION(file, token))
        return false;

    // t_{
    GET_TOKEN_RAW(token, file);
    if (token.type == TOKEN_CURLYL_BRACKET)
    {
        // SCOPE
        if (!SCOPE(file))
            return false;
    }
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
    if (!CALL_EXT(file))
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
        GET_TOKEN_RAW(token, file);
        if (token.type != TOKEN_IDENTIFIER)
            return false;
        GET_TOKEN_RAW(token, file);
        if (token.type != TOKEN_PIPE)
            return false;
        GET_TOKEN_RAW(token, file);
    }
    // SCOPE
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
bool CALL_EXT(FILE *file)
{
    pmesg(" ------ CALL_EXT ------\n");
    Token token;
    GET_TOKEN_RAW(token, file);
    // Function call
    if (token.type == TOKEN_LPAREN)
    {
        if (!ARG(file))
            return false;
        GET_TOKEN_RAW(token, file);
        if (token.type != TOKEN_SEMICOLON)
            return false;
    }
    // Assignment to a variable
    else if (token.type == TOKEN_ASSIGNMENT)
    {
        insertRightMoveRight(currentNode, NODE_FUNC_DEF, token.type, token.value.valueString.str);
        infestNum++;
        GET_TOKEN_RAW(token, file);
        if (!EXPRESSION(file, token))
            return false;
        insertRightMoveRight(currentNode, NODE_FUNC_DEF, token.type, token.value.valueString.str);
        infestNum++;
    }
    // Object function
    else if (token.type == TOKEN_DOT)
    {
        if (!CALL_OBJ(file))
            return false;
    }
    else
        return false;
    pmesg(" ------ END CALL_EXT ------\n");
    return true;
}
// TODO: spracovanie aj s viacerymi parametrami
bool CALL_OBJ(FILE *file)
{
    pmesg(" ------ CALL_OBJ ------\n");
    Token token;
    // t_id
    GET_TOKEN_RAW(token, file);
    if (token.type != TOKEN_IDENTIFIER)
        return false;
    // t_(
    GET_TOKEN_RAW(token, file);
    if (token.type != TOKEN_LPAREN)
        return false;
    // t_string
    // TODO: sem asi skor zadat neterminal arg len vymysliet aby bol string prvy
    GET_TOKEN_RAW(token, file);
    if (token.type != TOKEN_STRING_LITERAL)
        return false;
    // t_)
    GET_TOKEN_RAW(token, file);
    if (token.type != TOKEN_RPAREN)
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
    // Aj pre expression
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
 *  @todo Nefunkcny scope len na jeden riadok (bez {}) vytvorit vo funkcii ktora funkciu scope vola
 */
bool SCOPE(FILE *file)
{
    pmesg(" ------ SCOPE ------\n");
    insertLeftMoveLeft(currentNode, NODE_GENERAL, TOKEN_EMPTY, "");
    infestNum++;
    // Defined parametrers of infestation
    int tmp = infestNum;
    infestNum = 0;
    scopeNum++;
    if (!STATEMENT(file))
        return false;
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
    if (token.type == TOKEN_IDENTIFIER)
    {
        if (!ARGS(file))
            return false;
    }
    else if (token.type == TOKEN_STRING_LITERAL)
    {
        if (!ARGS(file))
            return false;
    }
    else
    {
        if (!EXPRESSION(file, token))
            return false;
    }

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
    // LOGIKA : Ak v expresne bude znak "(" prva zatvorka bude este expression ale ta dalsia uz to bude koncit.
    pmesg(" ------ EXPRESSION ------\n");
    Stack precStack; // Stack pre precedencnu analyzu
    Stack ruleStack; // Ukladanie pravidiel
    Stack_item curPrecItem;
    Stack_item curRuleItem;

    // Pre vkladanie charakteru ktory nie je v tokene
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

    const int N = 10;
    char table[10][10] = {
        {'<', '<', '<', '<', '<', '<', '<', '>', '<', '>'},
        {'>', '>', '>', '<', '<', '<', '<', '>', '<', '>'},
        {'>', '>', '>', '<', '>', '<', '<', '>', '<', '>'},
        {'>', '>', '>', '>', '>', '<', '<', '>', '<', '>'},
        {'>', '>', '>', '>', 'X', '<', '<', '>', '<', '>'},
        {'>', '>', '>', '>', '>', 'X', 'X', '>', 'X', '>'},
        {'<', '<', '<', '<', '<', '<', '<', '=', '<', 'X'},
        {'>', '>', '>', '>', '>', 'X', 'X', '>', 'X', '>'},
        {'>', '>', '>', '>', '>', 'X', 'X', '>', 'X', '>'},
        {'<', '<', '<', '<', '<', '<', '<', 'X', '<', ' '}};

    int tmp_char = find_OP(N, table, token, &precStack);

    // Ends when it gen ' ' (means: $$)
    while (tmp_char != ' ')
    {
        bool isExpressionCorrect = false;

        if (tmp_char == EOF)
        {
            pmesg("ERROR FIND OP\n");
            return false;
        }
        // Redukovanie podla pravidla
        else if (tmp_char == '>')
        {
            printf("- Reduction\n");
            ExpressionFSM state = sStartExc;
            isExpressionCorrect = false;
            do
            {
                RemoveTop(&precStack, &curPrecItem);
                switch (state)
                {
                case sStartExc:
                    if (curPrecItem.data.token_type == TOKEN_IDENTIFIER || curPrecItem.data.token_type == TOKEN_INT_LITERAL || curPrecItem.data.token_type == TOKEN_FLOAT_LITERAL)
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
                printf("STATE = %d", state);
                return false;
            }
            // Vkladanie neterminalu
            curPrecItem.type = precedence;
            curPrecItem.data.isPrec = true;
            curPrecItem.data.token_type = TOKEN_NTERMINAL;
            curPrecItem.data.token_val.valueString = varNotTerminal;
            push(&precStack, curPrecItem);
        }
        // Vkladanie do precStacku
        else if (tmp_char == '<')
        {
            printf("- Shift\n");
            // Vlozenie shift sign
            curPrecItem.data.isPrec = false;
            curPrecItem.data.token_type = TOKEN_EMPTY;
            curPrecItem.data.token_val.valueString = varLexThan;
            pushAfterTerminal(&precStack, curPrecItem);
            // Vlozenie tokenu
            curPrecItem.data.isPrec = true;
            curPrecItem.data.token_type = token.type;
            curPrecItem.data.token_val.valueString = token.value.valueString;
            printf("SHIFT = %s - type: %d\n", token.value.valueString.str, token.type);
            push(&precStack, curPrecItem);
        }
        else if (tmp_char == '=')
        {
            curPrecItem.data.isPrec = true;
            curPrecItem.data.token_type = token.type;
            curPrecItem.data.token_val.valueString = token.value.valueString;
            push(&precStack, curPrecItem);
        }
        // TODO Prerobit aby az nakonci hadzalo false (memory leak)
        else
            return false;
        printf("RULES -----------\n");
        // PrintAllStack(&ruleStack);
        printf("PREC ------------\n");
        // PrintAllStack(&precStack);
        // Znaci ze prebelha redukcia a chceme spracovat rovnaky token
        if (!isExpressionCorrect)
        {
            GET_TOKEN_RAW(token, file);
        }
        tmp_char = find_OP(N, table, token, &precStack);
    }

    // Naplnenie stromu
    bool dirRight = true;
    for (int i = 0; !isEmpty(ruleStack.top); i++)
    {
        RemoveTop(&ruleStack, &curRuleItem);
        if (curRuleItem.data.token_type == TOKEN_IDENTIFIER || curRuleItem.data.token_type == TOKEN_INT_LITERAL || curRuleItem.data.token_type == TOKEN_FLOAT_LITERAL)
        {
            if (dirRight)
            {
                if (i == 0)
                    insertLeft(currentNode, NODE_VAR, curRuleItem.data.token_type, curRuleItem.data.token_val.valueString.str);
                else
                    insertRight(currentNode, NODE_VAR, curRuleItem.data.token_type, curRuleItem.data.token_val.valueString.str);
                dirRight = false;
            }
            else
            {
                insertLeft(currentNode, NODE_VAR, curRuleItem.data.token_type, curRuleItem.data.token_val.valueString.str);
                while (moveUp(1))
                    ;
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
        }
    }

    printf("-------- END OF ABST -------\n RULE:\n");
    // PrintAllStack(&ruleStack);

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
    if (t.keyword_val == KEYWORD_I32 || t.keyword_val == KEYWORD_F64 || t.keyword_val == KEYWORD_U8)
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

// Nájde index operátora v tabuľke
int find_OP(const int N, char table[N][N], Token token, Stack *precStack)
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
        break;
    case TOKEN_RPAREN:
        x = 7;
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
    case TOKEN_SEMICOLON:
        x = 9;
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
    case TOKEN_EMPTY:
    case EOF:
        y = 9;
        break;
    default:
        break;
    }

    printf("IN THAT: X = %d, Y = %d\n", x, y);
    if (x < 0 || y < 0)
        return EOF;
    return table[y][x];
}