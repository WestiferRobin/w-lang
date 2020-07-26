#ifndef _COMPILER_H_
#define _COMPILER_H_
#include "compiler_comp.hpp"
#include "general_types.hpp"
#include <iostream>
#include <map>

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
    cout << "WORKS BE" << endl;
    delete fe;
    cout << "WORKS FE" << endl;
  }
  void parse(string file_name)
  {
    fe->run(file_name, the_ast);
    be->run(the_ast);
  }
  vector<AssemblyEntry*> getAssembly() { return be->getAssembly(); }
};

#endif
