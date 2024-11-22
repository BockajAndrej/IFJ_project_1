#ifndef SEMANTIC_H
#define SEMANTIC_H
/**
 * @file semantic.h
 * @author Jakub Filo
 * @category Semantic analysis
 * @brief This file contains functions for semantic analysis, processing binary tree, checking everything.
 */

#include "ast.h"
#include "symtable.h"
#include "stdio.h"

void ast_test();
void ast_IfElse_1();
void ast_valdef();
void ast_val_expression();
void check_logic();
void ast_while_1();

#endif