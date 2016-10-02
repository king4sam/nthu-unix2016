#include <stdio.h>
#include "hw2.h"
#include <unistd.h>

int mydup2(int oldfd, int newfd){
  // printf("oldfd %d, newfd %d, _SC_OPEN_MAX %ld\n", oldfd,newfd,sysconf(_SC_OPEN_MAX));
  int i,j,fd_array[_SC_OPEN_MAX] = {-2};

  if (oldfd < 0 || newfd < 0 ){
    fprintf(stderr, "invalid small fd err\n");
    return -1;
  }
  if(oldfd > sysconf(_SC_OPEN_MAX) || newfd > sysconf(_SC_OPEN_MAX)){
    fprintf(stderr, "invalid big fd err\n");
    return -1;
  }
  if (oldfd == newfd){
    return newfd;
  }

  int tmp_fd = -1;
  for(i = 0; tmp_fd < newfd; i++){
    tmp_fd = dup(oldfd);
    // printf("tmpfd %d\n", tmp_fd);
    if(tmp_fd == -1){
      fprintf(stderr, "dup err\n");
      for(j = 0;j<i;j++){
        close(fd_array[j]);
      }
      return -1;
      // err while dup
    }

    fd_array[i] = tmp_fd;
  }

  // newfd are not closed
  if(fd_array[i-1] > newfd){
    for(j = 0;j<i;j++){
      close(fd_array[j]);
    }
    // printf("newfd is not closed\n");
    int c = close(newfd);
    // fprintf(stderr, "t %d c %d\n", t,c);

    int r = dup(oldfd);
    // fprintf(stderr, "r %d \n", r);
  }
  //dup to newfd suc
  else if(fd_array[i-1] == newfd){
    // printf("newfd suc\n");
    for(j = 0; j < i-1;j++){
      close(fd_array[j]);
    }

  }
  // printf("end\n");
  return newfd;
}
