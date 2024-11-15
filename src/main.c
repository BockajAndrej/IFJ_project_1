#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexical_analyser.h"
#include "syntactic_analysis.h"
#include <error.h>
#include "symtable.h"
#include "ast.h"

FILE *file;


void print_table(SymbolTable *table) {
    Symbol *current = table->head;

    printf("Current symbol table:\n");
    while (current) {
        printf(" - Name: %s, ", current->name);

        switch (current->type) {
            case VALUE_INT:
                printf("Value: %d (int)\n", current->value.intValue);
                break;
            case VALUE_FLOAT:
                printf("Value: %.2f (float)\n", current->value.floatValue);
                break;
            case VALUE_STRING:
                printf("Value: %s (string)\n", current->value.strValue);
                break;
        }

        current = current->next;
    }
    printf("\n");
}


void test_hash_table() {
    SymbolTable *table = create_table();
    printf("After creating the table:\n");
    print_table(table);

    int intValue = 10;
    float floatValue = 3.14;
    char *strValue = "Hello";

    insert_symbol(table, "x", VALUE_INT, &intValue);
    printf("After inserting 'x':\n");
    print_table(table);

    insert_symbol(table, "pi", VALUE_FLOAT, &floatValue);
    printf("After inserting 'pi':\n");
    print_table(table);

    insert_symbol(table, "greeting", VALUE_STRING, strValue);
    printf("After inserting 'greeting':\n");
    print_table(table);

    Symbol *s = search_symbol(table, "pi");
    if (s && s->type == VALUE_FLOAT) {
        printf("Found: %s -> %.2f\n", s->name, s->value.floatValue);
    }

    delete_symbol(table, "x");
    printf("After deleting 'x':\n");
    print_table(table);

    free_table(table);
}

void test_ast() {
    // 1. Vytvor koreňový uzol pre "if"
    ASTNode *root = createNode(NODE_IF, NULL);

    // 2. Vytvor podmienku "a > b"
    ASTNode *condition = createNode(NODE_OP, ">");
    condition->left = createNode(NODE_VAR, "a");
    condition->right = createNode(NODE_VAR, "b");
    addChild(root, condition); // Pridaj podmienku do koreňa

    // 3. Telo "if" vetvy: c = a + b
    ASTNode *if_body = createNode(NODE_ASSIGN, "=");
    if_body->left = createNode(NODE_VAR, "c");
    ASTNode *addition = createNode(NODE_OP, "+");
    addition->left = createNode(NODE_VAR, "a");
    addition->right = createNode(NODE_VAR, "b");
    if_body->right = addition;
    addStatementToBlock(root, if_body); // Pridaj telo "if"

    // 4. Telo "else" vetvy: while (d < 10) { d = d + 1; }
    ASTNode *else_body = createNode(NODE_WHILE, NULL);

    ASTNode *loop_condition = createNode(NODE_OP, "<");
    loop_condition->left = createNode(NODE_VAR, "d");
    loop_condition->right = createNode(NODE_CONST, "10");
    addChild(else_body, loop_condition); // Pridaj podmienku do cyklu

    ASTNode *loop_body = createNode(NODE_ASSIGN, "=");
    loop_body->left = createNode(NODE_VAR, "d");
    ASTNode *increment = createNode(NODE_OP, "+");
    increment->left = createNode(NODE_VAR, "d");
    increment->right = createNode(NODE_CONST, "1");
    loop_body->right = increment;
    addStatementToBlock(else_body, loop_body); // Pridaj telo cyklu

    addStatementToBlock(root, else_body); // Pridaj "else" vetvu

    // 5. Vypíš štruktúru AST
    printf("Výpis zložitého AST:\n");
    printAST(root, 0);

    // 6. Uvoľni pamäť
    freeAST(root);

    printf("Zložitý test AST dokončený.\n");
}

int main(int argc, char **argv)
{
    if(argc == 1){
        test_hash_table();
        test_ast();
        return 0;
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

    // Syntactic analysis
    // if (!FIRST(file))
    // {
    //     printf("%s", " --- WRONG END --- \n");
    //     return 1;
    // }

    // printf("%s", " --- ENDED SUCESFULLY --- \n");

    Token token;
    // Print header for clarity
    printf("Token Type\tValue\t\tKeyword Value\n");
    printf("----------------------------------------------------------\n");

    do
    {
        token = get_token(file);
        print_token(token);

        dynamic_string_free(&token.value.valueString);
    } while (token.type != TOKEN_EOF); // Continue until EOF

    fclose(file); // Nezabudni zavrieť súbor
    return EXIT_SUCCESS;
}
