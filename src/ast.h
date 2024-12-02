#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include "lexical_analyser.h"
#include "stack.h"

// Typy uzlov
typedef enum
{
    NODE_GENERAL, // Starting NODE
    NODE_PARAM,
    NODE_OP,         // Operácia ( +, -, !=)
    NODE_VAR,        // Premenná (a, b)
    NODE_CONST,      // Konštanta (0, 1)
    NODE_STRING,     // String hodnota
    NODE_IF,         // Podmienka (if-else)
    NODE_ELSIF,      // Else if
    NODE_ELSIF_PREP, // Else if pomocná
    NODE_ELSE,       // Else
    NODE_PREP_IF,    // If pomocná <npode_prep2_if   >N_prog
    NODE_PREP2_IF,   // If pomocna na podmienku vlavo, nonnull hodnotu vpravo
    NODE_NONNULL,    // value inside |example| in if or while
    NODE_WHILE,      // Cyklus
    NODE_WHILE_PREP,
    NODE_FUNC_DEF,  // Definícia funkcie
    NODE_FUNC_CALL, // Volanie funkcie
    NODE_ASSIGN,    // Priradenie (a = b)
    NODE_RETURN,    // Návratová hodnota
    NODE_PROG,      // Koreň programu
    NODE_VAR_DECL,  // Deklarácia premennej
    NODE_LINE       // Nový riadok
} NodeType;

// Typy dát
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

// Štruktúra uzla binárneho stromu
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

// FUNKCIE PRE SPRAVU STROMU
BinaryTreeNode *createBinaryNode(NodeType type, Token_type tokenType, const char *value);
void insertLeft(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);
void insertLeftMoveLeft(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);
void insertLeftMoveRight(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);
void insertRight(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);
void insertRightMoveRight(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);
void insertRightMoveLeft(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);

// FUNKCIE PRE POHYB V STOME
void setStartNode(BinaryTreeNode *root);
void setStartNodeInOrder(BinaryTreeNode *root);
void InOrder(BinaryTreeNode *node, Stack *stack);
bool moveUp(int levels);
void moveDownLeft();
void moveDownRight();

// FREE A VYPIS
void freeBinaryTree(BinaryTreeNode *root);
void printBinaryTree(BinaryTreeNode *root);
void printBinaryTreeHelper(BinaryTreeNode *node, char *prefix, int isLast);
const char *NodeTypeToString(NodeType type);

const char *value_type_to_string(DataType type);
DataType value_string_to_type(const char *typeStr);
bool are_types_compatible(DataType actual, DataType expected);
#endif
