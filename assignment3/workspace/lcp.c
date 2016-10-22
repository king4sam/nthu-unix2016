#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>


#define BUFFSIZE 4096


int main(int argc, char **argv){
  char buf[BUFFSIZE];

  char* src_filename = argv[1];
  char* tar_filename = argv[2];
  // printf("src file %s\n",src_filename);
  // printf("tar file %s\n", tar_filename);
  int srcfd = open(src_filename,O_RDONLY);
  srcfd < 0 ? fprintf(stderr, "src open failed\n" ): printf("src fd %d\n", srcfd);

  struct stat src_stat;
  fstat( srcfd, &src_stat) == -1 ? fprintf(stderr, "get stat err\n"):printf("get stat suc");
  int tarfd = open(tar_filename, O_WRONLY|O_CREAT | O_EXCL,src_stat.st_mode);
  tarfd < 0 ? fprintf(stderr, "tar open failed\n" ): printf("tar fd %d\n", tarfd);

  int bytes_read;
  while ((bytes_read = read(srcfd, buf, BUFFSIZE)) > 0){
    if (write(tarfd, buf, bytes_read) != bytes_read)
      fprintf(stderr, "write error\n" );
    if (bytes_read < 0)
      fprintf(stderr, "read error\n" );
  }

  return 0;
}