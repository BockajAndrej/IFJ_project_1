#include "syntactic_analysis.h"
#include "lexical_analyser.h"

//TODO : 1. Prazdny token (t = epsilon)
void FIRST(FILE *file){
    if(file == NULL)
        return;
    Token token = get_token(file);

    if(token.type == TOKEN_KEYWORD)
        printf("Type == keyword");
    if(memcmp(token.value.valueString.str, "const", 5) == 0)
        printf("KEY == const");

    // switch (t.value.keyword)
    // {
    // case KEYWORD_PUB:
    //     printf("PUB");
    //     break;
    // case KEYWORD_VAR:
    //     printf("VAR");
    //     break;
    // case KEYWORD_CONST:
    //     printf("CONST");
    //     break;
    // default:
    //     printf("UNDEFINED");
    //     break;
    // }
    FIRST(NULL);
}