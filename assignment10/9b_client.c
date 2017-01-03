#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>

#define MAXSIZE   100

int main(int argc, char**argv){
  struct addrinfo   hints, *ailist, *aip;
  int   err, sockfd;
  char  buf[MAXSIZE];

  if (argc != 3) {
    fprintf(stderr, "usage client ipaddress portnum\n");
    return 1;
  }

  memset(&hints, 0, sizeof(hints));
  hints.ai_flags = 0;
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = 0;
  if ((err = getaddrinfo(argv[1], argv[2], &hints, &ailist)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(err));
    return 1;
  }

  for (aip = ailist; aip != NULL; aip = aip->ai_next) {
    if ((sockfd = socket(aip->ai_family, aip->ai_socktype, aip->ai_protocol)) < 0) {
      fprintf(stderr, "socket error: %s\n", strerror(errno));
      continue;
    }
    if (connect(sockfd, aip->ai_addr, aip->ai_addrlen) == 0) {
      printf("connect success\n");
      break;
    }
    close(sockfd);
  }

  if (aip == NULL) {
    fprintf(stderr, "can’t connect to %s:%s", argv[1],argv[2]);
    return 1;
  }
  freeaddrinfo(aip);

  while (read(sockfd, buf, MAXSIZE) != -1) {
    printf("%s", buf);
    sleep(5);
  }

  return 0;
}