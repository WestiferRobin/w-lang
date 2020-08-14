#ifndef _GENERAL_UTIL_H_
#define _GENERAL_UTIL_H_
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "general_types.h"

using namespace std;

class ASTUtility
{
public:
  ASTUtility() {}
  ~ASTUtility() {}
  static ASTNode * createASTNode(ASTType,ASTNode*,ASTNode*);
  static ASTNode * createASTNumberNode(TokenEntry);
  static ASTNode * createASTVariableNode(TokenEntry);
  static ASTNode * createASTArrayNode(TokenEntry);
  static ASTNode * createASTNullNode();
  static ASTNode * createASTCharNode(TokenEntry);
  static ASTNode * createASTWholeArrayNode(TokenEntry);
  static ASTNode * cloneASTNode(ASTNode * );
  static void validToken(TokenType, string, TokenEntry *&);
};

// FUCKING REFACTOR THIS SHIT OUT OF THIS BULLSHIT!!!!?!?!?!?!?!
const int ERROR_INVALID_BOOL_OP = 0;
const int ERROR_INVALID_SYMBOL = 1;
const int ERROR_INVALID_ARITH_OP = 2;
const int ERROR_INVALID_KEYWORD = 3;
const int ERROR_VAR_UNKNOWN = 4;
const int ERROR_INVALID_LOAD = 5;
const int ERROR_INVALID_OP_CODE = 6;

class ErrorReader
{
private:
public:
  ErrorReader() {}
  ~ErrorReader() {}
  static void readError(int, string);
};

// CHECK HOW THIS WOULD WORK
class FlagHandler
{
private:
public:
  FlagHandler() {}
  ~FlagHandler() {}
};

#endif