#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"


char *copy_str(const char *str) {
    if (str == NULL) return NULL; 

    char *copy = malloc(strlen(str) + 1);
    if (copy) { 
        strcpy(copy, str); 
    }
    return copy; 
}

void free_str(char *str) {
    if (str != NULL) { 
        free(str);     
    }
}


ASTNode* createNode(NodeType type, const char* value) {

    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));

    if (!node) {
        fprintf(stderr, "Error allocating memory for AST node\n");
        exit(1);
    }

    node->type = type;
    node->value = value ? copy_str(value) : NULL;
    node->left = NULL;
    node->right = NULL;
    node->body = NULL;
    node->body_size = 0;
    node->params = NULL;
    node->params_size = 0;

    return node;
}

ASTNode* createFunctionNode(const char* name) {
    return createNode(NODE_FUNC_DEF, name);
}

ASTNode* createBlockNode() {
    return createNode(NODE_BLOCK, NULL);
}

void addChildToArray(ASTNode*** array, int* size, ASTNode* child) {
    if (!array || !size || !child) return;

    ASTNode** new_array = realloc(*array, (*size + 1) * sizeof(ASTNode*));
    if (!new_array) {
        fprintf(stderr, "Error reallocating memory for AST array\n");
        exit(1);
    }

    *array = new_array;
    (*array)[*size] = child;
    (*size)++;
}

void addParameter(ASTNode* func, ASTNode* param) {
    addChildToArray(&func->params, &func->params_size, param);
}


void addStatementToBlock(ASTNode* block, ASTNode* stmt) {
    addChildToArray(&block->body, &block->body_size, stmt);
}



void addChild(ASTNode* parent, ASTNode* child) {
    addChildToArray(&parent->body, &parent->body_size, child);;
}


void freeAST(ASTNode* node) {
    if (!node) return;

    if (node->value) {
        free_str(node->value); 
        node->value = NULL;
    }

    if (node->body) {
        for (int i = 0; i < node->body_size; i++) {
            if (node->body[i]) {
                freeAST(node->body[i]); 
            }
        }
        free(node->body);
        node->body = NULL;
    }

    if (node->params) {
        for (int i = 0; i < node->params_size; i++) {
            if (node->params[i]) {
                freeAST(node->params[i]);
            }
        }
        free(node->params);
        node->params = NULL;
    }


    if (node->left) {
        freeAST(node->left);
        node->left = NULL;
    }
    if (node->right) {
        freeAST(node->right);
        node->right = NULL;
    }

        free(node);
}





void printAST(ASTNode* node, int indent) {
    if (!node) return;

    // Vytváranie odsadenia
    for (int i = 0; i < indent; i++) printf("  ");

    // Výpis uzla podľa jeho typu
    if (node->type == NODE_IF || node->type == NODE_WHILE) {
        printf("%s", node->type == NODE_IF ? "IF" : "WHILE");

        // Výpis podmienky
        if (node->body_size > 0 && node->body[0]) {
            printf(" (");
            printAST(node->body[0], 0); // Podmienka je prvý uzol v body
            printf(")");
        }
        printf("\n");

        // Výpis tela
        for (int i = 1; i < node->body_size; i++) {
            printAST(node->body[i], indent + 1);
        }
    } else if (node->type == NODE_OP) {
        printf("OPERATION (%s)\n", node->value);
    } else if (node->type == NODE_VAR) {
        printf("VARIABLE (%s)\n", node->value);
    } else if (node->type == NODE_CONST) {
        printf("CONSTANT (%s)\n", node->value);
    } else if (node->type == NODE_ASSIGN) {
        printf("ASSIGN (%s)\n", node->value);
        if (node->left) printAST(node->left, indent + 1);
        if (node->right) printAST(node->right, indent + 1);
    } else {
        printf("%s (%s)\n",
            node->type == NODE_FUNC_DEF ? "FUNCTION DEF" :
            node->type == NODE_PROG ? "PROGRAM" : "UNKNOWN",
            node->value ? node->value : "NULL");
    }

    // Rekurzívne spracovanie ďalších uzlov
    if (node->left) printAST(node->left, indent + 1);
    if (node->right) printAST(node->right, indent + 1);
    if (node->body && (node->type != NODE_IF && node->type != NODE_WHILE)) {
        for (int i = 0; i < node->body_size; i++) {
            printAST(node->body[i], indent + 1);
        }
    }
}

