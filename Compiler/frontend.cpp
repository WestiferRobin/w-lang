#include "compiler_comp.hpp"

/*
  Public Function: run
  - runs the operation of FrontEnd.
*/
void FrontEnd::run(string file_text)
{
  // local variables
  string line = "";
  ifstream myfile(file_text);

  // if file is able to be opened then proceed with operations.
  if (myfile.is_open())
  {
    // iterate through line
    while (getline(myfile,line))
    {
      // check for single line comments before operations
      int size = line.find("//");
      if (size == string::npos)
      {
        size = line.length();
      }

      // remove all white spaces before operations
      for (int i = 0; i < size; i++)
      {
        if (line[i] != ' ')
          the_code += line[i];
      }
    }
    // do private operations for front end.
    FrontEnd::scanner();

    // for (auto i = tokens.begin(); i != tokens.end(); i++)
    // {
    //   cout << i->entry << " " << i->tType << endl;
    // }
    FrontEnd::parse();

    // close file after operations
    myfile.close();
  }
}

/*
  Private Function: assignVariables
  - This assigns private datamembers for class.
*/
void FrontEnd::assignVariables()
{
  // assign key words to list for keyword_list 
  keyword_list = {
    "return",
    "const",
    "function",
    "PRINT",
    "let",
    "for",
    "while",
    "if",
    "elif",
    "else",
    "import",
    "switch",
    "case",
    "break",
    "delete"
  };

  // assign the ast to be null
  the_ast = NULL;
}

/*
  Private Function: scanner
  - creates all the needed tokens for the FrontEnd.
*/
void FrontEnd::scanner(void)
{
  string hold = "";
  string buildInstance = "";
  int startIndex = INT_MIN;
  int stopIndex = INT_MAX;
  bool isMultiLine = false;
  TokenEntry * temp;

  for (int i = 0; i < the_code.length(); i++)
  {
    if (isMultiLine && the_code[i] != '*') { continue; }

    hold += the_code[i];

    if ((tolower(the_code[i]) <= 'z' && tolower(the_code[i]) >= 'a') || the_code[i] == '_')
    {
      if (find(keyword_list.begin(), keyword_list.end(), hold) != keyword_list.end())
      {
        temp = new TokenEntry(T_KEYWORD, hold);
        tokens.push_back(*temp);
        hold = "";
      }
      else if (!(tolower(the_code[i+1]) <= 'z' && tolower(the_code[i+1]) >= 'a') && !(the_code[i+1] <= '9' && the_code[i+1] >= '0') && hold != "" && the_code[i+1] != '_')
      {
        temp = new TokenEntry(isdigit(hold[0]) ? T_NUMBER : T_VARIABLE, hold);
        tokens.push_back(*temp);
        hold = "";
      }
    }
    else if (isdigit(the_code[i]))
    {
      int j = i + 1;
      bool isNumber = true;
      while (isdigit(the_code[j])) { hold += the_code[j]; j++;}

      for (int k = 0; k < hold.length(); k++) 
      {
        if (isdigit(hold[k]) == false) 
        {
          isNumber = false; 
          break;
        }
      }

      temp = new TokenEntry(isNumber ? T_NUMBER : T_VARIABLE, hold);
      tokens.push_back(*temp);
      i = j - 1;
      hold = "";
    }
    else
    {
      switch (the_code[i])
      {
        case '_':
          break;
        case '{':
        case '}':
        case '[':
        case ']':
        case '(':
        case ')':
        case ';':
        case ',':
        case ':':
          temp = new TokenEntry(T_SYMBOL, hold);
          tokens.push_back(*temp);
          break;
        case '\"':
          hold += the_code[i];
          temp = new TokenEntry(T_SYMBOL, "\"");
          tokens.push_back(*temp);
          startIndex = i + 1;
          while (the_code[startIndex] != the_code[i]) 
          {
            temp = new TokenEntry(T_CHAR, to_string(the_code[startIndex++]));
            tokens.push_back(*temp);
          }
          i = startIndex;
          temp = new TokenEntry(T_SYMBOL, "\"");
          tokens.push_back(*temp);
          hold = "";
          break;
        case '\'':
          temp = new TokenEntry(T_CHAR, to_string(the_code[++i]));
          tokens.push_back(*temp);
          i++;
          hold = "";
          break;
        case '+':
        case '^':
        case '%':
        case '-':
        case '!':
        case '=':
          switch (the_code[i+1])
          {
            case '=':
              temp = new TokenEntry(T_SYMBOL, hold + the_code[i+1]);
              tokens.push_back(*temp);
              i += 1;
              break;
            default:
              if (isdigit(the_code[i+1]) && the_code[i-1] == '-' && the_code[i] == '-')
              {
                int j = i + 1;
                while (isdigit(the_code[j])) { hold += the_code[j]; j++;}
                temp = new TokenEntry(T_NUMBER, hold);
                tokens.push_back(*temp);
                i = j - 1;
              }
              else
              {
                temp = new TokenEntry(T_SYMBOL, hold);
                tokens.push_back(*temp);
              }
              break;
          }
          break;
        case '*':
        case '/':
          switch (the_code[i+1])
          {
            case '*':
            case '/':
              isMultiLine = the_code[i] == '/' && the_code[i+1] == '*';
              i += 1;
              break;
            case '=':
              temp = new TokenEntry(T_SYMBOL, hold + the_code[i+1]);
              tokens.push_back(*temp);
              i += 1;
              break;
            default:
              temp = new TokenEntry(T_SYMBOL, hold);
              tokens.push_back(*temp);
          }
          break;
        case '&':
        case '|':
          switch (the_code[i+1])
          {
            case '&':
            case '|':
            case '=':
              temp = new TokenEntry(T_SYMBOL, hold + the_code[i+1]);
              tokens.push_back(*temp);
              i += 1;
              break;
            default:
              temp = new TokenEntry(T_SYMBOL, hold);
              tokens.push_back(*temp);
          }
          break;
        case '<':
        case '>':
          switch (the_code[i+1])
          {
            case '=':
            case '<':
            case '>':
              buildInstance = hold + the_code[i+1] + the_code[i+2];
              temp = new TokenEntry(T_SYMBOL, buildInstance == "<<=" || buildInstance == ">>="  ? buildInstance : hold + the_code[i+1]);
              tokens.push_back(*temp);
              i += buildInstance == "<<=" || buildInstance == ">>=" ? 2 : 1;
              break;
            default:
              temp = new TokenEntry(T_SYMBOL, hold);
              tokens.push_back(*temp);
          }
          break;
        default:
          ErrorReader::readError(ERROR_INVALID_SYMBOL, to_string(the_code[i]));
          break;
      }
      hold = "";
    }
  }
  temp = new TokenEntry(T_ENDOFFILE, "");
  tokens.push_back(*temp);
  delete temp;
}

/*
  Private Function: parser
  - generates the AST for the backend.
*/
void FrontEnd::parse()
{
  the_ast = parser.initGrammar(tokens.data());
}



