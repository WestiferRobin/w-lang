#include "compiler_comp.hpp"

void FrontEnd::pre_parse(string file_text)
{
  string line = "";
  ifstream myfile(file_text);
  if (myfile.is_open())
  {
    while (getline(myfile,line))
    {
      for (int i = 0; i < line.length(); i++)
      {
        if (line[i] != ' ')
          straight_file += line[i];
      }
    }
    myfile.close();
  }
}


/*
TODO:
1. Write and implement LR(1) with expression grammer.
2. Implement AST Nodes for frontend.
3. Add nessesary Logic for adding tree during LR(1) parsing.
*/
void FrontEnd::parse(void)
{
  cout << straight_file << endl;
}
