#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "frontEndUtility.h"

int Initalize(char*);

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

  return Initalize(file_name);
}


/*
  Initalize:
    Initalizes the parsing and scanning process
*/
int Initalize(char *file_name)
{
  FILE *fp=fopen(file_name, "rb");
  fseek(fp, 0, SEEK_END);
  int size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  char file_string[size];
  int i = 0;
  char c;
  while((c = fgetc(fp)) != EOF)
  {
    if (c != '\n' && c != ' ')
    {
      file_string[i++] = c;
    }
  }
  file_string[i] = '\0';
  fclose(fp);

  DoFrontEnd(file_string);

  return 0;
}
