/**
 * @file ast.h
 * @author Pavel Givac
 * @category Abstract syntax tree
 * @brief This file contains functions for Abstract syntax tree
 */

#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include "lexical_analyser.h"
#include "stack.h"

/**
 * @brief Enumeration of different node types in the AST.
 * These represent various constructs such as operations, variables, constants, conditionals, and more.
 */
typedef enum
{
    NODE_GENERAL,    // Starting node of the AST
    NODE_PARAM,      // Parameter node
    NODE_OP,         // Operation node (e.g., +, -, !=)
    NODE_VAR,        // Variable node
    NODE_CONST,      // Constant node (e.g., 0, 1)
    NODE_STRING,     // String value node
    NODE_IF,         // If conditional node
    NODE_ELSIF,      // Else-if node
    NODE_ELSIF_PREP, // Auxiliary node for else-if
    NODE_ELSE,       // Else node
    NODE_PREP_IF,    // Auxiliary node for 'if'
    NODE_PREP2_IF,   // Auxiliary node for 'if' with non-null right condition
    NODE_NONNULL,    // Non-null value node (used in conditions)
    NODE_WHILE,      // While loop node
    NODE_WHILE_PREP, // Auxiliary node for while loop
    NODE_FUNC_DEF,   // Function definition node
    NODE_FUNC_CALL,  // Function call node
    NODE_ASSIGN,     // Assignment (e.g., a = b)
    NODE_RETURN,     // Return statement node
    NODE_PROG,       // Root program node
    NODE_VAR_DECL,   // Variable declaration node
    NODE_LINE        // Line break node
} NodeType;

/**
 * @brief Enumeration of possible data types used in the AST.
 * Represents different types such as integer, float, string, function, etc.
 */
typedef enum
{
    TYPE_INT,         // Integer type
    TYPE_INT_NULL,    // Nullable integer type
    TYPE_FLOAT,       // Float type
    TYPE_FLOAT_NULL,  // Nullable float type
    TYPE_STRING,      // String type
    TYPE_STRING_NULL, // Nullable string type
    TYPE_BOOL,        // Boolean type
    TYPE_VOID,        // Void type (no return value)
    TYPE_EMPTY,       // Empty type (possibly for uninitialized values)
    TYPE_NONNULL,     // Non-nullable type
    TYPE_NULL,        // Null type
    TYPE_UNKNOWN,     // Unknown type (for unrecognized types)
    TYPE_FUNCTION,    // Function type
    TYPE_U8_ARRAY     // 8-bit unsigned integer array (for testing)
} DataType;

/**
 * @brief Structure representing a binary tree node.
 * Each node contains a type, a token type, a string value, child nodes, and a parent node.
 */
typedef struct BinaryTreeNode
{
    NodeType type;        // Type of the node (e.g., variable, operation)
    Token_type tokenType; // Token type from the lexer
    char *strValue;       // String representation of the value
    bool isRight;         // Boolean flag for right child node
    struct BinaryTreeNode *left;
    struct BinaryTreeNode *right;
    struct BinaryTreeNode *parent;
} BinaryTreeNode;

// External variables for the current and in-order nodes
extern BinaryTreeNode *currentNode;
extern BinaryTreeNode *curInOrderNode;

// Function declarations for binary tree manipulation
BinaryTreeNode *createBinaryNode(NodeType type, Token_type tokenType, const char *value);
void insertLeft(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);
void insertLeftMoveLeft(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);
void insertLeftMoveRight(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);
void insertRight(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);
void insertRightMoveRight(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);
void insertRightMoveLeft(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);

// Functions for tree traversal and manipulation
void setStartNode(BinaryTreeNode *root);
void setStartNodeInOrder(BinaryTreeNode *root);
void InOrder(BinaryTreeNode *node, Stack *stack);
bool moveUp(int levels);
void moveDownLeft();
BinaryTreeNode *move_left_until(BinaryTreeNode *node, Token_type dest);
BinaryTreeNode *move_right_until(BinaryTreeNode *node, Token_type dest);
void moveDownRight();

// Functions for memory management
void freeBinaryTree(BinaryTreeNode *root);
void freeTreeFromAnyNode(BinaryTreeNode *node);

// Functions for printing and displaying the tree
void printBinaryTree(BinaryTreeNode *root);
void printBinaryTreeHelper(BinaryTreeNode *node, char *prefix, int isLast);

// Helper functions for converting node types and checking type compatibility
const char *NodeTypeToString(NodeType type);
const char *value_type_to_string(DataType type);
DataType value_string_to_type(const char *typeStr);
bool are_types_compatible(DataType actual, DataType expected);
DataType find_return_datatype(char *name);

#endif
