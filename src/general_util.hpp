#ifndef _GENERAL_UTIL_H_
#define _GENERAL_UTIL_H_
#include <iostream>

using namespace std;

const int ERROR_INVALID_BOOL_OP = 0;
const int ERROR_INVALID_SYMBOL = 1;
const int ERROR_INVALID_ARITH_OP = 2;
const int ERROR_INVALID_KEYWORD = 3;
const int ERROR_VAR_UNKNOWN = 4;
const int ERROR_INVALID_LOAD = 5;
const int ERROR_INVALID_OP_CODE = 6;

class ErrorReader
{
private:
public:
  ErrorReader() {}
  ~ErrorReader() {}
  static void readError(int, string);
};

#endif