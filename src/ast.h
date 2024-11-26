#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include "lexical_analyser.h"

// Typy uzlov
typedef enum
{
    NODE_GENERAL,    // Starting NODE
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
    TYPE_NEW_LINE,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_BOOL,
    TYPE_STRING,
    TYPE_VOID,
    TYPE_EMPTY,
    TYPE_NONNULL,
    TYPE_UNKNOWN,

    TYPE_U8_ARRAY //testing
} DataType;

typedef enum
{
    AST_VALUE_INT,
    AST_VALUE_FLOAT,
    AST_VALUE_STRING
} ASTValueType;

// Štruktúra uzla binárneho stromu
typedef struct BinaryTreeNode
{
    NodeType type;
    // DataType dataType;
    Token_type tokenType;
    // ASTValueType valueType; // Typ hodnoty
    char *strValue;
    bool isRight;
    struct BinaryTreeNode *left;   // Ľavý potomok
    struct BinaryTreeNode *right;  // Pravý potomok
    struct BinaryTreeNode *parent; // Rodič uzla
} BinaryTreeNode;

typedef struct
{
    NodeType type;
    Token_type tokenTypeL;
    const char *value;
    bool hasLeft;
    bool hasRight;
    const char *parentValue;
} NodeInfo;

extern BinaryTreeNode *currentNode;

// FUNKCIE PRE SPRAVU STROMU
BinaryTreeNode *createBinaryNode(NodeType type, Token_type tokenType, const char *value);
void insertLeft(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);
void insertLeftMoveLeft(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);
void insertLeftMoveRight(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);
void insertRight(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);
void insertRightMoveRight(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);
void insertRightMoveLeft(BinaryTreeNode *parent, NodeType type, Token_type tokenType, const char *value);

// FUNKCIE PRE POHYB V STOME
NodeInfo getCurrentNodeInfo();
void setStartNode(BinaryTreeNode *root);
bool moveUp(int levels);
void moveDownLeft();
void moveDownRight();
NodeInfo getNodeInfo(BinaryTreeNode *node);

// FREE A VYPIS
void freeBinaryTree(BinaryTreeNode *root);
void printBinaryTree(BinaryTreeNode *root);
void printBinaryTreeHelper(BinaryTreeNode *node, char *prefix, int isLast);
const char *NodeTypeToString(NodeType type);
const char *DataTypeToString(DataType type);

#endif
