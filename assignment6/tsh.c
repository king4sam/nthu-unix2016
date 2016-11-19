#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define MAXPATHLENG 1000
#define MAXLINE 1000
#define MAXARGV 10
#define MAXCMD 100
const char prompt[] = "tsh >>> ";
extern char **environ;

int do_cd(char** doargv){

}

int do_buildind_cmd(char** doargv){
  int rint;
  char* rptr;

  if( strcmp(doargv[0],"cd") == 0 ){
    rint = chdir(doargv[1]);
    if(rint != 0){
      printf("%s\n",strerror(errno));
    }
    return 1;
  }
  else if( strcmp(doargv[0], "pwd") == 0 ){
    char* pathbuf = malloc(sizeof(char) * MAXPATHLENG);
    rptr = getcwd(pathbuf,MAXPATHLENG);
    rptr == NULL? printf("%s\n",strerror(errno)) : printf("%s\n",pathbuf);
    return 1;
  }
  else{
    return 0;
  }
}

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
    // printf("del : %s\n", substr);
    doargv[count] = malloc(sizeof(char) * MAXCMD);
    strcpy(doargv[count],substr);
    count++;

    substr = strtok(NULL, " ");
  }
  doargv[count] = NULL;
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
  pid_t childpid;


  while(1){
    printf("%s", prompt);

    if(fgets(line, MAXLINE, stdin) != NULL){
      line[strlen(line)-1] = '\0';
      // printf("get line : %s\n", line);

      //parse cmd to build doargc doarvg
      doargv = malloc(sizeof(char*) * MAXARGV);
      for (int i = 0; i < MAXARGV; ++i)
      {
        doargv[i] = malloc(sizeof(char) * MAXCMD);
      }
      // printf("line %s argc %d \n",line, doargc);
      parsecmd(line, &doargc,doargv);

      // printf("cmd %s, argc %d \n",doargv[0], doargc);
      // for (int i = 0; i < doargc; ++i)
      // {
      //   printf("argv : %s\n", doargv[i]);
      // }

      //check isbg?
      bg = checkbg(doargc, doargv);
      // printf("isbg ? %d \n", bg);

      printf("cmd is %s \n", doargv[0]);

      //build-in cmd
      if( do_buildind_cmd(doargv) ){
        continue;
      }

      if((childpid = fork()) == 0){
          printf("child process\n");
          setpgid(childpid,childpid);
          if(execvp(doargv[0], doargv) < 0){
            printf("Command %s 404.\n", doargv[0]);
            exit(0);
          }
      }
      else if(childpid < 0){
        printf("fork error\n");
      }
      //parent
      else{
        printf("parent waiting\n");
        if (waitpid(childpid, NULL, 0) != childpid){
          printf("waitpid err\n");
        }
        else{
          printf("child terminate\n");
        }
      }
      //if not build-in cmd, execve

      // else
      //docmd(argc,arvg)
    }



  }

  return 0;
}