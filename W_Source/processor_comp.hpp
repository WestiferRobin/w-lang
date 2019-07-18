#ifndef _PROCESSOR_COMP_H_
#define _PROCESSOR_COMP_H_
#include <iostream>

using namespace std;

class Converter
{
public:
  static int decToHex(string);
};

class Interpreter
{
public:
  static bool isValidALUOp(string);
  static bool isValidJumpOp(string);
  static bool isValidAuxOp(string);
};


#endif
