#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "parser.h"

int openFile(char *fileName, int flag, int mode)
{
   
   int FD = 0;
   if (mode == 1)
      FD = open(fileName, flag);
   else
      FD = open(fileName, flag, mode);
   if (FD == -1)
   {
      fprintf(stderr, PROGRAMNAME ": %s: ", fileName);
      perror(NULL);
      exit(EXIT_FAILURE);
   }
   return FD;
}

void checkForFiles(int index, Data line)
{
   int input = 0, output = 0, flags;
   flags = O_WRONLY | O_CREAT | O_TRUNC;
   if (line.indexOfFiles[index][0]== 10)
   {
      input = openFile(line.files[index][0], O_RDONLY, 1);
      dup2(input, 0);
   }
   if (line.indexOfFiles[index][1] == 10)
   {
      output = openFile(line.files[index][1], flags, 0666);
      dup2(output,1);
   }
}

void dupChild(int fdin[], int fdout[])
{

   dup2(fdin[0], 0);
   dup2(fdout[1], 1);
}

void childProcess(int inputFD[],int outputFD[], int counter, Data line)
{
   close(outputFD[0]);
   if (counter < line.numCommands - 1 && line.numCommands > 1)
      dupChild(inputFD, outputFD);
   if (counter == line.numCommands - 1 && line.numCommands > 1) 
      dup2(inputFD[0], 0);
   checkForFiles(counter , line);
   close(inputFD[0]);
   close(outputFD[1]);

}

void beginProcesses(Data line)
{
   int counter =0, inputFD[2], outputFD[2];
   pipe(outputFD); 
   inputFD[0] = outputFD[1];
   for (counter = 0; counter < line.numCommands; counter++)
   {
      if (fork() == 0)
      {
         childProcess(inputFD, outputFD, counter, line);
         execvp(line.finalList[counter][0], line.finalList[counter]); 
         fprintf(stderr, PROGRAMNAME": %s: No such file or directory\n",
            line.finalList[counter][0]);
         exit(EXIT_FAILURE);      
      }
      else
      {
         close(outputFD[1]);
         if (counter > 0)
            close(inputFD[0]);
         inputFD[0] = outputFD[0];
         pipe(outputFD);
      }
   }
   while(wait(NULL) > 0);
      
   close(outputFD[0]);
   close(outputFD[1]);
   close(inputFD[0]);
}
