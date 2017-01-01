#include <stdio.h>
#include <stdlib.h>


int main(void){
  int x = 0x76543210;
  char *cp = (char*) &x;

  if (*cp == 0x10)
    printf("little-endian\n");
  else if (*cp == 0x76)
    printf("big-endian\n");
  else
    printf("I don't understand\n");
  return 0;
}