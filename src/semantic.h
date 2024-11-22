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
#include "lexical_analyser.h"

void ast_test();
void ast_IfElse_1();
void ast_valdef();
void ast_val_expression();
void check_logic();
void ast_while_1();

DataType value_string_to_type(const char *typeStr);
//const char *token_type_to_string(Token_type tokenType);
const char *value_type_to_string(DataType type);
bool are_types_compatible(DataType actual, DataType expected);
void process_var_declaration(BinaryTreeNode *node);
void process_const_declaration(BinaryTreeNode *node);
void ProcessTree(BinaryTreeNode *root);

#endif