#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_
#include "../general_types.hpp"
#include "../general_util.hpp"
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <stack>
#include <queue>

using namespace std;

const int MAX_NUM_REGISTERS = 127;

class Processor
{
private:
  map<string, int> registers;
  map<string, int> data_mem;
  map<string, int> jump_labels;
  map<string, int> global_vars;
  stack<int> return_addresses;
  queue<int> params;
  vector<AssemblyEntry> assembly;
  unsigned long long programCounter = 0;
  bool isGettingGlobal = true;
  void init();
  void readALUop(AssemblyEntry);
  bool isNumber(string);
  bool isRegister(string);
  bool isDataMem(string);
  int getValue(string);
  void setValue(string, int);
  bool isGlobal(string);
public:
  Processor(vector<AssemblyEntry> newCode) : assembly(newCode) { }
  ~Processor() {}
  void run();
};

#endif
