/**
 * @file syntactic_analysis.h
 * @author Bockaj Andrej
 * @category Syntactic analysis
 * @brief This file contains functions for process input file and verifies syntax  
 */
#ifndef _SYNTACTIC_ANALYSSIS_H
#define _SYNTACTIC_ANALYSIS_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "lexical_analyser.h"
#include "ast.h"
#include "stack.h"
#include "error.h"

#define NDEBUG

#ifndef NDEBUG // if not defined NDEBUG (no debugging)
#define pmesg(s) fprintf(stderr, "Line: %u - " s, __LINE__)
#else
#define pmesg(...) // in case NDEBUG will not print notifications
#endif

// Get token without comments
#define GET_TOKEN_RAW(token, file) \
    do                            \
    {                             \
        token = get_token(file);  \
    } while (token.type == TOKEN_COMMENT || token.type == TOKEN_EOL); 
    // print_token(token)

bool FIRST(FILE *file);
bool STATEMENT(FILE *file, int *infestNumLok);

bool VAR_DEF(FILE *file);
bool CONST_DEF(FILE *file);
bool FN_DEF(FILE *file);
bool IF_DEF(FILE *file);
bool ELSE_DEF(FILE *file);
bool WHILE_DEF(FILE *file);
bool RET_DEF(FILE *file);
bool CALL_DEF(FILE *file);

bool IF_EXT(FILE *file);
bool CALL_EXT(FILE *file, bool isAlreadyFn);
bool CALL_OBJ(FILE *file);

bool ASSIGN_VAR(FILE *file);
bool ASSIGN_CONST(FILE *file);

bool SCOPE(FILE *file);

bool PARAM(FILE *file);

bool ARG(FILE *file);
bool ARGS(FILE *file);

bool EXPRESSION(FILE *file, Token token);

bool VAR_TYPE(Token t);
bool VAL_TYPE(Token t);
bool FN_TYPE(Token t);

void find_OP(const int N, char table[N][N], Token token, Stack *precStack, int *character, int *numOfLPar);
#endif