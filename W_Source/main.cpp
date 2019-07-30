#include <iostream>
#include <fstream>
#include <map>
#include "compiler.hpp"
#include "processor.hpp"

using namespace std;

// global variables for the application
Compiler * compiler;
Processor * processor;

int start(int argc, char * argv[])
{
  if (argc <= 1)
  {
    cout << "Error: there is no valid arguments" << endl;
    return -1;
  }

  // get the file name and length of the string
  string file_name = argv[1];
  unsigned long length = file_name.length();

  // DO VALIDATION FOR EXTENSION BEING CORRECT
  if (file_name[length-2] != '.' || file_name[length-1] != 'w')
  {
    cout << "Error: there is no matching extensions." << endl;
    return -1;
  }

  /*
  TODO: Have compiler working for the following parts individually completed.
  1. expression
  2. assign
  3. if statements
  4. while loops
  5. for loops
  6. functions
  7. classes
  */
  compiler = new Compiler();
  compiler->parse(file_name);

  /*
  TODO: 
  1. GET YOUR SET ARCH DOWN!
  2. Create Excpetion reader
  3. Add line counter for jumps system
    ex.) L1: load r1, 234
             add r1, 1
             print r1
  4. Add memory system for loading and storing
  */
  string assembly[] = {
    "11010004",
    "11020002",
    "02010200",
    "13010001",
    "20010000",
    "10010000",
    "00000000"
  };
  processor = new Processor(assembly);
  processor->run();

  delete compiler;
  delete processor;

  return 0;
}

int main(int argc, char * argv[])
{
  return start(argc, argv);
}
