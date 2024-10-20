#ifndef _SYNTACTIC_ANALYSSIS_H
#define _SYNTACTIC_ANALYSIS_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "lexical_analyser.h"
#include "error.h"

// #define NDEBUG

#ifndef NDEBUG // if not defined NDEBUG (no debugging)
#define pmesg(s) fprintf(stderr, "Line: %u - " s, __LINE__)
#else
#define pmesg(...) // v pripade NDEBUG nebude vypisovat hlasenia
#endif

// Get token without comments
#define GET_TOKEN_RAW(token, file) \
    do                            \
    {                             \
        token = get_token(file);  \
    } while (token.type == TOKEN_COMMENT || token.type == TOKEN_EOL); \
    print_token(token);

bool FIRST(FILE *file);

bool STATEMENT(FILE *file, Token token);

bool VAR_DEF(FILE *file);
bool FN_DEF(FILE *file);

bool ASSIGN(FILE *file);

bool SCOPE(FILE *file);

bool PARAM(FILE *file);

bool EXPRESSION(FILE *file);

bool VAR_TYPE();
bool VAL_TYPE();
bool VAL_TYPE_FN();

#endif