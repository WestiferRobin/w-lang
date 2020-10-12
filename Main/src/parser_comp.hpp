#ifndef _PARSERS_H_
#define _PARSERS_H_
#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <map>
#include <queue>
#include <set>
#include <vector>
#include <ctype.h>
#include "compiler_comp.hpp"
#include "general_types.hpp"
#include "general_util.hpp"

using namespace std;

class StdInit
{
private:
  vector<tuple<string, string>> limit_sym;
  vector<AssemblyEntry*> stdInitAssembly;
  unsigned long long programCounter;
  void initStdConsts();
  void initLimits();
  void initStd();
public:
  void addSymbolsToTable(map<string, bool>&);
  vector<AssemblyEntry*> getAssembly() {return stdInitAssembly;}
  unsigned long long getCounter() { return programCounter; }
  StdInit(unsigned long long currCount);
  StdInit();
  ~StdInit() { stdInitAssembly.erase(stdInitAssembly.begin(), stdInitAssembly.end());}
};


class Parser
{
private:
  TokenEntry * currToken;
  map<string, bool> symbol_table;
  map<string, bool> global_symbol_table;
  map<string, bool> arr_table;
  set<string> case_set;
  set<string> function_table;
  int rawStringCount = 0;
  int rawArrayCount = 0;

  // start.cpp
  void start(ASTNode *&); 
  void dependencies(ASTNode *&);
  void dependencies(ASTNode*&, ASTNode*&);
  ASTNode * main();

  // scope.cpp
  ASTNode * function();
  ASTNode * functionCall();
  ASTNode * importFile(ASTNode *);
  bool isValidFunction();
  ASTNode * returnCall();
  
  // system_op.cpp
  ASTNode * copy();
  ASTNode * deleteStatement();
  ASTNode * length();
  ASTNode * equal();
  ASTNode * print();
  ASTNode * scan();
  ASTNode * append();
  ASTNode * remove();
  ASTNode * random();

  // assign.cpp
  ASTNode * assignment(void);
  ASTNode * assignment(bool);
  ASTNode * globalAssignment();
  void stmtList(ASTNode*);

  // cond.cpp
  void elseOrElseIfStatemnt(ASTNode*&, ASTNode*&);
  ASTNode * ifStatemnt();
  
  // loop.cpp
  ASTNode * whileLoop();
  ASTNode * forLoop();

  // expression.cpp
  ASTNode * andOrCondition();
  ASTNode * arithExpression();
  ASTNode * arithExpressionP(ASTNode*);
  ASTNode * condition();
  ASTNode * expression();
  ASTNode * factor();
  ASTNode * term();
  ASTNode * termP(ASTNode*);

public:
	Parser(map<string, bool> targ_symbol_table) : symbol_table(targ_symbol_table) { currToken = NULL; }
	~Parser() { delete currToken; }
	void initGrammar(TokenEntry *, ASTNode *& );
	void initGrammar(TokenEntry*, ASTNode*&, ASTNode*&);
};

#endif
