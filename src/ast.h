#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include "lexical_analyser.h"
#include "stack.h"

/// @brief Types of nodes
typedef enum
{
    NODE_GENERAL,    // Starting NODE
    NODE_PARAM,      // Node for parameters
    NODE_OP,         // Operations ( +, -, !=)
    NODE_VAR,        // Variables (a, b)
    NODE_CONST,      // Constants (0, 1)
    NODE_STRING,     // String value
    NODE_IF,         // condition (if-else)
    NODE_ELSIF,      // Else if
    NODE_ELSIF_PREP, // Else if auxiliary
    NODE_ELSE,       // Else
    NODE_PREP_IF,    // If auxiliary <npode_prep2_if   >N_prog
    NODE_PREP2_IF,   // If auxiliary condition left, nonnull value right
    NODE_NONNULL,    // value inside |example| in if or while
    NODE_WHILE,      // Loop
    NODE_WHILE_PREP,
    NODE_FUNC_DEF,  // Function definition
    NODE_FUNC_CALL, // function call
    NODE_ASSIGN,    // Assign (a = b)
    NODE_RETURN,    // Return value
    NODE_PROG,      // Root
    NODE_VAR_DECL,  // Variable declaration 
    NODE_LINE       // NewLine
} NodeType;

/// @brief Data types
typedef enum
{
    TYPE_INT,
    TYPE_INT_NULL,
    TYPE_FLOAT,
    TYPE_FLOAT_NULL,
    TYPE_STRING,
    TYPE_STRING_NULL,
    TYPE_BOOL,
    TYPE_VOID,
    TYPE_EMPTY,
    TYPE_NONNULL,
    TYPE_UNKNOWN,
    TYPE_FUNCTION,

    TYPE_U8_ARRAY // testing
} DataType;

/// @brief Structure of node of binary tree 
typedef struct BinaryTreeNode
{
    NodeType type;
    Token_type tokenType;
    char *strValue;
    bool isRight;
    struct BinaryTreeNode *left;   // Ľavý potomok
    struct BinaryTreeNode *right;  // Pravý potomok
    struct BinaryTreeNode *parent; // Rodič uzla
} BinaryTreeNode;

extern BinaryTreeNode *currentNode;
extern BinaryTreeNode *curInOrderNode;

// Functions for tree managing
BinaryTreeNode *createBinaryNode(NodeType type, Token_type tokenType, const char *value);
void insertLeft(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);
void insertLeftMoveLeft(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);
void insertLeftMoveRight(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);
void insertRight(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);
void insertRightMoveRight(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);
void insertRightMoveLeft(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);

// Function for move into tree
void setStartNode(BinaryTreeNode *root);
void setStartNodeInOrder(BinaryTreeNode *root);
void InOrder(BinaryTreeNode *node, Stack *stack);
bool moveUp(int levels);
void moveDownLeft();
void moveDownRight();

// Free
void freeBinaryTree(BinaryTreeNode *root);
// Print
void printBinaryTree(BinaryTreeNode *root);
void printBinaryTreeHelper(BinaryTreeNode *node, char *prefix, int isLast);
const char *NodeTypeToString(NodeType type);

const char *value_type_to_string(DataType type);
DataType value_string_to_type(const char *typeStr);
bool are_types_compatible(DataType actual, DataType expected);
#endif
