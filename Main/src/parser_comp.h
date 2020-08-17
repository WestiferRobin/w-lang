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
#include "compiler_comp.h"
#include "general_types.h"
#include "general_util.h"

using namespace std;

class StdInit
{
private:
  vector<tuple<string, string>> limit_sym;
  vector<AssemblyEntry*> stdInitAssembly;
  unsigned long long programCounter;
  void initStdConsts();
  void initStdFuncs();
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

  void start(ASTNode *&);
  void dependencies(ASTNode *&);
  ASTNode * main();

  ASTNode * function();
  ASTNode * functionCall();
  ASTNode * importFile(ASTNode *);
  bool isValidFunction();
  ASTNode * returnCall();
  
  ASTNode * copy();
  ASTNode * deleteStatement();
  ASTNode * length();
  ASTNode * equal();
  ASTNode * print();
  ASTNode * scan();

  ASTNode * assignment(void);
  ASTNode * assignment(bool);
  ASTNode * globalAssignment();
  void stmtList(ASTNode*);

  void caseStatemnt(ASTNode*&, ASTNode*&);
  void elseOrElseIfStatemnt(ASTNode*&, ASTNode*&);
  ASTNode * ifStatemnt();
  ASTNode * switchStatement();
  
  ASTNode * whileLoop();
  ASTNode * forLoop();

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
};

#endif