#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H


#include "ast.h"
#include "lexical_analyser.h"


//-------------------HEADER----------------------
void generateHeader();


//-------------------VAR DEC----------------------
void generateGlobalVarDecl(BinaryTreeNode *node);
void generateLocalVarDecl(BinaryTreeNode *node);


//-------------------CONST DEC----------------------
void generateGlobalConstDecl(BinaryTreeNode *node);
void generateLocalConstDecl(BinaryTreeNode *node);


//-------------------BODY DEC----------------------
void generateBody(BinaryTreeNode *node);


//-------------------ASSIMENT/FUNCALL/EXPRESIONS----------------------
void generateAssignment(BinaryTreeNode *node);
const char* generateFunctionCall(BinaryTreeNode *node);
const char* generateExpression(BinaryTreeNode *node);

//-------------------IF DEC----------------------
void generateIfStatement(BinaryTreeNode *node);

//-------------------WHILE DEC----------------------
void generateWhileStatement(BinaryTreeNode *node);
void generateWhileBody(BinaryTreeNode *node);

//-------------------FUNC DEC----------------------
void generateFunctionParams(BinaryTreeNode *node);
void generateFunctionEnd();


//-------------------PROCES----------------------
void processTokenType(BinaryTreeNode *node);


#endif 