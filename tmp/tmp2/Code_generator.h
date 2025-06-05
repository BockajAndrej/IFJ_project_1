/**
 * @file Code_generator.h
 * @brief Header file for the implementation of code generation for the IFJ project.
 *
 * @details This file contains function declarations necessary for generating intermediate code (IFJcode24).
 * 
 * @author Pavel Glvač <xglvacp00>
 *
 * @note This file is part of the IFJ2024 project.
 */
#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include "ast.h"
#include "stack.h"
#include "lexical_analyser.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Generates the header for the IFJcode24 output.
 */
void generateHeader();

/**
 * @brief Declares a global variable.
 * 
 * @param node Pointer to the binary tree node representing the variable declaration.
 */
void generateGlobalVarDecl(BinaryTreeNode *node);

/**
 * @brief Declares a local variable.
 * 
 * @param node Pointer to the binary tree node representing the variable declaration.
 */
void generateLocalVarDecl(BinaryTreeNode *node);

/**
 * @brief Declares a global constant.
 * 
 * @param node Pointer to the binary tree node representing the constant declaration.
 */
void generateGlobalConstDecl(BinaryTreeNode *node);

/**
 * @brief Declares a local constant.
 * 
 * @param node Pointer to the binary tree node representing the constant declaration.
 */
void generateLocalConstDecl(BinaryTreeNode *node);

/**
 * @brief Generates built-in functions for the given node.
 * 
 * @param node Pointer to the binary tree node representing the function.
 */
void generateBuildInFuncions(BinaryTreeNode *node);

/**
 * @brief Generates an if statement.
 * 
 * @param node Pointer to the binary tree node representing the if statement.
 */
void generateIfStatement(BinaryTreeNode *node);

/**
 * @brief Generates a while statement.
 * 
 * @param node Pointer to the binary tree node representing the while statement.
 */
void generateWhileStatement(BinaryTreeNode *node);

/**
 * @brief Generates function parameters.
 * 
 * @param node Pointer to the binary tree node representing the function.
 */
void generateFunctionParams(BinaryTreeNode *node);

/**
 * @brief Generates the end of a function.
 * 
 * @param node Pointer to the binary tree node representing the function's return statement.
 */
void generateFunctionEnd(BinaryTreeNode *node);

/**
 * @brief Generates an expression.
 * 
 * @param node Pointer to the binary tree node representing the expression.
 * @return The name of the variable storing the result of the expression.
 */
const char* generateExpression(BinaryTreeNode *node);

/**
 * @brief Generates a function call.
 * 
 * @param node Pointer to the binary tree node representing the function call.
 */
void generateFunctionCall(BinaryTreeNode *node);

/**
 * @brief Generates the body of a function or statement block.
 * 
 * @param node Pointer to the binary tree node representing the body.
 */
void generateBody(BinaryTreeNode *node);

/**
 * @brief Processes the token type of a given binary tree node.
 * 
 * @param node Pointer to the binary tree node to be processed.
 */
void processTokenType(BinaryTreeNode *node);

#endif
