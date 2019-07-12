#ifndef _COMPILER_COMP_H_
#define _COMPILER_COMP_H_
#include <iostream>
#include <string>
#include <fstream>
#include <stack>

using namespace std;

class FrontEnd
{
  string straight_file;
public:
  FrontEnd() { }
  ~FrontEnd() { }
  void pre_parse(string);
  void parse(void);
};

class BackEnd
{
public:
  BackEnd() { }
  ~BackEnd() { }
};

// This will might be needed but not primary. Maybe after OOP is applied
class Optimizer
{
public:
  Optimizer() {}
  ~Optimizer() {}
};


#endif
