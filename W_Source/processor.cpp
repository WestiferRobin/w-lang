#include "processor.hpp"

void Processor::orgAssembly(string * raw)
{
  unsigned int count = 0;
  while (raw->substr(0, 2) != "00")
  {
    Entry entry(*raw); 
    assembly.insert(pair<unsigned int, Entry>(count, entry));
    raw++;
    count++;
  }
  Entry entry("00000000", HALT);
  assembly.insert(pair<unsigned int, Entry>(count, entry));
}

void Processor::init()
{
  string hexChar = "0123456789ABCDEF";
  for (int i = 0; i < hexChar.length(); i++)
  {
    for (int j = 0; j < hexChar.length(); j++)
    {
      string key = string() + hexChar[i] + hexChar[j];
      registers.insert(pair<string, short> (key, 0));
    }
  }
}

void Processor::run()
{
  init();
  unsigned int i = 0;
  while (i < UINT_MAX)
  {
    try
    {
      Entry * instance = &assembly.at(i);
      switch (instance->getType())
      {
        case HALT:
          cout << "End of Program." << endl;
          /*for (auto i = registers.begin(); i != registers.end(); i++)
          {
            cout << i->first << " = " << i->second << endl;
          }*/
          return;
        case REGISTER:
          if (Interpreter::isValidALUOp(instance->getOppCode()))
          {
            cout << "ITS AN REGISTER ALU OP" << endl;
            Interpreter::calculate(&registers, instance);
          }
          else if (Interpreter::isValidAuxOp(instance->getOppCode()))
          {
            cout << "ITS AN REGISTER AUX OP" << endl;
            Interpreter::auxExec(&registers, instance);
          }
          else
          {
            throw 2;
          }
          break;
        case IMMEDIATE:
          if (Interpreter::isValidJumpOp(instance->getOppCode()))
          {
            cout << "ITS AN IMMEDIATE JUMP" << endl;
          }
          else
          {
            throw 3;
          }
          break;
        case MEMORY:
          cout << "Memory type" << endl;
          break;
        case HYBRID:
          if (Interpreter::isValidALUOp(instance->getOppCode()))
          {
            cout << "ITS a Hybrid ALU OP" << endl;
            Interpreter::calculate(&registers, instance);
          }
          else if (Interpreter::isValidAuxOp(instance->getOppCode()))
          {
            cout << "ITS a Hybrid AUX OP" << endl;
            Interpreter::auxExec(&registers, instance);
          }
          else
          {
            throw 4;
          }
          break;
        default:
          throw 1;
          return;
      }
      i++;
    }
    catch (int e)
    {
      cout << "Error Code: " << e << endl;
    }
    
  }
}

