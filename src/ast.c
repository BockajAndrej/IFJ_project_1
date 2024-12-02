#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

// Pomocná funkcia na kopírovanie reťazca
char *copy_str(const char *str)
{
    if (!str)
        return NULL;
    char *copy = malloc(strlen(str) + 1);
    if (!copy)
    {
        fprintf(stderr, "Error: Memory allocation failed for string copy.\n");
        exit(1);
    }
    strcpy(copy, str);
    return copy;
}

// VYTVORENIE UZLA
BinaryTreeNode *createBinaryNode(NodeType type, Token_type tokenType, const char *value)
{
    BinaryTreeNode *node = malloc(sizeof(BinaryTreeNode));
    if (!node)
    {
        fprintf(stderr, "Error: Memory allocation failed for BinaryTreeNode.\n");
        exit(1);
    }
    node->type = type;
    node->tokenType = tokenType;
    node->strValue = copy_str(value); // Uloženie hodnoty
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

// VLOZENIE V LAVO
void insertLeft(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value)
{
    if (!parent)
    {
        fprintf(stderr, "Error: Parent node is NULL.\n");
        return;
    }
    if (parent->left)
    {
        fprintf(stderr, "Error: Left child already exists for node %s.\n", parent->strValue);
        return;
    }
    BinaryTreeNode *child = createBinaryNode(type, tokenType, value);
    child->isRight = false;
    child->parent = parent;
    parent->left = child;
}

void insertLeftMoveLeft(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value)
{
    insertLeft(parent, type, tokenType, value);
    moveDownLeft();
}

void insertLeftMoveRight(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value)
{
    insertLeft(parent, type, tokenType, value);
    moveDownRight();
}

// VLOZENIA V PRAVO
void insertRight(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value)
{
    if (!parent)
    {
        fprintf(stderr, "Error: Parent node is NULL.\n");
        return;
    }
    if (parent->right)
    {
        fprintf(stderr, "Error: Right child already exists for node %s.\n", parent->strValue);
        return;
    }
    BinaryTreeNode *child = createBinaryNode(type, tokenType, value);
    child->isRight = true;
    child->parent = parent;
    parent->right = child;
}

void insertRightMoveRight(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value)
{
    insertRight(parent, type, tokenType, value);
    moveDownRight();
}

void insertRightMoveLeft(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value)
{
    insertRight(parent, type, tokenType, value);
    moveDownLeft();
}

// PREMENNA NA SLEDOVANIE UZLA
BinaryTreeNode *currentNode = NULL;

// POHYB
void setStartNode(BinaryTreeNode *root)
{
    currentNode = root;
}

bool moveUp(int levels)
{
    int movedLevels = 0;
    while (levels > 0)
    {
        if (currentNode && currentNode->parent)
        {
            currentNode = currentNode->parent;
            movedLevels++;
            levels--;
        }
        else
        {
            printf("Error: Cannot move up %d levels. Moved up %d level(s). Actual node: %s\n", levels + movedLevels, movedLevels, currentNode->strValue);
            exit(EXIT_FAILURE);
        }
    }
    return currentNode->isRight;
}

void moveDownLeft()
{
    if (currentNode && currentNode->left)
    {
        currentNode = currentNode->left;
    }
    else
    {
        printf("Error: Cannot move left from the current node.\n");
        exit(EXIT_FAILURE);
    }
}

void moveDownRight()
{
    if (currentNode && currentNode->right)
    {
        currentNode = currentNode->right;
    }
    else
    {
        printf("Error: Cannot move right from the current node. (%s)\n", currentNode->strValue);
        exit(EXIT_FAILURE);
    }
}

void freeBinaryTree(BinaryTreeNode *root)
{
    if (!root)
        return;

    freeBinaryTree(root->left);
    freeBinaryTree(root->right);

    if (root->strValue != NULL)
    {
        free(root->strValue);
    }
    free(root);
}

void printBinaryTree(BinaryTreeNode *root)
{
    if (!root)
    {
        printf("The tree is empty.\n");
        return;
    }

    // Start the helper with an empty prefix and mark root as the last node
    printBinaryTreeHelper(root, "", 1);
}

void printBinaryTreeHelper(BinaryTreeNode *node, char *prefix, int isLast)
{
    if (node == NULL)
        return;

    const char *COLOR_RESET = "\033[0m";  // Reset to default
    const char *COLOR_RED = "\033[31m";   // Red for left child
    const char *COLOR_GREEN = "\033[32m"; // Green for right child
    const char *COLOR_BLUE = "\033[34m";  // Blue for root

    const char *color = COLOR_BLUE; // Default color for root
    if (node->parent)
    {
        if (node->parent->left == node)
            color = COLOR_RED; // Red for left child
        else if (node->parent->right == node)
            color = COLOR_GREEN; // Green for right child
    }

    printf("%s", prefix);
    if (isLast)
        printf("└── ");
    else
        printf("├── ");

    printf("%s%s (\"%s\") %s %s\n", color, NodeTypeToString(node->type),
           node->strValue ? node->strValue : "NULL", token_type_to_string(node->tokenType),
           COLOR_RESET);

    char newPrefix[1024];
    strcpy(newPrefix, prefix);
    if (isLast)
        strcat(newPrefix, "    ");
    else
        strcat(newPrefix, "│   ");

    int children = 0;
    if (node->left)
        children++;
    if (node->right)
        children++;

    if (children == 0)
        return;

    if (node->left && node->right)
    {
        printBinaryTreeHelper(node->right, newPrefix, 0); // Not the last child
        printBinaryTreeHelper(node->left, newPrefix, 1);  // Last child
    }
    else if (node->right)
        printBinaryTreeHelper(node->right, newPrefix, 1);

    else if (node->left)
        printBinaryTreeHelper(node->left, newPrefix, 1);
}

const char *NodeTypeToString(NodeType type)
{
    switch (type)
    {
    case NODE_GENERAL:
        return "NODE_GENERAL";
    case NODE_OP:
        return "NODE_OP";
    case NODE_VAR:
        return "NODE_VAR";
    case NODE_CONST:
        return "NODE_CONST";
    case NODE_STRING:
        return "NODE_STRING";
    case NODE_IF:
        return "NODE_IF";
    case NODE_WHILE:
        return "NODE_WHILE";
    case NODE_WHILE_PREP:
        return "NODE_WHILE_PREP";
    case NODE_FUNC_DEF:
        return "NODE_FUNC_DEF";
    case NODE_FUNC_CALL:
        return "NODE_FUNC_CALL";
    case NODE_ASSIGN:
        return "NODE_ASSIGN";
    case NODE_RETURN:
        return "NODE_RETURN";
    case NODE_PROG:
        return "NODE_PROG";
    case NODE_VAR_DECL:
        return "NODE_VAR_DECL";
    case NODE_LINE:
        return "NODE_LINE";
    case NODE_PREP_IF:
        return "NODE_PREP_IF";
    case NODE_ELSIF_PREP:
        return "NODE_ELSIF_PREP";
    case NODE_ELSIF:
        return "NODE_ELSIF";
    case NODE_ELSE:
        return "NODE_ELSE";
    case NODE_PREP2_IF:
        return "NODE_PREP2_IF";
    case NODE_NONNULL:
        return "NODE_NONNULL";
    default:
        return "UNKNOWN_NODE_TYPE";
    }
}

const char *value_type_to_string(DataType type)
{
    switch (type)
    {
    case TYPE_INT:
        return "i32";
    case TYPE_FLOAT:
        return "f64";
    case TYPE_INT_NULL:
        return "?i32";
    case TYPE_FLOAT_NULL:
        return "?f64";
    case TYPE_STRING:
        return "string";
    case TYPE_BOOL:
        return "bool";
    case TYPE_VOID:
        return "void";
    case TYPE_U8_ARRAY:
        return "[]u8";
    case TYPE_EMPTY:
        return "empty";
    case TYPE_NONNULL:
        return "nonNull";
    case TYPE_FUNCTION:
        return "function";
    default:
        return "unknown";
    }
}

DataType value_string_to_type(const char *typeStr)
{
    if (strcmp(typeStr, "i32") == 0)
        return TYPE_INT;
    if (strcmp(typeStr, "f64") == 0)
        return TYPE_FLOAT;
    if (strcmp(typeStr, "?i32") == 0)
        return TYPE_INT_NULL;
    if (strcmp(typeStr, "?f64") == 0)
        return TYPE_FLOAT_NULL;
    if (strcmp(typeStr, "bool") == 0)
        return TYPE_BOOL;
    if (strcmp(typeStr, "string") == 0)
        return TYPE_STRING;
    if (strcmp(typeStr, "[]u8") == 0)
        return TYPE_U8_ARRAY;
    if (strcmp(typeStr, "void") == 0)
        return TYPE_VOID;
    if (strcmp(typeStr, "nonNull") == 0)
        return TYPE_VOID;
    if (strcmp(typeStr, "function") == 0)
        return TYPE_FUNCTION;
    // Add more types as needed
    return TYPE_UNKNOWN;
}

bool are_types_compatible(DataType actual, DataType expected)
{
    // printf("actual = %s > expected = %s\n", value_type_to_string(actual), value_type_to_string(expected));
    if (actual == expected)
        return true;
    if (expected == TYPE_INT && actual == TYPE_INT_NULL)
        return true;
    if (expected == TYPE_INT_NULL && (actual == TYPE_INT))
        return true;
    if (expected == TYPE_FLOAT && actual == TYPE_INT)
        return true;
    if (expected == TYPE_FLOAT_NULL && (actual == TYPE_INT || actual == TYPE_FLOAT || actual == TYPE_INT_NULL))
        return true;

    return false;
}