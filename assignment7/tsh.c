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

struct job
{
    int bg;
    int jobid;
    char* command;
    pid_t pid;
};

struct job* jobary[MAXJOBS];
int latestjob = 0;

void ignsomesig(){
  signal (SIGINT, SIG_IGN);
  signal (SIGQUIT, SIG_IGN);
  signal (SIGTSTP, SIG_IGN);
  signal (SIGHUP, SIG_IGN);
  signal (SIGTTIN, SIG_IGN);
  signal (SIGTTOU, SIG_IGN);
}

void dflsomesig(){
  signal (SIGINT, SIG_DFL);
  signal (SIGQUIT, SIG_DFL);
  signal (SIGTSTP, SIG_DFL);
  signal (SIGHUP, SIG_DFL);
  signal (SIGTTIN, SIG_DFL);
  signal (SIGTTOU, SIG_DFL);
}

void addjob(struct job * j){
  jobary[latestjob] = j;
  latestjob++;
  return;
}

void removejob(int jobindex){
  free(jobary[jobindex]);
  jobary[jobindex] = NULL;
  if(latestjob-1 == jobindex){
    while(latestjob > 0 && jobary[latestjob-1] == NULL){
      latestjob--;
    }
  }
}

int getjobid(pid_t pid){
  int i = 0;
  for(;i < latestjob;i++){
    if(jobary[i] != NULL && jobary[i]->pid == pid)
      return i;
  }
  return -1;
}

void showjob(int jobindex){
  if(jobary[jobindex] != NULL){
    printf("%3d %5d %s \n",
      jobary[jobindex]->jobid
      ,jobary[jobindex]->pid
      ,jobary[jobindex]->command
    );
  }
  else
    return;
}

void showjobs(){
  printf("-------\n");
  int i = 0;
  for(;i < latestjob;i++){
    showjob(i);
  }
  printf("-------\n");
}

void sig_fork(int signo){
  // printf("childsig\n");
  pid_t pid;
  int stat;

  while ((pid = waitpid(-1, &stat, WNOHANG)) > 0){
    // printf("%d child sig\n", pid);
    if(WIFEXITED(stat)){ /* Process terminated normally. */
      removejob(getjobid(pid));
      return;
    }
  }

  return;
}

int do_buildind_cmd(int doargc, char** doargv){
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
  else if(strcmp (doargv[0], "jobs") == 0){
    showjobs();
    return 1;
  }
  else if(strcmp(doargv[0], "fg") == 0){
    int jid,tmp;
    if(doargc != 2){
      printf("usage : fg [jobid]\n");
    }
    jid = atoi(doargv[1]);
    if( jobary[jid] == NULL){
      printf("no such job\n");
    }
    ignsomesig();
    tmp = tcsetpgrp(STDIN_FILENO,jobary[jid]->pid );
    if(tmp == -1){
      printf("tcsetpgrp error\n");
    }
    // set control process as foreground job
    waitpid(jobary[jid]->pid, NULL, 0);
    tmp = tcsetpgrp(STDIN_FILENO,control_process);
    if(tmp == -1){
      printf("tcsetpgrp error\n");
    }
    removejob(jid);
    dflsomesig();
    return 1;
  }
  else if(strcmp(doargv[0], "exit") == 0){
    kill(control_process,SIGINT);
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
  tcsetpgrp(STDIN_FILENO, control_process);

  struct job* newjob;

  while(1){

    tmp = tcsetpgrp(STDIN_FILENO,control_process);
    // printf("now fgin : %d\n", tcgetpgrp(STDIN_FILENO));
    printf("%s", prompt);
    if(fgets(line, MAXLINE, stdin) != NULL && strlen(line) > 1 ){
      //control proess ignore sig after user input

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
      if( do_buildind_cmd(doargc, doargv) ){
        dflsomesig();
        continue;
      }
      ignsomesig();
      //if not build-in cmd, execve
      if((childpid = fork()) == 0){
          // printf("child process\n");
          childpid = getpid();
          setpgid(childpid,childpid);
          //defalt sig handler
          dflsomesig();
          // printf("child pid %d pgid %d\n",childpid,getpgid(childpid));
          // printf("now fgin : %d\n", tcgetpgrp(STDIN_FILENO));
          // printf("now fgout : %d\n", tcgetpgrp(STDOUT_FILENO));
          if(execvp(doargv[0], doargv) < 0){
            fprintf(stderr,"%s\n",strerror(errno));
            exit(0);
          }
      }
      else if(childpid < 0){
        fprintf(stderr,"fork error\n");
      }
      //parent
      else{
        parentpid = getpid();
        setpgid(childpid,childpid);
        // printf("parent pid %d pgid %d\n",parentpid,getpgid(parentpid));
        // printf("parent see child pid %d pgid %d\n",childpid,getpgid(childpid));
        if(bg){
          newjob = (struct job *)malloc(sizeof(struct job));
          newjob->bg = bg;
          newjob->jobid = latestjob;
          newjob->pid = childpid;
          newjob->command = malloc(sizeof(char) * MAXCMD);
          strcpy(newjob->command,doargv[0]);
          // printf("new job : bd %d jobid %d pid %d cmd %s \n",newjob->bg, newjob->jobid, newjob->pid,newjob->command);
          addjob(newjob);
          signal(SIGCHLD, sig_fork);
          tmp = tcsetpgrp(STDIN_FILENO,control_process);
          // printf("now fgin : %d\n", tcgetpgrp(STDIN_FILENO));
          //defalt sig handler
          dflsomesig();
        }
        else{
          //set child process as foreground job
          tmp = tcsetpgrp(STDIN_FILENO,childpid);
          if(tmp == -1){
            printf("tcsetpgrp error\n");
          }
          // printf("now fgin : %d\n,waitfor childprocess\n", tcgetpgrp(STDIN_FILENO));

          waitpid(childpid, NULL, 0);
          // printf("waitend\n");
          tmp = tcsetpgrp(STDIN_FILENO,control_process);
          if(tmp == -1){
            printf("tcsetpgrp error\n");
          }
          // printf("now fgin : %d\n", tcgetpgrp(STDIN_FILENO));

          //defalt sig handler
          dflsomesig();
        }
      }
    }

    tmp = tcsetpgrp(STDIN_FILENO,control_process);
    // printf("now fgin : %d\n", tcgetpgrp(STDIN_FILENO));
    // printf("now fgout : %d\n", tcgetpgrp(STDOUT_FILENO));
  }

  return 0;
}