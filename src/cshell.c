#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "executor.h"
Data makeStructure()
{
   Data line;
   return line;
}

int checkForExit(Data line, int returnCode)
{
   
   if (returnCode == 1)
      return 1;
   else if(strcmp(line.finalList[0][0], "exit") == 0)
      exit(0);
   else if (returnCode == EOF)
   {
      printf("exit\n");
      exit(0);
   }
   return 0;
}

int main()
{
   Data line;
   int returnCode = 0;
   setbuf(stdout, NULL);
   while (1)
   {
      line = makeStructure();
      returnCode = beginParse(&line);
      checkForExit(line, returnCode);
      if (returnCode == 0)
         beginProcesses(line);
   }
   return returnCode;
}
