// lab 3: rohit pokhrel-csci 341
// 28th March 2023
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include<sys/wait.h>

void processCommand(char* command, char* flags[20], int type, int* pid, int* stat) {
  // flags needs to be null terminated
  // if type is 0, then last command
  // if type is 1, indicates a command whose output is to be piped
  // if type is 2, indicates a command that the shell has to wait for


  // set flags0 and then run process command
  flags[0] = calloc(100, sizeof(char));
  int start_index = 0;
  for (int j = 0; j < strlen(command); j++) {
     if (command[j] == '/') start_index = j;
  }
  //printf("start index is %d\n", start_index);
  for (int j = start_index + 1; j < strlen(command); j++) {
     if (command[j] == 0) break;
     flags[0][j-start_index-1] = command[j];
  }
  for (int i = 0; i < 20; i++) {
   
    if (flags[i] == NULL) break;
    if (flags[i][strlen(flags[i]) - 1] == '\n') {
      flags[i][strlen(flags[i]) - 1] = 0;
    }
      //      printf("flags i is %s\n", flags[i]);
      if ((strlen(flags[i]) == 1) && flags[i][0] == '\n') {
	flags[i] == NULL;
      }
    
  }
  //  printf("flags 0 is %s", flags[0]);
      
  *pid = fork();
  if (*pid == 0) {
    // in child, hit execv
    //printf("%s %s", command, flags[0])
    execv(command, flags);
  } else {
    // in parent
    wait(stat);
    //printf("pid: %d signal: %d\n", pid, stat);
  }
}




void printArgs(int n, int* pid[20], int* stat[20]) {

    for (int i = 0; i < n; i++) {
      //printf("printing number %d\n", i);
      printf("pid: %d signal: %d\n", *pid[i], *stat[i]);
      //if (pid[i] != NULL) free(pid[i]);
      //free(stat[i]);
    }
    //    printf("exited\n");
}


int main() {
  printf("CS341> ");
  char* input = malloc(1000);
  fgets(input, 1000, stdin);
  // parse input
  char* path = (char*) calloc(100, sizeof(char));
  char* flags[20];
  int new = 1;
  int type = 0;
  int spaces = 0;
  int argIndex = 0;
  int pathIndex = 0;
  int type2Count = 0;
  int* pid[20];
  int* stat[20];

  for (int i = 0; i <= 19; i++) {
    pid[i] = malloc(sizeof(int));
    stat[i] = malloc(sizeof(int));
  }
 
  for (int i = 0; i <= strlen(input); i++) {
    //printf("char at index i is %c\n", input[i]);
    new = 0;
    if (i == strlen(input) || input[i] == '|' || input[i] == ';') {
     
      if (i != strlen(input)) spaces--;
      new = 1;
      if (i == strlen(input)) type = 0;
      else if (input[i] == '|') type = 1;
      else type = 2;
      // call function for executing command, and reset everything

      // set flags0 and then run process command
      //flags[0] = calloc(100, sizeof(char));
      //int start_index = 0;
      //for (int j = 0; j < strlen(command); j++) {
      //if (command[j] == '/') start_index = j;
      //}
      //for (int j = start_index + 1; j < strlen(command); j++) {
      //if (command[j] == 0) break;
      //flags[0][j-start_index] = command[j];
      //}
      for (int m = spaces + 1; m < 20; m++) flags[m] = NULL;
      if (type != 2) {
	  //printArgs(type2Count, pid, stat);
	  //printf("got here dawg");
	  //print type 2 array here
	  processCommand(path, flags, type, pid[type2Count], stat[type2Count]);
	  type2Count++;
	  //	  printf("type 2 count is %d\n", type2Count);
	  printArgs(type2Count, pid, stat);
	  type2Count = 0;
	  //printf("pid: %d signal: %d\n", *pid[0], *stat[0]);
	  //if (flags[2] != NULL) printf("%s\n", flags[2]);
	}
      else {
	processCommand(path, flags, type, pid[type2Count], stat[type2Count]);
	type2Count++;
      }
      
      // for now im printing

      // PRINT COMMAND AND PATH HERE 
      //printf("path is %s and spaces is %d \n", path, spaces);
      //for (int j = 0; j <= spaces; j++) {
	//if (flags[j] == NULL) break;
      //printf("flag is %s\n", flags[j]);
      //}

     
      // delete portion between this and prev comment
      new = 1;
      for (int j = 0; j <= 99; j++) {
	path[j] = '\0';
      }
      for (int j = 0; j <= 19; j++) {
	flags[j] = NULL;
      }
      spaces = 0;
      argIndex = 0;
      pathIndex = 0;
      while (i != strlen(input) && (input[i] == ' ' || input[i] == '|' || input[i] == ';')) {
	i++;
      }
      //printf("error causing i could be %d", i);
      if (i == strlen(input)) continue;
      if (input[i-1] != '|') i--;
      continue;
    }
    if (input[i] == ' ') {
      //printf("added to spaces here\n");
      spaces++;
      argIndex = 0;
      continue;
    }
    if (spaces == 0) {
      // add to path
      path[pathIndex] = input[i];
      pathIndex++;
    } else if (spaces >= 1) {
      if (argIndex == 0) {
	flags[spaces] = calloc(100, sizeof(char));
      }
      // current space number is argument number
      flags[spaces][argIndex] = input[i];
      argIndex++;
    }
  }
}

