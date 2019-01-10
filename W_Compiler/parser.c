#include <stdio.h>
#include <string.h>
//#include "parserUtility.h"

int InitParse(char*);

int main(int argc, char *argv[])
{
  // BREAK IF NO ARGUMENTS
  if (argc <= 1)
  {
    printf("Error: there is no valid arguments\n");
    return -1;
  }

  char *file_name = argv[1];
  unsigned long length = strlen(file_name);

  // DO VALIDATION FOR EXTENSION BEING CORRECT
  if (file_name[length-2] != '.' || file_name[length-1] != 'w')
  {
    printf("Error: there is no matching extensions.\n");
    return -1;
  }

  return InitParse(file_name);
}


/*
  InitParse:
    Initalizes the parsing and scanning process
*/
int InitParse(char *file_name)
{
  FILE *fp;
  char str[60];
  fp = fopen(file_name, "r");
  if (fp == NULL)
  {
    printf("Couldn't open file\n");
    return -1;
  }
  char * strLine = fgets(str, 60, fp);
  while (strLine != NULL)
  {
      printf("%s", strLine);
      strLine = fgets(str, 60, fp);
  }
  fclose(fp);

  return 0;
}
