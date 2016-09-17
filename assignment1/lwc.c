#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>

int main (int argc, char **argv){
  int i,isin_space = 1,linecount = 0,wordcount = 0,charcount = 0;
  FILE* fp;
  char ch;
  char* re_pattern= "-([lwc]+)";

  if(argc < 3){
    printf("There are too few args\n");
    return 2;
  }
  char* fname = argv[argc-1];
  printf("file name is %s\n",fname);

  fp = fopen(fname,"r");
  if(fp == NULL){
    printf("No such file\n");
    return 1;
  }

  while( (ch = fgetc(fp))!= EOF){
    charcount++;
    if(isspace(ch)){
      isin_space = 1;
      if(ch == '\n'){
        linecount++;
      }
    }
    else{
      wordcount +=isin_space;
      isin_space = 0;
    }
  }

  printf("total linecount : %d \n",linecount);
  printf("total wordcount : %d \n",wordcount);
  printf("total charcount : %d \n",charcount);

  // regfree(&reg);

  return 0;
}