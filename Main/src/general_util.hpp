#ifndef _GENERAL_UTIL_H_
#define _GENERAL_UTIL_H_
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "general_types.hpp"

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
  static void cleanParameter(map<string, string>, ASTNode*&);
};

class ErrorReader
{
private:
public:
  ErrorReader() {}
  ~ErrorReader() {}
  static void readError(ErrorCode, string);
};

#endif