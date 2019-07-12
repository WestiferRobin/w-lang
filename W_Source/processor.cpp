#include "processor.hpp"

void Processor::orgAssembly(string * raw)
{
  unsigned int count = 0;
  while (raw->substr(0, 2) != "00")
  {
    Entry entry(*raw, getType(*raw)); 
    assembly.insert(pair<unsigned int, Entry>(count, entry));
    raw++;
    count++;
  }
  Entry entry("00000000", H);
  assembly.insert(pair<unsigned int, Entry>(count, entry));
}

EntryType Processor::getType(string raw)
{
  switch (raw.at(0))
  {
    case '0':
      return R;
    case '1':
      return I;
    default:
      return M;
  }
}

void Processor::run()
{
  unsigned int i = 0;
  while (i < UINT_MAX)
  {
    switch (assembly.at(i).getEntryType())
    {
      case H:
        cout << "End of Program." << endl;
        return;
      case R:
        unsigned int rd = 1;
        short dest = regg.at(assembly.at(i).getRegister());
        unsigned int rs = 2;
        short source = regg.at(assembly.at(i).getRemaining());
        // operation
        break;
      case I:
        unsigned int rd = 1;
        short dest = regg.at(assembly.at(i).getRegister());
        short source = regg.at(assembly.at(i).getRemaining());
        // operation
        break;
      case M:
        break;
      default:
        cout << "ERROR!!!!" << endl;
        return;
    }
    i++;
  }
}

