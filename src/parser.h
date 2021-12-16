/* This structure contains data structures for maintaining the 
 * parsed input from the command line. The crucial elements of this
 * structure are finalList, numCommands, and numArgs. The others are
 * meant to prevent dynamic memory use while maintaining the data while
 * on the stack.
 *
 * finalList is an array of lists of strings. Each index represents a command
 * and its arguments. Notice the first index (0 - 19) indicates the command
 * number the user wrote. The second index (0-11) represents the arguments
 * needed for the command, with the actual command being 0.
 *
 * indexOfFiles contains the index of the arguments saved in finalList
 * to be used as input and output files. The second index, 0 or 1, 
 * represents input and output, respectively. The first index, 0 - 19,
 * represents the command number that the input/output file is used with.
 *
 * numCommands represents the amount of different commands to be executed.
 * Multiple commands implies there are multiple pipings to be used.
 *
 * numArgs represent the number of arguments per command, with the command
 * being the index of the array.
 */
#ifndef PARSER_H
#define PARSER_H

#define PROGRAMNAME "cshell"
typedef struct
{
   char commandLine[1025];
   char *commandArray[20];
   char *finalList[20][11];
   char *files[20][2];
   int numCommands;
   int numArgs[20];
   int indexOfFiles[20][2];

} Data;
int beginParse(Data *line);
#endif
