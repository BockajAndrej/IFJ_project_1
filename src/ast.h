#ifndef AST_H
#define AST_H

#include <stdbool.h>

// Typy uzlov
typedef enum
{
    NODE_OP,    // Operácia ( +, -, !=)
    NODE_VAR,   // Premenná (a, b)
    NODE_CONST, // Konštanta (0, 1)
    NODE_IF,    // Podmienka (if-else)
    NODE_ELSIF, // Else if
    NODE_ELSIF_PREP,
    NODE_ELSE,
    NODE_PREP_IF,   // priprava na podmienku a telo
    NODE_PREP2_IF,
    NODE_NONNULL,   // value inside |example| in if or while
    NODE_WHILE,     // Cyklus
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
    TYPE_EMPTY,
    TYPE_NONNULL
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
    DataType dataType;
    ASTValueType valueType; // Typ hodnoty
    union
    {
        int intValue;
        float floatValue;
        char *strValue;
    } value;
    struct BinaryTreeNode *left;   // Ľavý potomok
    struct BinaryTreeNode *right;  // Pravý potomok
    struct BinaryTreeNode *parent; // Rodič uzla
} BinaryTreeNode;

typedef struct
{
    NodeType type;
    DataType dataType;
    const char *value;
    bool hasLeft;
    bool hasRight;
    const char *parentValue;
} NodeInfo;

extern BinaryTreeNode *currentNode;

// FUNKCIE PRE SPRAVU STROMU
BinaryTreeNode *createBinaryNode(NodeType type, DataType dataType, const char *value);
void insertLeft(BinaryTreeNode *parent, NodeType type, DataType dataType, const char *value);
void insertLeftMoveLeft(BinaryTreeNode *parent, NodeType type, DataType dataType, const char *value);
void insertLeftMoveRight(BinaryTreeNode *parent, NodeType type, DataType dataType, const char *value);
void insertRight(BinaryTreeNode *parent, NodeType type, DataType dataType, const char *value);
void insertRightMoveRight(BinaryTreeNode *parent, NodeType type, DataType dataType, const char *value);
void insertRightMoveLeft(BinaryTreeNode *parent, NodeType type, DataType dataType, const char *value);

// FUNKCIE PRE POHYB V STOME
NodeInfo getCurrentNodeInfo();
void setStartNode(BinaryTreeNode *root);
void moveUp(int levels);
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
