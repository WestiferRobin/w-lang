#ifndef _COMPILER_COMP_H_
#define _COMPILER_COMP_H_
#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <map>
#include <queue>
#include <set>
#include <vector>
#include <ctype.h>
#include "token_types.hpp"
#include "ast_types.hpp"
#include "../general_types.hpp"
#include "../general_util.hpp"

using namespace std;

class StdInit
{
private:
  vector<AssemblyEntry> stdInitAssembly;
  unsigned long long programCounter;
  void initStdConsts();
  void initStdFuncs();
public:
  vector<AssemblyEntry> getAssembly() {return stdInitAssembly;}
  unsigned long long getCounter() { return programCounter; }
  StdInit(unsigned long long currCount) : programCounter(currCount) 
  { 
    initStdConsts();
    initStdFuncs();
  }
  ~StdInit() { }
};

//TODO: divide up the SHIT TON of private functions
class LLParser
{
private:
  // private datamember
  TokenEntry * currToken;
  map<string, bool> symbol_table;
  map<string, bool> global_symbol_table;
  set<string> arr_table;
  set<string> case_set;
  set<string> function_table;

  // private functions
  void validToken(TokenType,string);
  ASTNode * createASTNode(ASTType,ASTNode*,ASTNode*);
  ASTNode * createASTNumberNode(TokenEntry);
  ASTNode * createASTVariableNode(TokenEntry);
  ASTNode * createASTArrayNode(TokenEntry);
  ASTNode * createASTNullNode();
  ASTNode * createASTCharNode(TokenEntry);
  ASTNode * createASTWholeArrayNode(TokenEntry);
  
  ASTNode * Start();
  void Dependencies(ASTNode *);
  ASTNode * Main();

  ASTNode * ImportFile();
  ASTNode * ReturnCall();
  ASTNode * Function();
  ASTNode * FunctionCall();
  bool IsValidFunction();
  ASTNode * Print();
  ASTNode * DeleteStatement();

  void StmtList(ASTNode*);
  ASTNode * Assignment(void);
  ASTNode * Assignment(bool);
  ASTNode * GlobalAssignment();

  ASTNode * IfStatemnt();
  void ElseOrElseIfStatemnt(ASTNode*&, ASTNode*&);
  
  ASTNode * WhileLoop();
  ASTNode * ForLoop();

  ASTNode * Expression();
  ASTNode * AndOrCondition();
  ASTNode * Condition();
  ASTNode * ArithExpression();
  ASTNode * ArithExpressionP(ASTNode*);
  ASTNode * Term();
  ASTNode * TermP(ASTNode*);
  ASTNode * Factor();

  ASTNode * SwitchStatement();
  void CaseStatemnt(ASTNode*&, ASTNode*&);

public:
  ASTNode * initGrammar(TokenEntry * );
  map<string, bool> getSymbolTable() { return symbol_table; } 
  LLParser() {}
  ~LLParser() {}
};

class FrontEnd
{
private:
  // private data members
  ASTNode* the_ast;
  LLParser parser;
  string the_code;
  vector<string> keyword_list;
  vector<TokenEntry> tokens;

  // private functions 
  void assignVariables();
  void scanner(void);
  void parse(void);
public:
  FrontEnd() { FrontEnd::assignVariables(); }
  ~FrontEnd() { }
  void run(string);
  ASTNode* getAST() { return the_ast; }
  map<string, bool> getSymbolTable() { return parser.getSymbolTable();}
};

class BackEnd
{
private:
  bool isLast = false;
  bool isOneNumber = false;
  unsigned long long programCounter = 0;
  unsigned int regIndex = 0;
  unsigned int whileLoopInstance = 0;
  unsigned int forLoopInstance = 0;
  unsigned int ifStatmentInstance = 0;
  unsigned int elifStatementInstance = 0;
  unsigned int elseStatementInstance = 0;
  unsigned int endStatementInstance = 0;
  unsigned int caseStatementInstance = 0;
  unsigned int defaultStatementInstance = 0;
  stack<unsigned int> ifInstances;
  queue<unsigned int> elifInstances;
  stack<unsigned int> endInstances;
  stack<unsigned int> elseInstances;
  stack<unsigned int> whileInstances;
  stack<unsigned int> forInstances;
  queue<unsigned int> caseInstance;
  stack<unsigned int> defaultInstance;
  map<string, bool> symbol_table;
  vector<AssemblyEntry> assembly;
  void createAssembly(ASTNode*);
  void loadDependentLocals(ASTNode*);
  void initStandardLib();
public:
  BackEnd() { }
  ~BackEnd() { }
  void addSymbolTable(map<string, bool> tmp) { symbol_table = tmp;}
  void run(ASTNode*);
  vector<AssemblyEntry> getAssembly() { return assembly; }
};

#endif
