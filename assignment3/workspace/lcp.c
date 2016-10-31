#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

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
  fstat( srcfd, &src_stat) == -1 ? fprintf(stderr, "get stat err\n"):printf("get stat suc\n");
  off_t n_offset = 0;

  int tarfd = open(tar_filename, O_WRONLY|O_CREAT | O_EXCL,src_stat.st_mode);
  if(tarfd < 0){
    tarfd = open(tar_filename, O_WRONLY);
  }
  tarfd < 0 ? fprintf(stderr, "tar open failed\n" ): printf("tar fd %d\n", tarfd);

  int bytes_read;
  while ((bytes_read = read(srcfd, buf, 1)) > 0){
    if(buf[0] == 0){
      buf[0] = '\0';
      write(tarfd,buf,bytes_read);
    }
    else{
      write(tarfd,buf,bytes_read);
    }
  //   if (write(tarfd, buf, bytes_read) != bytes_read)
  //     fprintf(stderr, "write error\n" );
  //   if (bytes_read < 0)
  //     fprintf(stderr, "read error\n" );
  }

  return 0;
}