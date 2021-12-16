#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"

int parseString(Data *line);
int parseArgs(Data *line);
void parseSymbols( Data *line);

int checkLineLength(int numChars)
{
   if (numChars == 1024)
   {
      fprintf(stderr, PROGRAMNAME ": Command line too long\n");
      return 1;
   }
   return 0;
}
void checkForRedirectedSTDIN(int numChars)
{
   if (numChars == 0)
      printf("exit");
}

int checkForValidCommand(int ws)
{
   if (ws == 0)
   {
      fprintf(stderr, PROGRAMNAME": Invalid pipe\n");
      return 1;
   }
   return 0;
}

int beginParse(Data *line)
{
   int c = 0, numChars = 0, returnCode = 0; /*pipeIndex = -1,*/ 
   int numNonWhitespaces = 0, invalidPipe = 1;
   printf(":-) ");
   while ((c = fgetc(stdin)) != '\n' && numChars <= 1024 && c != EOF)
   {
      if ( isspace(c) == 0 && c != '|')
         numNonWhitespaces = 1;
      if ( c == '|' && numChars >= 0)
      {
         if (numNonWhitespaces == 0)
            invalidPipe = 0;

         numNonWhitespaces = 0;
      }
      line->commandLine[numChars] = c;
      numChars++;
   }
    
   /* close program upon ctrl + d */
   if (c == EOF)
   {
      checkForRedirectedSTDIN(numChars);
      printf("\n");
      exit(0);
   }

   if ((checkForValidCommand(numNonWhitespaces) == 1) || 
      checkForValidCommand(invalidPipe) == 1)
      return 1;
   if ((returnCode = checkLineLength(numChars)) == 1)
      return returnCode;
   returnCode =parseString(line);
   return returnCode;
}
/* creates an array of strings delimited by pipe symbol for use in execv. 
 * If there are no pipe symbols, there is only one "command". Also checks
 * for invalid pipes.
 */
int parseString(Data *line)
{
   char *tok;
   int index = 0;
   tok = strtok(line->commandLine, "|");
   while (tok != NULL && index < 20)
   {
      line->commandArray[index] = tok;
      tok = strtok(NULL, "|");
      index++;
   }
   if (tok != NULL)
   {
      fprintf(stderr, PROGRAMNAME": Too many commands\n");
      return 1;
   }
   line->numCommands = index;
   return parseArgs(line);
}
/*
 * parseArgs checks each index of commandArray (an array whose indices
 * refer to a string), and parses the string into multiple strings 
 * delimited by whitespace.
 */

int checkArgCount(int numArgs, char *cmd)
{
   if(numArgs >= 11)
   {
      fprintf(stderr, PROGRAMNAME ": %s: Too many arguments\n", cmd); 
      return 1;
   }
   return 0;
}

/* will add checking for null tok pointer later. Assume user puts in
 * valid redirection, if any 
 */
int checkTok(char *tok, Data *line, int cmdIndex)
{
   if (strcmp(tok, "<") == 0)
   {
      if ((tok = strtok(NULL, " ")) == NULL)
      {
         fprintf(stderr, PROGRAMNAME ": Syntax error\n");
         return -1;
      }
      line->files[cmdIndex][0] = tok;
      line->indexOfFiles[cmdIndex][0] = 10;
      return 1;
   }
   else if (strcmp(tok, ">") == 0)
   {   
      if ((tok = strtok(NULL, " ")) == NULL)
      {
         fprintf(stderr, PROGRAMNAME ": Syntax error\n");
         return -1;
      }
      line->files[cmdIndex][1] = tok;
      line->indexOfFiles[cmdIndex][1] = 10;
      return 1;
   }

   return 0;
}
void initFileIndicies(Data *line)
{
   int i = 0;
   for (i = 0; i < line->numCommands; i++)
   {
      line->indexOfFiles[i][0] = 0;
      line->indexOfFiles[i][1] = 0;
   }
}
void insertValidCMDorEntry(Data *line, char *tok, int *argIndex, int cmdIndex)
{
   int arg = *argIndex;
   line->finalList[cmdIndex][(arg)] = tok;
   *argIndex += 1;
}

int parseArgs(Data *line)
{
   char *tok;
   int cmdIndex = 0, argIndex = 0, rc = 0;
   initFileIndicies(line);
   for(cmdIndex = 0; cmdIndex < line->numCommands; cmdIndex++)
   {
      tok = strtok(line->commandArray[cmdIndex], " ");
      while( tok != NULL)
      {
         if ((rc =checkTok(tok, line, cmdIndex)) ==0)
         {
            insertValidCMDorEntry(line, tok, &argIndex, cmdIndex);           
            tok = strtok(NULL, " "); 
         }
         else if (rc == -1)
            return 1;
         else
            tok = strtok(NULL, " ");
      }
      line->finalList[cmdIndex][argIndex] = NULL;
      line->numArgs[cmdIndex] = argIndex;
      if (checkArgCount(argIndex, line->finalList[cmdIndex][0]) == 1)
         return 1;
      argIndex = 0;
   }
   return 0;
}
