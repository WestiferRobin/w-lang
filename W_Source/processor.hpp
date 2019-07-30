#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_
#include "processor_comp.hpp"
#include "processor_types.hpp"
#include <iostream>
#include <map>
#include <string>

using namespace std;

class Processor
{
private:
  map<unsigned int, Entry> assembly;
  map<string, short> registers;
  map<string, short> data_mem;
  void orgAssembly(string[]);
  void init();
public:
  Processor(string newCode[]) { orgAssembly(newCode); }
  ~Processor() {}
  void run();
};

#endif
