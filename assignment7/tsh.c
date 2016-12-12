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
#define MAXJOBS 128
const char prompt[] = "tsh >>> ";
extern char **environ;

pid_t control_process;


void sig_fork(int signo)
{
    printf("childsig\n");
    pid_t pid;
    int stat;
    pid = waitpid(-1,&stat,0);

    if(pid != -1){
      printf("%d child sig\n", pid);
      // printf("child procrss %d terminate\n",pid);
    }
    return;
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
  else if(strcmp(doargv[0], "jobs") == 0){

  }
  else if(strcmp(doargv[0], "fg") == 0){

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

int checkbg(int* argc, char** argv){
  int c = *(argc);
  if(strcmp(argv[c-1], "&") == 0){
    argv[c-1] = NULL;
    (*argc) = c-1;
    return 1;
  }
  else{
    return 0;
  }
  return strcmp(argv[c-1], "&") == 0? 1:0;
}


int main(int argc,char** argv){
  int doargc = 0 ,bg;
  char** doargv;
  char cmd[MAXCMD];
  char line[MAXLINE];
  pid_t childpid;
  pid_t parentpid;
  int tmp;

  // pid_t pgrpid = getpgid(getpid());
  // pid_t fggrpid = tcgetpgrp(0);
  // printf("mygrpid %d fdpgrpid %d \n", pgrpid, fggrpid);
  //
  control_process = getpid();
  tcsetpgrp (STDIN_FILENO, control_process);


  while(1){
    printf("%s", prompt);


    if(fgets(line, MAXLINE, stdin) != NULL){
      //control proess ignore sig after user input
      signal (SIGINT, SIG_IGN);
      signal (SIGQUIT, SIG_IGN);
      signal (SIGTSTP, SIG_IGN);
      signal (SIGHUP, SIG_IGN);
      signal (SIGTTIN, SIG_IGN);
      signal (SIGTTOU, SIG_IGN);
      // signal(SIGINT,SIG_IGN);
      // signal(SIGTSTP,ctrlz);

      line[strlen(line)-1] = '\0';

      //parse cmd to build doargc doarvg
      doargv = malloc(sizeof(char*) * MAXARGV);
      for (int i = 0; i < MAXARGV; ++i)
      {
        doargv[i] = malloc(sizeof(char) * MAXCMD);
      }
      parsecmd(line, &doargc,doargv);

      //check isbg?
      bg = checkbg(&doargc, doargv);
      // printf("isbg ? %d \n", bg);
      //build-in cmd
      if( do_buildind_cmd(doargv) ){
        continue;
      }

      //if not build-in cmd, execve
      if((childpid = fork()) == 0){
          // printf("child process\n");
          childpid = getpid();
          setpgid(childpid,childpid);
          //defalt sig handler
          signal (SIGINT, SIG_DFL);
          signal (SIGQUIT, SIG_DFL);
          signal (SIGTSTP, SIG_DFL);
          signal (SIGHUP, SIG_DFL);
          signal (SIGTTIN, SIG_DFL);
          signal (SIGTTOU, SIG_DFL);
          printf("child pid %d pgid %d\n",childpid,getpgid(childpid));
          if(execvp(doargv[0], doargv) < 0){
            fprintf(stderr,"%s\n",strerror(errno));
            exit(0);
          }
          printf("child terminate\n");
      }
      else if(childpid < 0){
        fprintf(stderr,"fork error\n");
      }
      //parent
      else{
        parentpid = getpid();
        printf("parent pid %d pgid %d\n",parentpid,getpgid(parentpid));
        // printf("parent see child pid %d pgid %d\n",childpid,getpgid(childpid));
        if(bg){
          signal(SIGCHLD, sig_fork);
        }
        if(!bg){
          //set child process as foreground job
          tmp = tcsetpgrp(STDIN_FILENO,childpid);
          if(tmp == -1){
            printf("tcsetpgrp error\n");
          }

          // set control process as foreground job
          waitpid(childpid, NULL, 0);
          tmp = tcsetpgrp(STDIN_FILENO,control_process);
          if(tmp == -1){
            printf("tcsetpgrp error\n");
          }

          //defalt sig handler
          signal (SIGINT, SIG_DFL);
          signal (SIGQUIT, SIG_DFL);
          signal (SIGTSTP, SIG_DFL);
          signal (SIGHUP, SIG_DFL);
          signal (SIGTTIN, SIG_DFL);
          signal (SIGTTOU, SIG_DFL);
        }
      }

    }
  }

  return 0;
}