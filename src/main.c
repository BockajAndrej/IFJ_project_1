#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexical_analyser.h"
#include "syntactic_analysis.h"
#include <error.h>
#include "ast.h"
#include "semantic.h"
#include "symtable.h"
#include "Code_generator.h"

int main(int argc, char **argv)
{
    FILE *file;
    BinaryTreeNode *root = createBinaryNode(NODE_GENERAL, TOKEN_EMPTY, "");
    setStartNode(root);

    // Check if file was entered as an argument
    if (argc < 2)
        file = stdin;
    if (argc == 2)
        file = fopen(argv[1], "r");
    else
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 99;
    }

    // // Syntactic analysis
    if (!FIRST(file))
    {
        fprintf(stderr, "%s", " --- WRONG END --- \n");
        // WRONG
        fclose(file);
        handle_error(ERR_SYNTAX);
    }

    //printBinaryTree(root);

    SymbolStack *stack = initialize_symbol_stack();
    ProcessTree(root, stack);
    free_symbol_stack(stack);

    // generator skusky
    processTokenType(root);

    fclose(file);
    return EXIT_SUCCESS;
}