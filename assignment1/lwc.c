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
  size_t nmatch = 2;
  regex_t reg;
  regmatch_t pmatch[ nmatch ];
  int flag = REG_EXTENDED|REG_ICASE;
  if( regcomp( &reg , re_pattern  , flag ) != 0 ){
    printf( "regexp comp error.\n" );
    exit(1);
  }
  int j = 0;
  if (regexec( &reg , argv[1] , nmatch , pmatch , 0 ) == 0){
    int match_len = pmatch[1].rm_eo - pmatch[1].rm_so;
    printf(" match length %d\n", match_len);
    char* m = malloc((match_len+1)* sizeof(char) );
    m[match_len] = '\0';
    strncpy(m, &argv[1][pmatch[1].rm_so],match_len);
    printf("%d \n",strlen(m));
    printf("args is %s\n",m);

    for(j = 0; j < match_len;j++){
      switch (m[j]){
        case 'l':
          printf("it's l\n");
          break;
        case 'w':
          printf("it's w\n");
          break;
        case 'c':
          printf("it's c\n");
          break;
      }
    }
  }

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