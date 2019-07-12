#ifndef _COMPILER_INTER_H_
#define _COMPILER_INTER_H_
#include "compiler_comp.hpp"
#include <iostream>

class Compiler
{
  FrontEnd fe;
  BackEnd be;
public:
  Compiler() { }
  ~Compiler() { }
  void parse(string file_name)
  {
    fe.pre_parse(file_name); 
    fe.parse();
  }
};

#endif
