/**
 * @file semantic.h
 * @author Jakub Filo
 * @category Semantic analysis
 * @brief This file contains functions for semantic analysis, processing binary tree, checking everything.
 */
#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "symtable.h"
#include "stdio.h"
#include "lexical_analyser.h"
#include "error.h"

DataType value_string_to_type(const char *typeStr);
// const char *token_type_to_string(Token_type tokenType);
const char *value_type_to_string(DataType type);
bool are_types_compatible(DataType actual, DataType expected);

BinaryTreeNode *move_left_until(BinaryTreeNode *node, Token_type dest);
BinaryTreeNode *move_right_until(BinaryTreeNode *node, Token_type dest);

// function
void process_func_def(BinaryTreeNode *funcDefNode, SymbolStack *stack);
Symbol *parse_parameters(BinaryTreeNode *paramsListNode);
DataType process_func_return(BinaryTreeNode *returnNode, SymbolStack *stack);
DataType process_validate_func_call(BinaryTreeNode *funcnode, SymbolStack *stack);
void process_voidFunc(BinaryTreeNode *node, SymbolStack *stack);

// expressions
DataType process_expression(BinaryTreeNode *returnNode, SymbolStack *stack);
// if
void process_if(BinaryTreeNode *ConditionNode, SymbolStack *stack);
// while
void process_while(BinaryTreeNode *Whilenode, SymbolStack *stack);

void process_var_declaration(BinaryTreeNode *node, SymbolStack *stack);
void process_identifier_assign(BinaryTreeNode *node, SymbolStack *stack);
BinaryTreeNode *ProcessTree(BinaryTreeNode *root, SymbolStack *stack);


#endif