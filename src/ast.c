#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

// Pomocná funkcia na kopírovanie reťazca
char* copy_str(const char* str) {
    if (!str) return NULL;
    char* copy = malloc(strlen(str) + 1);
    if (!copy) {
        fprintf(stderr, "Error: Memory allocation failed for string copy.\n");
        exit(1);
    }
    strcpy(copy, str);
    return copy;
}

//VYTVORENIE UZLA
BinaryTreeNode* createBinaryNode(NodeType type, DataType dataType, const char* value) {
    BinaryTreeNode* node = malloc(sizeof(BinaryTreeNode));
    if (!node) {
        fprintf(stderr, "Error: Memory allocation failed for BinaryTreeNode.\n");
        exit(1);
    }
    node->type = type;
    node->dataType = dataType;
    node->valueType = AST_VALUE_STRING; // Predvolený typ
    node->value.strValue = copy_str(value); // Uloženie hodnoty
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

//VLOZENIE V LAVO
void insertLeft(BinaryTreeNode* parent, NodeType type, DataType dataType, const char* value) {
    if (!parent) {
        fprintf(stderr, "Error: Parent node is NULL.\n");
        return;
    }
    if (parent->left) {
        fprintf(stderr, "Error: Left child already exists for node %s.\n", parent->value.strValue);
        return;
    }
    BinaryTreeNode* child = createBinaryNode(type, dataType, value);
    child->parent = parent;
    parent->left = child;
}

//VLOZENIA V PRAVO
void insertRight(BinaryTreeNode* parent, NodeType type, DataType dataType, const char* value) {
    if (!parent) {
        fprintf(stderr, "Error: Parent node is NULL.\n");
        return;
    }
    if (parent->right) {
        fprintf(stderr, "Error: Right child already exists for node %s.\n", parent->value.strValue);
        return;
    }
    BinaryTreeNode* child = createBinaryNode(type, dataType, value);
    child->parent = parent;
    parent->right = child;
}

//PREMENNA NA SLEDOVANIE UZLA
BinaryTreeNode* currentNode = NULL;




//POHYB
void setStartNode(BinaryTreeNode* root) {
    currentNode = root;
}

void moveUp() {
    if (currentNode && currentNode->parent) {
        currentNode = currentNode->parent;
    } else {
        printf("Error: Cannot move up, current node has no parent.\n");
    }
}

void moveDownLeft() {
    if (currentNode && currentNode->left) {
        currentNode = currentNode->left;
    } else {
        printf("Error: Cannot move left from the current node.\n");
    }
}

void moveDownRight() {
    if (currentNode && currentNode->right) {
        currentNode = currentNode->right;
    } else {
        printf("Error: Cannot move right from the current node.\n");
    }
}

NodeInfo getNodeInfo(BinaryTreeNode* node) {
    NodeInfo info = {0};

    if (!node) {
        info.value = "NULL"; // Ak je uzol NULL, vrátime informácie s hodnotou NULL
        return info;
    }

    info.type = node->type;
    info.dataType = node->dataType;

    // Nastavenie hodnoty uzla
    if (node->valueType == AST_VALUE_STRING && node->value.strValue) {
        info.value = node->value.strValue;
    } else if (node->valueType == AST_VALUE_INT) {
        static char buffer[20];
        snprintf(buffer, sizeof(buffer), "%d", node->value.intValue);
        info.value = buffer;
    } else if (node->valueType == AST_VALUE_FLOAT) {
        static char buffer[20];
        snprintf(buffer, sizeof(buffer), "%.2f", node->value.floatValue);
        info.value = buffer;
    } else {
        info.value = "NULL";
    }

    // Nastavenie informácií o deťoch
    info.hasLeft = (node->left != NULL);
    info.hasRight = (node->right != NULL);

    // Nastavenie hodnoty rodiča
    if (node->parent && node->parent->value.strValue) {
        info.parentValue = node->parent->value.strValue;
    } else {
        info.parentValue = "NULL";
    }

    return info;
}




//FREE A VYPIS
void freeBinaryTree(BinaryTreeNode* root) {
    if (!root) return;

    freeBinaryTree(root->left);
    freeBinaryTree(root->right);

    if (root->valueType == AST_VALUE_STRING && root->value.strValue) {
        free(root->value.strValue);
    }
    free(root);
}

void printBinaryTree(BinaryTreeNode* root, int level) {
    if (!root) return;

    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("NODE_TYPE: %s, VALUE: %s\n", NodeTypeToString(root->type), root->value.strValue ? root->value.strValue : "NULL");

    printBinaryTree(root->left, level + 1);
    printBinaryTree(root->right, level + 1);
}

const char* NodeTypeToString(NodeType type) {
    switch (type) {
        case NODE_OP: return "NODE_OP";
        case NODE_VAR: return "NODE_VAR";
        case NODE_CONST: return "NODE_CONST";
        case NODE_IF: return "NODE_IF";
        case NODE_WHILE: return "NODE_WHILE";
        case NODE_FUNC_DEF: return "NODE_FUNC_DEF";
        case NODE_FUNC_CALL: return "NODE_FUNC_CALL";
        case NODE_ASSIGN: return "NODE_ASSIGN";
        case NODE_RETURN: return "NODE_RETURN";
        case NODE_PROG: return "NODE_PROG";
        case NODE_VAR_DECL: return "NODE_VAR_DECL";
        case NODE_LINE: return "NODE_LINE";
        default: return "UNKNOWN_NODE_TYPE";
    }
}

const char* DataTypeToString(DataType type) {
    switch (type) {
        case TYPE_INT: return "TYPE_INT";
        case TYPE_FLOAT: return "TYPE_FLOAT";
        case TYPE_BOOL: return "TYPE_BOOL";
        case TYPE_STRING: return "TYPE_STRING";
        case TYPE_NEW_LINE: return "TYPE_NEW_LINE";
        default: return "UNKNOWN_DATA_TYPE";
    }
}