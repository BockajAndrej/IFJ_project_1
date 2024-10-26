#include "syntactic_analysis.h"

// TODO : 1. Pocitanie tokenov (kazdy riadok resetuje)
bool FIRST(FILE *file)
{
    pmesg("    ------ FIRST ------\n");
    Token token;

    GET_TOKEN_RAW(token, file);
    // Ukoncenie pri konci suboru
    if (token.type == TOKEN_EOF && token.type != TOKEN_EOL)
        return true;

    switch (token.keyword_val)
    {
    case KEYWORD_PUB:
        if (!FN_DEF(file))
            return false;
        break;
    case KEYWORD_VAR:
    case KEYWORD_CONST:
        if (!VAR_DEF(file))
            return false;
        break;
    default:
        pmesg("UNDEFINED\n");
        return false;
        break;
    }

    if (!FIRST(file))
        return false;
    return true;
}
bool STATEMENT(FILE *file, Token token)
{
    pmesg("    ------ STATEMENT ------\n");
    if (token.type == TOKEN_IDENTIFIER)
        return false;
    else
    {
        switch (token.keyword_val)
        {
        case KEYWORD_CONST:
        case KEYWORD_VAR:
            if (!VAR_DEF(file))
                return false;
            break;
        case KEYWORD_IF:
            return false;
            break;
        case KEYWORD_ELSE:
            return false;
            break;
        case KEYWORD_WHILE:
            return false;
            break;
        case KEYWORD_RETURN:
            return false;
            break;
        default:
            return false;
            break;
        }
    }
    pmesg("    ------ END STATEMENT ------\n");
    return true;
}

bool VAR_DEF(FILE *file)
{
    pmesg("    ------ VARDEF ------\n");
    Token token;
    GET_TOKEN_RAW(token, file);
    if (token.type != TOKEN_IDENTIFIER)
        return false;

    GET_TOKEN_RAW(token, file);
    switch (token.type)
    {
    // ID;
    case TOKEN_SEMICOLON:
        return true;
    // ID =
    // ID :
    case TOKEN_ASSIGNMENT:
    case TOKEN_COLON:
        if (!ASSIGN(file))
            return false;
        break;
    default:
        return false;
        break;
    }
    pmesg("    ------ END VARDEF ------\n");
    return true;
}
bool FN_DEF(FILE *file)
{
    pmesg("    ------ FN_DEF ------\n");
    Token token;
    // t_fn
    GET_TOKEN_RAW(token, file);
    if (token.keyword_val != KEYWORD_FN)
        return false;
    // t_ID
    GET_TOKEN_RAW(token, file);
    if (token.type != TOKEN_IDENTIFIER)
        return false;
    // t_(
    GET_TOKEN_RAW(token, file);
    if (token.type != TOKEN_LPAREN)
        return false;
    // Parametre
    if (!PARAM(file))
        return false;
    // Return type
    GET_TOKEN_RAW(token, file);
    if (!FN_TYPE(token))
        return false;
    // t_{
    GET_TOKEN_RAW(token, file);
    if (token.type == TOKEN_CURLYL_BRACKET)
    {
        // SCOPE
        if (!SCOPE(file))
            return false;
    }

    return true;
}
bool IF_DEF(FILE *file)
{
    pmesg("    ------ IF_DEF ------\n");
    Token token;
    // t_
    GET_TOKEN_RAW(token, file);
    if (token.type != TOKEN_LPAREN)
        return false;
    if (!EXPRESSION(file))
        return false;

    return true;
}

bool IF_EXT(FILE *file)
{
    pmesg("    ------ IF_EXT ------\n");
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
    return true;
}

bool ASSIGN(FILE *file)
{
    pmesg("    ------ ASSIGN ------\n");
    Token token;
    GET_TOKEN_RAW(token, file);
    switch (token.type)
    {
    // @import("ifj24.zig");
    case TOKEN_IMPORT:
        GET_TOKEN_RAW(token, file);
        if (token.type != TOKEN_LPAREN)
            return false;
        GET_TOKEN_RAW(token, file);
        if (token.type != TOKEN_STRING_LITERAL)
            return false;
        GET_TOKEN_RAW(token, file);
        if (token.type != TOKEN_RPAREN)
            return false;
        GET_TOKEN_RAW(token, file);
        if (token.type != TOKEN_SEMICOLON)
            return false;
        break;
    case TOKEN_KEYWORD:
        if (!VAL_TYPE(token))
            return false;
        // Musi zatial koncis s ";"
        if (!EXPRESSION(file))
            return false;
        break;
    default:
        return false;
        break;
    }
    pmesg("    ------ END ASSIGN ------\n");
    return true;
}

bool SCOPE(FILE *file)
{
    pmesg("    ------ SCOPE ------\n");
    Token token;
    GET_TOKEN_RAW(token, file);
    if (!STATEMENT(file, token))
    {
        if (token.type == TOKEN_CURLYR_BRACKET)
            return true;
        return false;
    }
    if (!SCOPE(file))
        return false;
    pmesg("    ------ END SCOPE ------\n");
    return true;
}

bool PARAM(FILE *file)
{
    pmesg("    ------ PARAM ------\n");
    Token token;
    // t_) (end of recursion)
    GET_TOKEN_RAW(token, file);
    if (token.type == TOKEN_RPAREN)
        return true;

    // x : []u8, y : []u8
    switch (token.type)
    {
    case TOKEN_IDENTIFIER:
        // : u8
        GET_TOKEN_RAW(token, file);
        if (token.type != TOKEN_COLON)
            return false;
        GET_TOKEN_RAW(token, file);
        if (!VAL_TYPE(token))
            return false;
        break;
    case TOKEN_COMMA:
        // , y : u8
        GET_TOKEN_RAW(token, file);
        if (token.type != TOKEN_IDENTIFIER)
            return false;
        GET_TOKEN_RAW(token, file);
        if (token.type != TOKEN_COLON)
            return false;
        GET_TOKEN_RAW(token, file);
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
    pmesg("    ------ END PARAM ------\n");
    return true;
}

bool EXPRESSION(FILE *file)
{
    // Zatial musi koncit ";" alebo ")"
    // Logika: ak v expresne bude znak "(" prva zatvorka bude este expression ale ta dalsia uz to bude koncit.
    pmesg("    ------ EXPRESSION ------\n");
    Token token;
    // Konci ak nie je ";" a sucasne ")"
    do
    {
        GET_TOKEN_RAW(token, file);
    } while (token.type != TOKEN_SEMICOLON);

    pmesg("    ------ END EXPRESSION ------\n");
    return true;
}

bool VAR_TYPE(Token t)
{
    if (t.keyword_val == KEYWORD_VAR || t.keyword_val == KEYWORD_CONST)
        return true;
    return false;
}
bool VAL_TYPE(Token t)
{
    if (t.keyword_val == KEYWORD_I32 || t.keyword_val == KEYWORD_F64 || t.keyword_val == KEYWORD_U8)
        return true;
    return false;
}
bool FN_TYPE(Token t)
{
    if (VAL_TYPE(t) || t.keyword_val == KEYWORD_VOID)
        return true;
    return false;
}