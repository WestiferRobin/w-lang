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
  void Start(ASTNode *&);
  void Dependencies(ASTNode *&);
  ASTNode * Main();

  // private functions
  // ast_utils.cpp
  void validToken(TokenType,string);
  ASTNode * createASTNode(ASTType,ASTNode*,ASTNode*);
  ASTNode * createASTNumberNode(TokenEntry);
  ASTNode * createASTVariableNode(TokenEntry);
  ASTNode * createASTArrayNode(TokenEntry);
  ASTNode * createASTNullNode();
  ASTNode * createASTCharNode(TokenEntry);
  ASTNode * createASTWholeArrayNode(TokenEntry);

  ASTNode * ImportFile(ASTNode *);
  ASTNode * ReturnCall();
  ASTNode * Function();
  ASTNode * FunctionCall();
  bool IsValidFunction();
  ASTNode * Print();
  ASTNode * Scan();
  ASTNode * Copy();
  ASTNode * Length();
  ASTNode * Equal();
  ASTNode * DeleteStatement();

  // assign.cpp
  void StmtList(ASTNode*);
  ASTNode * Assignment(void);
  ASTNode * Assignment(bool);
  ASTNode * GlobalAssignment();

  // cond.cpp
  ASTNode * IfStatemnt();
  void ElseOrElseIfStatemnt(ASTNode*&, ASTNode*&);
  ASTNode * SwitchStatement();
  void CaseStatemnt(ASTNode*&, ASTNode*&);
  
  // loops.cpp
  ASTNode * WhileLoop();
  ASTNode * ForLoop();

  // ast_math.cpp
  ASTNode * Expression();
  ASTNode * AndOrCondition();
  ASTNode * Condition();
  ASTNode * ArithExpression();
  ASTNode * ArithExpressionP(ASTNode*);
  ASTNode * Term();
  ASTNode * TermP(ASTNode*);
  ASTNode * Factor();

public:
  void initGrammar(TokenEntry *, ASTNode *& );
  LLParser(map<string, bool> targ_symbol_table) : symbol_table(targ_symbol_table) {}
  ~LLParser() {delete currToken;}
};

class FrontEnd
{
private:
  string the_code;
  vector<string> keyword_list;
  vector<TokenEntry> tokens;
  map<string, bool> fe_symbol_table;

  void scanner(void);

  // TODO: place in different file fe_handle_utils.cpp
  bool handleVariablesAndKeywords(int *, string *);
  void handleNumbersAndVariables(int *, string *);
  void handleCharactersAndStrings(int *, string *);
  void handleOperatorsTypeOne(int *, string *);
  void handleOperatorsTypeTwo(int *, string *, bool *);
  void handleOperatorsTypeThree(int *, string *);
  void handleOperatorsTypeFour(int *, string *);
public:
  FrontEnd(map<string, bool> symbol_table);
  ~FrontEnd() { }
  void run(string, ASTNode *&);
};

class BackEnd
{
private:
  bool isLast = false;
  bool isOneNumber = false;
  unsigned long long programCounter = 0;
  unsigned int regIndex = 0;
  // TODO: Make this into a vector of some struct
    unsigned int whileLoopInstance = 0;
    stack<unsigned int> whileInstances;
    unsigned int forLoopInstance = 0;
    stack<unsigned int> forInstances;
    unsigned int ifStatmentInstance = 0;
    stack<unsigned int> ifInstances;
    unsigned int elifStatementInstance = 0;
    queue<unsigned int> elifInstances;
    unsigned int elseStatementInstance = 0;
    stack<unsigned int> elseInstances;
    unsigned int endStatementInstance = 0;
    stack<unsigned int> endInstances;
    unsigned int caseStatementInstance = 0;
    queue<unsigned int> caseInstance;
    unsigned int defaultStatementInstance = 0;
    stack<unsigned int> defaultInstance;
  ///////////////////////////////////////////////////
  map<string, bool> be_symbol_table;
  vector<AssemblyEntry*> assembly;
  void createAssembly(ASTNode*&);
  void loadDependentLocals(ASTNode*&);
  void initStandardLib();
public:
  BackEnd(map<string, bool> symbol_table) : be_symbol_table(symbol_table) { }
  ~BackEnd() { assembly.erase(assembly.begin(), assembly.end()); }
  void run(ASTNode*&);
  vector<AssemblyEntry*> getAssembly() { return assembly; }
};

#endif
