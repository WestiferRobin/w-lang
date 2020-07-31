#ifndef _AST_TYPES_H_
#define _AST_TYPES_H_
#include <iostream>

enum ASTType
{
   MODULUS,
   NOT,
   ADD,
   MINUS,
   BIT_AND,
   BOOL_AND,
   BOOL_OR,
   BIT_OR,
   BIT_XOR,
   BIT_LEFT,
   BIT_RIGHT,
   DIV,
   MULTI,

   NUM,
   VAR,
   CHAR,
   KEYWORD,

   EQUALS,
   NOT_EQUALS,
   LESS_THAN,
   GREATER_THAN,
   LESS_THAN_EQ,
   GREATER_THAN_EQ,

   ASSIGN,
   ASSIGN_NONE,
   ASSIGN_ADD,
   ASSIGN_MINUS,
   ASSIGN_AND,
   ASSIGN_OR,
   ASSIGN_DIV,
   ASSIGN_MULTI,
   ASSIGN_XOR,
   ASSIGN_SHIFT_LEFT,
   ASSIGN_SHIFT_RIGHT,
   ASSIGN_MODULUS,
   
   ARRAY_EXCH,
   ARRAY_INIT,
   ARRAY_INIT_SIZE,
   ARRAY_INIT_PRE_ELM,
   ARRAY_INIT_ELM,
   ARRAY_ACCESS,
   ARRAY_WHOLE,
   
   COND_CMPR,
   IF_STMT,
   IF_COND,
   ELIF_STMT,
   ELIF_COND,
   ELSE_STMT,
   ELSE_COND,

   WHILE_LOOP,
   WHILE_LOOP_COND,
   WHILE_LOOP_COND_P,
   WHILE_LOOP_STATEMENT,
   WHILE_LOOP_EXIT,

   FOR_LOOP,
   FOR_LOOP_COND,
   FOR_LOOP_COND_P,
   FOR_LOOP_STATEMENT,
   FOR_LOOP_EXIT,

   SWITCH_COND,
   SWITCH_CMPR,
   SWITCH_END,
   CASE_COND,
   CASE_STMT,
   DEFAULT_STMT,
   DEFAULT_COND,

   START,
   MAIN,
   STMTLIST,
   DEPENDENCY_FUNC,
   DEPENDENCY_GLOBAL,
   DEPENDENCY,
   IMPORT,
   
   FUNCTION,
   FUNCTION_CALL,
   PARAM_CALL,
   PARAM_INST,
   PARAM_ARRAY_CALL,
   PARAM_ARRAY_INST,
   RETURN,
   RETURN_ARR,

   PRINT,
   SCAN,
   COPY,
   LENGTH,
   EQUAL_FUNC,
   DELETE,

   UNKNOWN
};


class ASTNode
{
public:
   ASTType type;
   int value;
   string key;
   ASTNode * left;
   ASTNode * right;
 
   ASTNode()
   {
      type = UNKNOWN;
      value = INT_MIN;
      right = NULL;
      left = NULL;
      key = "";
   }
 
   ~ASTNode()
   {
      delete left;
      delete right;
      key.clear();
      
   }
};


#endif