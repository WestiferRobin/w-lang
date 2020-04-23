#ifndef _COMPILER_INTER_H_
#define _COMPILER_INTER_H_
#include "compiler_comp.hpp"
#include "../general_types.hpp"
#include <iostream>

class Compiler
{
  FrontEnd * fe;
  BackEnd * be;
public:
  Compiler() 
  { 
    fe = new FrontEnd(); 
    be = new BackEnd();
  }
  ~Compiler() { delete fe; delete be;}
  void parse(string file_name)
  {
    fe->run(file_name); 
    be->addSymbolTable(fe->getSymbolTable());
    be->run(fe->getAST());
  }
  vector<AssemblyEntry*> getAssembly() { return be->getAssembly(); }
};

#endif
