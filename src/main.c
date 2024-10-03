#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <Lexical_analyser.h>
#include <error.h>
FILE file;

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    if (argc > 1)
    {
        fprintf(stderr, "Program launched with more than 1 parameter.\n");
        return IO_ERR;
    }

    return SUCCESSFULL_ERR;
}