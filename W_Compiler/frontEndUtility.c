#include "frontEndUtility.h"

void DoFrontEnd(char * rawString, int size)
{
  printf("%s\n", rawString);
  regex_t reOp;
  regex_t reNum;
  int flagNum = regcomp(&reNum, "[1-9]+", REG_EXTENDED|REG_NOSUB);
  if(flagNum!=0)
  {
    return;
  }

  for (int i=0; i<size; i++)
  {
    char * asdf = "";
    size_t len;
    char *str2 = "";
    char charPoint = rawString[i];
    flagNum = regexec(&reNum, &charPoint,0,NULL,0);
    if(flagNum == 0)
    {
      printf("%c", rawString[i]);
    }


  }

  regfree(&reNum);

}
