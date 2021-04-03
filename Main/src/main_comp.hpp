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
  map<string, vector<int>> array_table;
  stack<int> return_addresses;
  queue<tuple<string, int>> params;
  stack<map<string, int>> prev_state;
  queue<tuple<string, vector<int>>> params_array;
  map<string, string> params_convert;
  int params_convert_count = 0;
  stack<map<string, vector<int>>> prev_state_array;
  vector<AssemblyEntry*> assembly;
  unsigned long long programCounter = 0;
  bool isGettingGlobal = true;
  void init();
  void readALUop(AssemblyEntry *);
  bool isNumber(string);
  bool isRegister(string);
  bool isDataMem(string);
  int getValue(string);
  void setValue(string, unsigned long );
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

class Start
{
  const string debugFlag = "-d";
public:
  Start() {}
  int run(int argc, char * argv[])
  {
    Compiler * compiler;
    Processor * processor;

    if (argc <= 1)
    {
      cout << "Error: there is no valid arguments" << endl;
      return -1;
    }

    // get the file name and length of the string
    string file_name = argv[1];
    unsigned long length = file_name.length();
    bool isDisplayingAssembly = false;

    // DO VALIDATION FOR EXTENSION BEING CORRECT
    if (file_name[length-2] != '.' || file_name[length-1] != 'w')
    {
      cout << "Error: there is no matching extensions." << endl;
      return -1;
    }
    
    if (argc == 3)
    {
      isDisplayingAssembly = argv[2] == debugFlag;
    }

    // Do the compiler
    compiler = new Compiler();
    compiler->parse(file_name);
    
    vector<AssemblyEntry*> assembly(compiler->getAssembly());

    if (isDisplayingAssembly)
    {
      cout << "----- RUNNING ASSMEBLY -----" << endl;
      for (int index = 0; index < assembly.size(); index++)
      {
        cout << assembly[index]->toString() << endl;
      }
      cout << "----------------------------" << endl;
    }

    processor = new Processor(assembly);
    processor->run();

    assembly.clear();
    
    delete processor;
    delete compiler;

    return 0;
  }
};

#endif

