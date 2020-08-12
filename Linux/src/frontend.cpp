#include "compiler_comp.hpp"

void FrontEnd::run(string file_text, ASTNode *& the_ast)
{
  string line = "";
  ifstream myfile(file_text);

  if (myfile.is_open())
  {
    while (getline(myfile,line))
    {
      int size = line.find("//");
      if (size == string::npos)
      {
        size = line.length();
      }

      for (int i = 0; i < size; i++)
      {
        if (line[i] != ' ')
        {
          if (line[i] == '\"')
          {
            the_code += line[i++];
            while (line[i] != '\"')
            {
              the_code += line[i++];
            }
          }
          else if (line[i] == '\'')
          {
            the_code += line[i++];
            while (line[i] != '\'')
            {
              the_code += line[i++];
            }
          }
          the_code += line[i];
        }
      }
    }

    myfile.close();
    scanner();
    Parser * parser = new Parser(fe_symbol_table);
    parser->initGrammar(tokens.data(), the_ast);
  }
}

FrontEnd::FrontEnd(map<string, bool> symbol_table) : fe_symbol_table(symbol_table) 
{
   keyword_list = {
    "return",
    "const",
    "function",
    "PRINT",
    "SCAN",
    "COPY",
    "LENGTH",
    "EQUAL",
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
}

bool FrontEnd::handleVariablesAndKeywords(int * index, string * token)
{
  if (find(keyword_list.begin(), keyword_list.end(), *token) != keyword_list.end())
  {
    tokens.push_back(TokenEntry(T_KEYWORD, *token));
    return true;
  }
  else if (!(tolower(the_code[*(index)+1]) <= 'z' && tolower(the_code[*(index)+1]) >= 'a') 
    && !(the_code[*(index)+1] <= '9' && the_code[*(index)+1] >= '0') 
    && *token != "" && the_code[*(index)+1] != '_')
  {
    tokens.push_back(TokenEntry(isdigit((*token)[0]) ? T_NUMBER : T_VARIABLE, *token));
    return true;
  }
  return false;
}

void FrontEnd::handleNumbersAndVariables(int * index, string * token)
{
  int tempIndex = *(index) + 1;
  bool isNumber = true;
  while (isdigit(the_code[tempIndex])) { *token += the_code[tempIndex++];}

  for (int charIndex = 0; charIndex < (*token).length(); charIndex++) 
  {
    if (isdigit((*token)[charIndex])) continue;
    isNumber = false; 
    break;
  }
  tokens.push_back(TokenEntry(isNumber ? T_NUMBER : T_VARIABLE, *token));
  *index = tempIndex - 1;
}

void FrontEnd::handleCharactersAndStrings(int * index, string * token)
{
  if (the_code[*index] == '\'')
  {
    (*index)++;
    string message = to_string(the_code[*(index)]);
    tokens.push_back(TokenEntry(T_CHAR, message));
    (*index)++;
    return;
  }
  else
  {
    *token += the_code[*index];
    tokens.push_back(TokenEntry(T_SYMBOL, "\""));
    int startIndex = *(index) + 1;

    while (the_code[startIndex] != the_code[*index]) 
    {
      tokens.push_back(TokenEntry(T_CHAR, to_string(the_code[startIndex++])));
    }

    *index = startIndex;
    tokens.push_back(TokenEntry(T_SYMBOL, "\""));
  }
}

void FrontEnd::handleOperatorsTypeOne(int * index, string * token)
{
  switch (the_code[*(index) + 1])
  {
    case '=':
      tokens.push_back(TokenEntry(T_SYMBOL, *token + the_code[*(index) + 1]));
      *(index) += 1;
      break;
    default:
      if (isdigit(the_code[*(index) + 1]) && the_code[*(index) - 1] == '-' && the_code[*index] == '-')
      {
        int digitIndex = *(index) + 1;
        while (isdigit(the_code[digitIndex])) { *token += the_code[digitIndex++];}
        tokens.push_back(TokenEntry(T_NUMBER, *token));
        *index = digitIndex - 1;
      }
      else
      {
        tokens.push_back(TokenEntry(T_SYMBOL, *token));
      }
      break;
  }
}

void FrontEnd::handleOperatorsTypeTwo(int * index, string * token, bool * isMultiLine)
{
  switch (the_code[*(index)+1])
  {
    case '*':
    case '/':
      *isMultiLine = the_code[*(index)] == '/' && the_code[*(index)+1] == '*';
      *index += 1;
      break;
    case '=':
      tokens.push_back(TokenEntry(T_SYMBOL, *token + the_code[*(index)+1]));
      *index += 1;
      break;
    default:
      tokens.push_back(TokenEntry(T_SYMBOL, *token));
  }
}

void FrontEnd::handleOperatorsTypeThree(int * index, string * token)
{
  switch (the_code[*(index)+1])
  {
    case '&':
    case '|':
    case '=':
      tokens.push_back(TokenEntry(T_SYMBOL, *token + the_code[*(index)+1]));
      *(index) += 1;
      break;
    default:
      tokens.push_back(TokenEntry(T_SYMBOL, *token));
      break;
  }
}

void FrontEnd::handleOperatorsTypeFour(int * index, string * token)
{
  string buildInstance = "";
  switch (the_code[*(index)+1])
  {
    case '=':
    case '<':
    case '>':
      buildInstance = *token + the_code[*(index)+1] + the_code[*(index)+2];
      tokens.push_back(TokenEntry(T_SYMBOL, buildInstance == "<<=" || buildInstance == ">>="  ? buildInstance : *token + the_code[*(index)+1]));
      *(index) += buildInstance == "<<=" || buildInstance == ">>=" ? 2 : 1;
      break;
    default:
      tokens.push_back(TokenEntry(T_SYMBOL, *token));
  }
}

void FrontEnd::scanner(void)
{
  string hold = "";
  bool isMultiLine = false;

  for (int i = 0; i < the_code.length(); i++)
  {
    if (isMultiLine && the_code[i] != '*') { continue; }

    hold += the_code[i];

    if ((tolower(the_code[i]) <= 'z' && tolower(the_code[i]) >= 'a') || the_code[i] == '_')
    {
      if (!handleVariablesAndKeywords(&i, &hold)) continue;
    }
    else if (isdigit(the_code[i]))
    {
      handleNumbersAndVariables(&i, &hold);
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
          tokens.push_back(TokenEntry(T_SYMBOL, hold));
          break;
        case '\"':
        case '\'':
          handleCharactersAndStrings(&i, &hold);
          break;
        case '+':
        case '^':
        case '%':
        case '-':
        case '!':
        case '=':
          handleOperatorsTypeOne(&i, &hold);
          break;
        case '*':
        case '/':
          handleOperatorsTypeTwo(&i, &hold, &isMultiLine);
          break;
        case '&':
        case '|':
          handleOperatorsTypeThree(&i, &hold);
          break;
        case '<':
        case '>':
          handleOperatorsTypeFour(&i, &hold);
          break;
        default:
          ErrorReader::readError(ERROR_INVALID_SYMBOL, to_string(the_code[i]));
          break;
      }
    }
    hold = "";
  }
  tokens.push_back(TokenEntry(T_ENDOFFILE, ""));
}