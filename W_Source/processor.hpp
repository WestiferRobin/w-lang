#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_
#include "processor_comp.hpp"
#include "processor_types.hpp"
#include <iostream>
#include <map>

using namespace std;

class Processor
{
private:
  map<unsigned int, Entry> assembly;
  map<unsigned int, unsigned short> regg;
  void orgAssembly(string[]);
  EntryType getType(string);
public:
  Processor(string newCode[]) { orgAssembly(newCode); }
  ~Processor() {}
  void run();
};

#endif
