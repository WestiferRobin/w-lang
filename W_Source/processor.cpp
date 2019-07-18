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

void Processor::run()
{
  unsigned int i = 0;
  while (i < UINT_MAX)
  {
    try
    {
      switch (assembly.at(i).getType())
      {
        case HALT:
          cout << "End of Program." << endl;
          return;
        case REGISTER:
          cout << "R type " << Converter::decToHex(assembly.at(i).getRegister()) << " " << Converter::decToHex(assembly.at(i).getRemaining()) << endl;
          if (Interpreter::isValidALUOp(assembly.at(i).getOppCode()))
          {
            cout << "ITS AN REGISTER ALU OP" << endl;
          }
          else if (Interpreter::isValidAuxOp(assembly.at(i).getOppCode()))
          {
            cout << "ITS AN REGISTER AUX OP" << endl;
          }
          else
          {
            //throw 1;
          }
          break;
        case IMMEDIATE:
          cout << "I type " << Converter::decToHex(assembly.at(i).getRegister()) << " " << Converter::decToHex(assembly.at(i).getRemaining()) << endl;
          if (Interpreter::isValidJumpOp(assembly.at(i).getOppCode()))
          {
            cout << "ITS AN IMMEDIATE JUMP" << endl;
          }
          else
          {
            //throw 1;
          }
          break;
        case MEMORY:
          cout << "Memory type" << endl;
          break;
        case HYBRID:
          cout << "HYBRID type " << endl;
          if (Interpreter::isValidALUOp(assembly.at(i).getOppCode()))
          {
            cout << "ITS AN Hybrid ALU OP" << endl;
          }
          else if (Interpreter::isValidAuxOp(assembly.at(i).getOppCode()))
          {
            cout << "ITS AN Hybrid AUX OP" << endl;
          }
          else
          {
            //throw 1;
          }
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

