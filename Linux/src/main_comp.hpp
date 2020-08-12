#ifndef _MAIN_COMP_H_
#define _MAIN_COMP_H_
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include "compiler_comp.hpp"
#include "general_types.hpp"
#include "general_util.hpp"

using namespace std;

const int MAX_NUM_REGISTERS = 127;


class Compiler
{
  FrontEnd * fe;
  BackEnd * be;
  ASTNode * the_ast;
  map<string, bool> symbol_table;
public:
  Compiler() 
  { 
    fe = new FrontEnd(symbol_table);
    be = new BackEnd(symbol_table);
    the_ast = NULL;
  }
  ~Compiler() 
  {
    delete be; 
    delete fe;
    delete the_ast;
    
  }
  void parse(string file_name)
  {
    fe->run(file_name, the_ast);
    be->run(the_ast);
  }
  vector<AssemblyEntry*> getAssembly() { return be->getAssembly(); }
};


class Processor
{
private:
  map<string, int> registers;
  map<string, int> data_mem;
  map<string, int> jump_labels;
  map<string, int> global_vars;
  map<string, vector<int> > array_table;
  stack<int> return_addresses;
  queue<int> params;
  queue<vector<int> > params_array;
  vector<AssemblyEntry*> assembly;
  unsigned long long programCounter = 0;
  bool isGettingGlobal = true;
  void init();
  void readALUop(AssemblyEntry *);
  bool isNumber(string);
  bool isRegister(string);
  bool isDataMem(string);
  int getValue(string);
  void setValue(string, int);
  bool isGlobal(string);
  bool isArrayInst(string);
  bool isArrayVariable(string);
  bool isLoadValid(string);
  string printArray(vector<int>);
  string printString(vector<int>);
public:
  Processor(vector<AssemblyEntry*> newCode) : assembly(newCode) { }
  ~Processor() {assembly.clear();}
  void run();
};

#endif

