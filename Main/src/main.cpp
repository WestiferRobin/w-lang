#include <iostream>
#include <fstream>
#include "main_comp.hpp"

using namespace std;

const string debugFlag = "-d";

int start(int argc, char * argv[])
{
  Compiler * compiler;
  Processor * processor;

  if (argc <= 1)
  {
    cout << "Error: there is no valid arguments" << endl;
    return -1;
  }

  // get the file name and length of the string
  string file_name = argv[1];
  unsigned long length = file_name.length();
  bool isDisplayingAssembly = false;

  // DO VALIDATION FOR EXTENSION BEING CORRECT
  if (file_name[length-2] != '.' || file_name[length-1] != 'w')
  {
    cout << "Error: there is no matching extensions." << endl;
    return -1;
  }
  
  if (argc == 3)
  {
    isDisplayingAssembly = argv[2] == debugFlag;
  }

  // Do the compiler
  compiler = new Compiler();
  compiler->parse(file_name);
  
  vector<AssemblyEntry*> assembly(compiler->getAssembly());

  if (isDisplayingAssembly)
  {
    cout << "----- RUNNING ASSMEBLY -----" << endl;
    for (int index = 0; index < assembly.size(); index++)
    {
      cout << assembly[index]->toString() << endl;
    }
    cout << "----------------------------" << endl;
  }

  processor = new Processor(assembly);
  processor->run();

  assembly.clear();
  
  delete processor;
  delete compiler;

  return 0;
}

int main(int argc, char * argv[])
{
  return start(argc, argv);
}
