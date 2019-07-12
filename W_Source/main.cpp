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
  TODO: GET YOUR SET ARCH DOWN!
  */
  string assembly[] = {
    "11010001",
    "11020001",
    "01010200",
    "1F000000",
    "20010000",
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
