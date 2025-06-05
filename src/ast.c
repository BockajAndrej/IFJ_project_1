/**
 * @file ast.c
 * @author Pavel Givac
 * @category Abstract syntax tree
 * @brief This file contains functions for Abstract syntax tree
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

// Copies a string to a newly allocated memory.
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

// Creates a new binary tree node with specified type, token, and value.
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
    node->strValue = copy_str(value); // Store string value
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

// Inserts a left child node for the given parent if no left child exists.
void insertLeft(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value)
{
    if (!parent)
    {
        fprintf(stderr, "Error: Parent node is NULL.\n");
        return;
    }
    if (parent->left)
    {
        fprintf(stderr, "Error: Left child already exists.\n");
        return;
    }
    BinaryTreeNode *child = createBinaryNode(type, tokenType, value);
    child->isRight = false;
    child->parent = parent;
    parent->left = child;
}

// Inserts a left child and moves current node left.
void insertLeftMoveLeft(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value)
{
    insertLeft(parent, type, tokenType, value);
    moveDownLeft();
}

// Inserts a left child and moves current node right.
void insertLeftMoveRight(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value)
{
    insertLeft(parent, type, tokenType, value);
    moveDownRight();
}

// Inserts a right child node for the given parent if no right child exists.
void insertRight(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value)
{
    if (!parent)
    {
        fprintf(stderr, "Error: Parent node is NULL.\n");
        return;
    }
    if (parent->right)
    {
        fprintf(stderr, "Error: Right child already exists.\n");
        return;
    }
    BinaryTreeNode *child = createBinaryNode(type, tokenType, value);
    child->isRight = true;
    child->parent = parent;
    parent->right = child;
}

// Inserts a right child and moves current node right.
void insertRightMoveRight(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value)
{
    insertRight(parent, type, tokenType, value);
    moveDownRight();
}

// Inserts a right child and moves current node left.
void insertRightMoveLeft(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value)
{
    insertRight(parent, type, tokenType, value);
    moveDownLeft();
}

// Global variables for tracking current and in-order traversal nodes.
BinaryTreeNode *currentNode = NULL;
BinaryTreeNode *curInOrderNode = NULL;

// Set the current node to the root.
void setStartNode(BinaryTreeNode *root)
{
    currentNode = root;
}

// Set the in-order node to the root.
void setStartNodeInOrder(BinaryTreeNode *root)
{
    curInOrderNode = root;
}

// Perform in-order traversal and push nodes to the stack.
void InOrder(BinaryTreeNode *node, Stack *stack)
{
    if (node != NULL)
    {
        InOrder(node->left, stack); // Traverse left subtree
        Stack_item tmp_item;
        tmp_item.type = rule;
        tmp_item.data.isPrec = false;
        tmp_item.data.token_val.valueString.str = node->strValue;
        tmp_item.data.token_type = node->tokenType;
        push(stack, tmp_item);       // Push current node to stack
        InOrder(node->right, stack); // Traverse right subtree
    }
}

// Move up the tree by the specified number of levels.
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
            handle_error(ERR_COMPILER_INTERNAL);
        }
    }
    // Return whether the current node is a right child
    return currentNode->isRight;
}

// Move to the left child of the current node.
void moveDownLeft()
{
    if (currentNode && currentNode->left)
    {
        currentNode = currentNode->left;
    }
    else
    {
        printf("Error: Cannot move left from the current node.\n");
        handle_error(ERR_COMPILER_INTERNAL);
    }
}

// Move to the right child of the current node.
void moveDownRight()
{
    if (currentNode && currentNode->right)
    {
        currentNode = currentNode->right;
    }
    else
    {
        printf("Error: Cannot move right from the current node. (%s)\n", currentNode->strValue);
        handle_error(ERR_COMPILER_INTERNAL);
    }
}

// Move left through the tree until a node with the specified token type is found.
BinaryTreeNode *move_left_until(BinaryTreeNode *node, Token_type dest)
{
    while (node != NULL)
    {
        if (node->tokenType == dest)
            return node;
        node = node->left;
    }
    return NULL;
}

// Move right through the tree until a node with the specified token type is found.
BinaryTreeNode *move_right_until(BinaryTreeNode *node, Token_type dest)
{
    while (node != NULL)
    {
        if (node->tokenType == dest)
            return node;
        node = node->right;
    }
    return NULL;
}

// Recursively free all nodes in the binary tree.
void freeBinaryTree(BinaryTreeNode *root)
{
    if (!root)
        return;

    freeBinaryTree(root->left);  // Free left subtree
    freeBinaryTree(root->right); // Free right subtree

    if (root->strValue != NULL)
    {
        free(root->strValue); // Free string value
    }
    free(root); // Free the node itself
}

// Free the entire tree starting from the root node, traversing up to find the root.
void freeTreeFromAnyNode(BinaryTreeNode *node)
{
    while (node->parent != NULL)
    {
        node = node->parent; // Traverse up to the root
    }
    freeBinaryTree(node); // Free the tree starting from the root
}

// Print the binary tree starting from the root.
void printBinaryTree(BinaryTreeNode *root)
{
    if (!root)
    {
        printf("The tree is empty.\n");
        return;
    }

    printBinaryTreeHelper(root, "", 1); // Helper function to print tree
}

// Recursively prints the binary tree with a visual structure, color-coding nodes.
void printBinaryTreeHelper(BinaryTreeNode *node, char *prefix, int isLast)
{
    if (node == NULL)
        return;

    // Define colors for different types of nodes
    const char *COLOR_RESET = "\033[0m";  // Reset color
    const char *COLOR_RED = "\033[31m";   // Red for left child
    const char *COLOR_GREEN = "\033[32m"; // Green for right child
    const char *COLOR_BLUE = "\033[34m";  // Blue for root

    // Set color based on node's position in the tree
    const char *color = COLOR_BLUE; // Default color for root
    if (node->parent)
    {
        if (node->parent->left == node)
            color = COLOR_RED; // Red for left child
        else if (node->parent->right == node)
            color = COLOR_GREEN; // Green for right child
    }

    // Print the current node with the appropriate prefix and color
    printf("%s", prefix);
    if (isLast)
        printf("└── ");
    else
        printf("├── ");
    printf("%s%s (\"%s\") %s %s\n", color, NodeTypeToString(node->type),
           node->strValue ? node->strValue : "NULL", token_type_to_string(node->tokenType),
           COLOR_RESET);

    // Prepare the prefix for child nodes
    char newPrefix[1024];
    strcpy(newPrefix, prefix);
    if (isLast)
        strcat(newPrefix, "    ");
    else
        strcat(newPrefix, "│   ");

    // Determine if the node has children and recursively print them
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
        printBinaryTreeHelper(node->right, newPrefix, 1); // Only right child

    else if (node->left)
        printBinaryTreeHelper(node->left, newPrefix, 1); // Only left child
}

// Convert NodeType enum to a human-readable string.
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

// Convert DataType enum to a string representation.
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
    case TYPE_NULL:
        return "null";
    case TYPE_NONNULL:
        return "nonNull";
    case TYPE_FUNCTION:
        return "function";
    default:
        return "unknown";
    }
}

// Convert string to corresponding DataType.
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
    if (strcmp(typeStr, "null") == 0)
        return TYPE_NULL;
    if (strcmp(typeStr, "string") == 0)
        return TYPE_STRING;
    if (strcmp(typeStr, "[]u8") == 0)
        return TYPE_U8_ARRAY;
    if (strcmp(typeStr, "void") == 0)
        return TYPE_VOID;
    if (strcmp(typeStr, "nonNull") == 0)
        return TYPE_NONNULL;
    if (strcmp(typeStr, "function") == 0)
        return TYPE_FUNCTION;
    return TYPE_UNKNOWN; // Default
}

// Determine the DataType of a string based on its value.
DataType find_return_datatype(char *value)
{
    int is_float = 0, is_string = 0;

    // Check if the value is "null"
    if (strcmp(value, "null") == 0)
        return TYPE_NULL;

    // Iterate through each character in the value string
    for (int i = 0; value[i] != '\0'; i++)
    {
        // Check if the character is a decimal point
        if (value[i] == '.')
        {
            // If already identified as float or string, mark as string and break
            if (is_float || is_string)
            {
                is_string = 1;
                break;
            }
            // Mark as float
            is_float = 1;
        }
        // If the character is not a digit, mark as string and break
        else if (!isdigit(value[i]))
        {
            is_string = 1;
            break;
        }
    }

    // Return the appropriate data type based on the flags
    if (is_string)
        return TYPE_STRING;
    if (is_float)
        return TYPE_FLOAT;
    return TYPE_INT; // Default to integer
}

// Check if two data types are compatible.
bool are_types_compatible(DataType actual, DataType expected)
{
    if (actual == expected)
        return true;
    if (expected == TYPE_INT && actual == TYPE_INT_NULL)
        return true;
    if (expected == TYPE_INT_NULL && (actual == TYPE_INT || actual == TYPE_NULL))
        return true;
    if (expected == TYPE_FLOAT && actual == TYPE_INT)
        return true;
    if (expected == TYPE_FLOAT_NULL && (actual == TYPE_INT || actual == TYPE_FLOAT || actual == TYPE_INT_NULL || actual == TYPE_NULL))
        return true;
    return false;
}
