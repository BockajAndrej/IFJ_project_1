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

int main(int argc, char **argv)
{
    FILE *file;
    BinaryTreeNode *root = createBinaryNode(NODE_GENERAL, TOKEN_EMPTY, "");
    setStartNode(root);

    if (argc == 1)
    {
        // test_hash_table();
        // return 0;
    }

    // Skontroluj, či bol zadaný súbor ako argument
    if (argc < 2)
        file = stdin;
    if (argc == 2)
        file = fopen(argv[1], "r");
    else
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    if (file == NULL)
        perror("Failed to open file");

    // // Syntactic analysis
    if (!FIRST(file))
    {
        printf("%s", " --- WRONG END --- \n");
        return 1;
    }
    printBinaryTree(root);

    SymbolStack *stack = initialize_symbol_stack();
    ProcessTree(root, stack);

    printf("%s", " --- ENDED SUCESFULLY --- \n");
    fclose(file); // Nezabudni zavrieť súbor
    return EXIT_SUCCESS;
}
