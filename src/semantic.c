/**
 * @file semantic.c
 * @author Jakub Filo
 * @category Semantic analysis
 * @brief This file contains functions for semantic analysis, processing binary tree, checking everything.
 */

#include "semantic.h"

void ast_test()
{
    // 1. Vytvor koreňový uzol
    // BinaryTreeNode *root = createBinaryNode(NODE_OP, TYPE_INT, "+");
    // setStartNode(root); // Nastavenie koreňa ako počiatočného uzla

    // // 2. Pridaj ľavý podstrom
    // insertLeft(root, NODE_OP, TYPE_INT, "*");
    // moveDownLeft();

    // // 3. Pridaj deti k ľavému podstromu
    // insertLeft(currentNode, NODE_CONST, TYPE_INT, "2");
    // insertRight(currentNode, NODE_CONST, TYPE_INT, "3");

    // // 4. Návrat na koreň
    // moveUp(1);

    // // 5. Pridaj pravý podstrom
    // insertRight(root, NODE_OP, TYPE_INT, "*");
    // moveDownRight();

    // // 6. Pridaj deti k pravému podstromu
    // insertLeft(currentNode, NODE_CONST, TYPE_INT, "4");
    // insertRight(currentNode, NODE_CONST, TYPE_INT, "5");

    // // 7. Výpis celej štruktúry stromu
    // printf("\nComplete Tree Structure:\n");
    // printBinaryTree(root);

    // // 8. Návrat do ľavého podstromu
    // moveUp(1);      // Späť na koreň
    // moveDownLeft(); // Do ľavého podstromu

    // // 9. Získanie a výpis informácií o uzle "*"
    // NodeInfo info = getNodeInfo(currentNode);
    // printf("\nInfo o uzle ľavého podstromu (semantická analýza):\n");
    // printf("  Type: %s\n", NodeTypeToString(info.type));
    // printf("  Value: %s\n", info.value);
    // printf("  Data Type: %s\n", DataTypeToString(info.dataType));
    // printf("  Has Left: %s\n", info.hasLeft ? "Yes" : "No");
    // printf("  Has Right: %s\n", info.hasRight ? "Yes" : "No");
    // printf("  Parent Value: %s\n", info.parentValue);

    // // 10. Semantická analýza - overenie uzla
    // if (info.type == NODE_CONST && info.dataType == TYPE_INT)
    // {
    //     printf("  Uzol je platná konštanta s celým číslom.\n");
    // }
    // else
    // {
    //     printf("  Chyba: Uzol nie je platná konštanta alebo nemá správny dátový typ.\n");
    // }

    // 11. Uvoľnenie stromu
    // freeBinaryTree(root);
    // printf("\nTree successfully freed.\n");
}

void ast_valdef()
{
    // i32 a = 3;
    BinaryTreeNode *root = createBinaryNode(NODE_VAR_DECL, TOKEN_KEYWORD, "i32");
    setStartNode(root); // Nastavenie koreňa ako počiatočného uzla
    insertLeft(root, NODE_VAR, TOKEN_IDENTIFIER, "a");
    insertRightMoveRight(root, NODE_ASSIGN, TOKEN_ASSIGNMENT, "=");
    insertLeft(currentNode, NODE_CONST, TOKEN_INT_LITERAL, "3");

    {
        printf("\nComplete Tree Structure:\n");
        printBinaryTree(root);
        freeBinaryTree(root);
        printf("\nTree successfully freed.\n");
    }
}

void ast_val_expression()
{
    // i32 a = 3 + C;
    BinaryTreeNode *root = createBinaryNode(NODE_VAR_DECL, TOKEN_KEYWORD, "i32");
    setStartNode(root);
    insertLeft(root, NODE_VAR, TOKEN_IDENTIFIER, "a");
    insertRightMoveRight(root, NODE_ASSIGN, TOKEN_ASSIGNMENT, "=");
    insertRightMoveRight(currentNode, NODE_OP, TOKEN_ADDITION, "+");
    insertLeft(currentNode, NODE_CONST, TOKEN_INT_LITERAL, "3");
    insertRight(currentNode, NODE_VAR, TOKEN_IDENTIFIER, "c");

    {
        printf("\nComplete Tree Structure:\n");
        printBinaryTree(root);
        freeBinaryTree(root);
        printf("\nTree successfully freed.\n");
    }
}


void ast_while_1()
{
    /*
        while (c != NULL) |INP|
        {
            c = 3;
        }
    */
    BinaryTreeNode *root = createBinaryNode(NODE_WHILE, TOKEN_KEYWORD, "while");
    setStartNode(root);
    insertRight(root, NODE_PROG, TOKEN_EMPTY, "");
    insertLeftMoveLeft(root, NODE_WHILE_PREP, TOKEN_EMPTY, "");
    insertRight(currentNode, NODE_NONNULL, TOKEN_IDENTIFIER, "INP");
    insertLeftMoveLeft(currentNode, NODE_OP, TOKEN_NOT_EQUAL, "!=");
    insertLeft(currentNode, NODE_VAR, TOKEN_IDENTIFIER, "c");
    insertRight(currentNode, NODE_CONST, TOKEN_EMPTY, "NULL"); //! NULL ako token?
    moveUp(2);
    moveDownRight();
    insertRightMoveRight(currentNode, NODE_ASSIGN, TOKEN_ASSIGNMENT, "=");
    insertLeft(currentNode, NODE_VAR, TOKEN_IDENTIFIER, "c");
    insertRight(currentNode, NODE_CONST, TOKEN_INT_LITERAL, "3");

    {
        printf("\nComplete Tree Structure:\n");
        printBinaryTree(root);
        freeBinaryTree(root);
        printf("\nTree successfully freed.\n");
    }
}

void ast_IfElse_1()
{

    // if (a < 0) |val|
    //     i32 a = 5;
    // else if (a == 0)
    //     f64 b = 9;
    // else
    //     []u8 c = "nieco";
    BinaryTreeNode *root = createBinaryNode(NODE_IF, TOKEN_KEYWORD, "if");
    setStartNode(root);
    insertRightMoveRight(currentNode, NODE_PREP_IF, TOKEN_EMPTY, "");
    insertLeftMoveLeft(currentNode, NODE_PREP2_IF, TOKEN_EMPTY, "");
    insertRight(currentNode, NODE_NONNULL, TOKEN_IDENTIFIER, "val");
    insertLeftMoveLeft(currentNode, NODE_OP, TOKEN_LESS_THAN, "<");
    insertLeft(currentNode, NODE_VAR, TOKEN_IDENTIFIER, "a");
    insertRight(currentNode, NODE_CONST, TOKEN_INT_LITERAL, "0");
    moveUp(2);
    insertRightMoveRight(currentNode, NODE_PROG, TOKEN_EMPTY, "");
    insertRightMoveRight(currentNode, NODE_VAR_DECL, TOKEN_KEYWORD, "i32");
    insertLeft(currentNode, NODE_VAR, TOKEN_IDENTIFIER, "a");
    insertRightMoveRight(currentNode, NODE_ASSIGN, TOKEN_ASSIGNMENT, "");
    insertRight(currentNode, NODE_CONST, TOKEN_INT_LITERAL, "3");
    moveUp(4);
    insertLeftMoveLeft(currentNode, NODE_ELSIF, TOKEN_EMPTY, "");
    insertRightMoveRight(currentNode, NODE_ELSIF_PREP, TOKEN_EMPTY, "");
    insertLeftMoveLeft(currentNode, NODE_OP, TOKEN_EQUAL, "==");
    insertLeft(currentNode, NODE_VAR, TOKEN_IDENTIFIER, "a");
    insertRight(currentNode, NODE_CONST, TOKEN_INT_LITERAL, "0");
    moveUp(1);
    insertRightMoveRight(currentNode, NODE_PROG, TOKEN_EMPTY, "");
    insertRightMoveRight(currentNode, NODE_VAR_DECL, TOKEN_KEYWORD, "f64");
    insertLeft(currentNode, NODE_VAR, TOKEN_IDENTIFIER, "b");
    insertRightMoveRight(currentNode, NODE_ASSIGN, TOKEN_ASSIGNMENT, "=");
    insertRight(currentNode, NODE_CONST, TOKEN_FLOAT_LITERAL, "9.6");
    moveUp(4);
    insertLeftMoveLeft(currentNode, NODE_ELSE, TOKEN_EMPTY, "");
    insertRightMoveRight(currentNode, NODE_PROG, TOKEN_EMPTY, "");
    insertRightMoveRight(currentNode, NODE_VAR_DECL, TOKEN_KEYWORD, "[]u8");
    insertLeft(currentNode, NODE_VAR, TOKEN_IDENTIFIER, "c");
    insertRightMoveRight(currentNode, NODE_ASSIGN, TOKEN_ASSIGNMENT, "=");
    insertRight(currentNode, NODE_CONST, TOKEN_STRING_LITERAL, "nieco");

    {
        printf("\nComplete Tree Structure:\n");
        printBinaryTree(root);
        freeBinaryTree(root);
        printf("\nTree successfully freed.\n");
    }
}
