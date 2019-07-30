#ifndef _PROCESSOR_COMP_H_
#define _PROCESSOR_COMP_H_
#include <iostream>
#include <map>
#include "processor_types.hpp"

using namespace std;

class Converter
{
public:
  static int hexToDec(string);
};

class Interpreter
{
public:
  static bool isValidALUOp(string);
  static bool isValidJumpOp(string);
  static bool isValidAuxOp(string);
  static void calculate(map<string, short> *, Entry *);
  static void auxExec(map<string, short> *, Entry *);
};



#endif
