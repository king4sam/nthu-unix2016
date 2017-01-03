#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <pthread.h>

#define DIRNAME "/proc"
#define BACKLOG  16
#define MAXSIZE 16

static void *
send_pronum(void *data)
{
  int   err;
  int   connfd;
  char  pronumbuf[100];

  connfd = *(int *)data;
  free(data); //free connfd memery

  for (; ;) {
    struct sysinfo si;
    if(sysinfo(&si) != 0){
      fprintf(stderr, "sysinfo error\n");
      pthread_exit(NULL);
    }
    sprintf(pronumbuf,"there are %d process on the host.\n",(int)si.procs);

    if (write(connfd , pronumbuf, strlen(pronumbuf) + 1) < 0) { //sizeof(pronumbuf) is wrong
      fprintf(stderr, "write error: %s\n", strerror(errno));
      pthread_exit(NULL);
    }
    printf("write done\n");

    sleep(5);
  }
}

int
main(int argc, char *argv[])
{
  int     listenfd, *connfdp;
  int     err;
  pid_t childpid;
  socklen_t len;
  struct addrinfo   hint, *ailist, *aip;
  struct sockaddr_in  cliaddr;
  pthread_t tid;
  char    clihost[MAXSIZE], cliport[MAXSIZE];
  pthread_attr_t    attr;

  if (argc != 2) {
    fprintf(stderr, "usage server portnum\n");
    return 1;
  }

  if ((err = pthread_attr_init(&attr)) != 0) {
    fprintf(stderr, "pthread_attr_init error: %s\n", strerror(err));
    return 1;
  }
  if ((err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)) != 0) {//create detach pthread, also can use pthread_detach
    fprintf(stderr, "pthread_attr_setdetachstat error: %s\n", strerror(err));
    return 1;
  }
  memset(&hint, 0, sizeof(hint));
  hint.ai_flags = AI_PASSIVE;
  hint.ai_family = AF_INET;
  hint.ai_socktype = SOCK_STREAM;
  hint.ai_protocol = 0;

  if ((err = getaddrinfo(NULL, argv[1], &hint, &ailist) != 0)) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(err));
    return 1;
  }
  for (aip = ailist; aip != NULL; aip = aip->ai_next) {
    if ((listenfd = socket(aip->ai_family, aip->ai_socktype, aip->ai_protocol)) < 0)
      continue;
    if (bind(listenfd, aip->ai_addr, aip->ai_addrlen) == 0)
      break;
    close(listenfd);
  }

  if (aip == NULL) {
    fprintf(stderr, "no available address\n");
    return 1;
  }
  freeaddrinfo(aip);

  if (listen(listenfd, BACKLOG) != 0) {
    fprintf(stderr, "listen error: %s\n", strerror(errno));
    return 1;
  }
  printf("start accept request!\n");

  for (; ;) {
    len = sizeof(cliaddr);
    if ((connfdp = malloc(sizeof(int))) == NULL) {  //just in case, make connfd to be private
      fprintf(stderr, "no available memery\n");
      return 1;
    }

    if ((*connfdp = accept(listenfd, (struct sockaddr *)&cliaddr, &len)) < 0) {
      fprintf(stderr, "accept error: %s", strerror(errno));
      continue;
    }

    if ((err = getnameinfo((struct sockaddr *)&cliaddr, sizeof(cliaddr), //get client host and port
                clihost, MAXSIZE, cliport,MAXSIZE, NI_NUMERICSERV)) != 0) {
      fprintf(stderr, "getnameinfo error: %s\n", gai_strerror(err));
      return 1;
    }
    if((childpid = fork()) == 0){
      if (pthread_create(&tid, &attr, send_pronum, connfdp) != 0) { //once get a connfd , create a pthread immediately
        fprintf(stderr, "pthread_create error\n");
        close(*connfdp);  //if failure, close connection and free the memery we requested
        free(connfdp);
      }
      printf("pthread %ld connect to client %s port %s\n", tid, clihost, cliport);
    }
    else if(childpid < 0){
      fprintf(stderr,"fork error\n");
    }
    //parent

  }
  printf("break for loop \n");
  return 0;
}