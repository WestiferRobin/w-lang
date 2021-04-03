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
#include <bits/stdc++.h>
#include "general_types.hpp"
#include "general_util.hpp"
#include "parser_comp.hpp"

using namespace std;

class FrontEnd
{
private:
  string the_code;
  vector<string> keyword_list;
  vector<TokenEntry> tokens;
  map<string, bool> fe_symbol_table;

  void scanner(void);

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
  void run(string, ASTNode*&, ASTNode*&);
};


class BackEnd
{
private:
  bool isLast = false;
  bool isOneNumber = false;
  unsigned long long programCounter = 0;
  unsigned int regIndex = 0;
  Instance * whileInstance;
  Instance * forInstance;
  Instance * ifInstance;
  Instance * elifInstance;
  Instance * elseInstance;
  Instance * endInstance;
  Instance * caseInstance;
  Instance * defaultInstance;
  map<string, bool> be_symbol_table;
  vector<AssemblyEntry*> assembly;
  void createAssembly(ASTNode*&);
  void loadDependentLocals(ASTNode*&);
  void initStandardLib();
public:
  BackEnd(map<string, bool>& symbol_table) : be_symbol_table(symbol_table) 
  { 
    whileInstance = new StackInstance();
    forInstance = new StackInstance();
    ifInstance = new StackInstance();
    elseInstance = new StackInstance();
    endInstance = new StackInstance();
    defaultInstance = new StackInstance();
    caseInstance = new QueueInstance();
    elifInstance = new QueueInstance();
  }
  ~BackEnd() 
  { 
    assembly.erase(assembly.begin(), assembly.end()); 
    delete whileInstance;
    delete forInstance;
    delete ifInstance;
    delete elseInstance;
    delete endInstance;
    delete defaultInstance;
    delete caseInstance;
    delete elifInstance;
  }
  void run(ASTNode*&);
  vector<AssemblyEntry*> getAssembly() { return assembly; }
};

#endif
