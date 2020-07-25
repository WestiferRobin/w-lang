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
