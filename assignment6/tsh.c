#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE 1000
#define MAXARGV 10
#define MAXCMD 100
const char prompt[] = "tsh >>> ";

int parsecmd(char* line, int* doargc, char** doargv){

  char* cpline;
  char* substr;
  cpline = malloc(sizeof(char) * strlen(line));
  strcpy(cpline,line);
  int count = 0;

  substr = strtok(cpline, " ");
  strcpy(doargv[0],substr);
  count++;
  substr = strtok(NULL, " ");
  while (substr){
    printf("del : %s\n", substr);
    doargv[count] = malloc(sizeof(char) * MAXCMD);
    strcpy(doargv[count],substr);
    count++;

    substr = strtok(NULL, " ");
  }
  (*doargc) = count;

  return 0;
}

int checkbg(int argc, char** argv){
  return strcmp(argv[argc-1], "&") == 0? 1:0;
}


int main(int argc,char** argv){
  int doargc = 0 ,bg;
  char** doargv;
  char cmd[MAXCMD];
  char line[MAXLINE];


  while(1){
    printf("%s", prompt);

    if(fgets(line, MAXLINE, stdin) != NULL){
      line[strlen(line)-1] = '\0';
      printf("get line : %s\n", line);

      //parse cmd to build doargc doarvg
      doargv = malloc(sizeof(char*) * MAXARGV);
      for (int i = 0; i < MAXARGV; ++i)
      {
        doargv[i] = malloc(sizeof(char) * MAXCMD);
      }
      printf("line %s argc %d \n",line, doargc);
      parsecmd(line, &doargc,doargv);

      // printf("cmd %s, argc %d \n",doargv[0], doargc);
      // for (int i = 0; i < doargc; ++i)
      // {
      //   printf("argv : %s\n", doargv[i]);
      // }

      //check isbg?
      bg = checkbg(doargc, doargv);
      printf("isbg ? %d \n", bg);


      // if((pid = Fork()) == 0){                                                //Run user process in a child
      //     if(execve(cmd, argv, environ) < 0){                             //executes user command if successful
      //         printf("%s: Command not found.\n", argv[0]);                    //Throw error if execution unsuccessful
      //         exit(0);
      //     }
      // }
      //if not build-in cmd, execve

      // else
      //docmd(argc,arvg)
    }



  }

  return 0;
}