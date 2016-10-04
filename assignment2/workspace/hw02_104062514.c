#include <stdio.h>
#include "hw2.h"
#include <unistd.h>
#include <stdlib.h>

int checkfdvalid(int fd){
  if (fd < 0 ){
    fprintf(stderr, "invalid small fd err\n");
    return -1;
  }
  if(fd > sysconf(_SC_OPEN_MAX)){
    fprintf(stderr, "invalid big fd err\n");
    return -1;
  }
  return 0;
}

int check_fd_equal(int ofd,int nfd){
  return ofd == nfd ? 1: 0;
}

int close_tmp_fd(int* fd_array, int length){
  int i,c;
  for(i = 0; i < length;i++){
    c = close(fd_array[i]);
    if(c < 0){
      return -1;
    }
  }
  return 0;
}

int mydup2(int oldfd, int newfd){
  long sc_open_max = sysconf(_SC_OPEN_MAX);
  int * fd_array = (int*) malloc(sizeof(int) * sc_open_max);
  int i,j;

  int ofd_v = checkfdvalid(oldfd);
  int nfd_v = checkfdvalid(newfd);
  if(ofd_v < 0 || nfd_v < 0){
    return -1;
  }

  int fd_eq = check_fd_equal(newfd,oldfd);
  if(fd_eq == 1){
    return newfd;
  }

  int tmp_fd = -1;
  for(i = 0; tmp_fd < newfd; i++){
    tmp_fd = dup(oldfd);
    if(tmp_fd == -1){
      fprintf(stderr, "dup err\n");
      close_tmp_fd(fd_array,i);
      return -1;
      // err while dup
    }
    fd_array[i] = tmp_fd;
  }

  // newfd are not closed
  if(fd_array[i-1] > newfd){
    int c = close(newfd);
    int r = dup(oldfd);
    close_tmp_fd(fd_array,i);
  }
  //dup to newfd suc
  else if(fd_array[i-1] == newfd){
    close_tmp_fd(fd_array,i-1);
  }
  return newfd;
}
