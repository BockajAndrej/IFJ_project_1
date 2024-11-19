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
    BinaryTreeNode *root = createBinaryNode(NODE_OP, TYPE_INT, "+");
    setStartNode(root); // Nastavenie koreňa ako počiatočného uzla

    // 2. Pridaj ľavý podstrom
    insertLeft(root, NODE_OP, TYPE_INT, "*");
    moveDownLeft();

    // 3. Pridaj deti k ľavému podstromu
    insertLeft(currentNode, NODE_CONST, TYPE_INT, "2");
    insertRight(currentNode, NODE_CONST, TYPE_INT, "3");

    // 4. Návrat na koreň
    moveUp(1);

    // 5. Pridaj pravý podstrom
    insertRight(root, NODE_OP, TYPE_INT, "*");
    moveDownRight();

    // 6. Pridaj deti k pravému podstromu
    insertLeft(currentNode, NODE_CONST, TYPE_INT, "4");
    insertRight(currentNode, NODE_CONST, TYPE_INT, "5");

    // 7. Výpis celej štruktúry stromu
    printf("\nComplete Tree Structure:\n");
    printBinaryTree(root);

    // 8. Návrat do ľavého podstromu
    moveUp(1);      // Späť na koreň
    moveDownLeft(); // Do ľavého podstromu

    // 9. Získanie a výpis informácií o uzle "*"
    NodeInfo info = getNodeInfo(currentNode);
    printf("\nInfo o uzle ľavého podstromu (semantická analýza):\n");
    printf("  Type: %s\n", NodeTypeToString(info.type));
    printf("  Value: %s\n", info.value);
    printf("  Data Type: %s\n", DataTypeToString(info.dataType));
    printf("  Has Left: %s\n", info.hasLeft ? "Yes" : "No");
    printf("  Has Right: %s\n", info.hasRight ? "Yes" : "No");
    printf("  Parent Value: %s\n", info.parentValue);

    // 10. Semantická analýza - overenie uzla
    if (info.type == NODE_CONST && info.dataType == TYPE_INT)
    {
        printf("  Uzol je platná konštanta s celým číslom.\n");
    }
    else
    {
        printf("  Chyba: Uzol nie je platná konštanta alebo nemá správny dátový typ.\n");
    }

    // 11. Uvoľnenie stromu
    freeBinaryTree(root);
    printf("\nTree successfully freed.\n");
}

void ast_valdef()
{
    // int a = 3;
    BinaryTreeNode *root = createBinaryNode(NODE_VAR_DECL, TYPE_INT, "a");
    setStartNode(root); // Nastavenie koreňa ako počiatočného uzla
    insertRightMoveRight(root, NODE_ASSIGN, TYPE_EMPTY, "");
    insertLeft(currentNode, NODE_CONST, TYPE_INT, "3");

    {
        printf("\nComplete Tree Structure:\n");
        printBinaryTree(root);
        freeBinaryTree(root);
        printf("\nTree successfully freed.\n");
    }
}

void ast_val_expression()
{
    // int a = 3 + C;
    BinaryTreeNode *root = createBinaryNode(NODE_VAR_DECL, TYPE_INT, "a");
    setStartNode(root);
    insertRightMoveRight(root, NODE_ASSIGN, TYPE_EMPTY, "");
    insertRightMoveRight(currentNode, NODE_OP, TYPE_EMPTY, "+");
    insertLeft(currentNode, NODE_CONST, TYPE_INT, "3");
    insertRight(currentNode, NODE_VAR, TYPE_INT, "c");

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
    //     int a = 5;
    // else if (a == 0)
    //     float b = 9;
    // else
    //     bool c = false;
    BinaryTreeNode *root = createBinaryNode(NODE_IF, TYPE_EMPTY, "");
    setStartNode(root);
    insertRightMoveRight(root, NODE_PREP_IF, TYPE_EMPTY, "");
    insertLeftMoveLeft(currentNode, NODE_PREP2_IF, TYPE_EMPTY, "");
    insertRight(currentNode, NODE_NONNULL, TYPE_NONNULL, "val");
    insertLeftMoveLeft(currentNode, NODE_OP, TYPE_EMPTY, "<");
    insertLeft(currentNode, NODE_VAR, TYPE_INT, "a");    // TYPE_INT pravdepodobne bocky urci alebo budem vediet z hashtable
    insertRight(currentNode, NODE_CONST, TYPE_INT, "0"); // spravit si funkciu na urcenie datatype?
    moveUp(2);
    insertRightMoveRight(currentNode, NODE_PROG, TYPE_EMPTY, "");
    insertRightMoveRight(currentNode, NODE_VAR_DECL, TYPE_INT, "a");
    insertRightMoveRight(currentNode, NODE_ASSIGN, TYPE_EMPTY, "");
    insertRight(currentNode, NODE_CONST, TYPE_INT, "3");
    moveUp(4);
    insertLeftMoveLeft(currentNode, NODE_ELSIF, TYPE_EMPTY, "");
    insertRightMoveRight(currentNode, NODE_ELSIF_PREP, TYPE_EMPTY, "");
    insertLeftMoveLeft(currentNode, NODE_OP, TYPE_EMPTY, "==");
    insertLeft(currentNode, NODE_VAR, TYPE_INT, "a");
    insertRight(currentNode, NODE_CONST, TYPE_INT, "0");
    moveUp(1);
    insertRightMoveRight(currentNode, NODE_PROG, TYPE_EMPTY, "");
    insertRightMoveRight(currentNode, NODE_VAR_DECL, TYPE_FLOAT, "b");
    insertRight(currentNode, NODE_ASSIGN, TYPE_EMPTY, "9");
    moveUp(4);
    moveDownLeft();
    insertLeftMoveLeft(currentNode, NODE_ELSE, TYPE_EMPTY, "");
    insertRightMoveRight(currentNode, NODE_PROG, TYPE_EMPTY, "");
    insertRightMoveRight(currentNode, NODE_VAR_DECL, TYPE_BOOL, "c");
    insertRightMoveRight(currentNode, NODE_ASSIGN, TYPE_EMPTY, "");
    insertRight(currentNode, NODE_CONST, TYPE_BOOL, "false");

    {
        printf("\nComplete Tree Structure:\n");
        printBinaryTree(root);
        freeBinaryTree(root);
        printf("\nTree successfully freed.\n");
    }
}

void check_logic()
{
}