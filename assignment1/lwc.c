#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>

#define LINE 0
#define WORD 1
#define CHARACTER 2

int main (int argc, char **argv){
  int i,isin_space = 1;
  unsigned long *charcount,*linecount,*wordcount,*filename;
  int maxcount = 0;
  int options[3] = {0,0,0};// line word character

  // init
  charcount = (unsigned long*)malloc(argc * sizeof(unsigned long));
  linecount = (unsigned long*)malloc(argc * sizeof(unsigned long));
  wordcount = (unsigned long*)malloc(argc * sizeof(unsigned long));
  filename = (unsigned long*)malloc(argc * sizeof(unsigned long));
  for (int i = 0; i < argc; i++){
    wordcount[i] = linecount[i] = charcount[i] = filename[i]= 0;
  }
  int filecount = 0, t_linecount = 0, t_wordcount = 0, t_charcount = 0;
  FILE* fp;
  char ch;
  char* re_pattern= "-([lwc]+)";
  size_t nmatch = 2;
  regex_t reg;
  regmatch_t pmatch[ nmatch ];
  int flag = REG_EXTENDED|REG_ICASE;

  //too few args
  if(argc < 3){
    printf("There are too few args\n");
    return 2;
  }
  //reg err
  if( regcomp( &reg , re_pattern  , flag ) != 0 ){
    printf( "regexp comp error.\n" );
    exit(1);
  }
  //process args
  for(i = 1; i < argc;i++){
    int j = 0;
    //legal options
    if (regexec( &reg , argv[i] , nmatch , pmatch , 0 ) == 0){
      int match_len = pmatch[1].rm_eo - pmatch[1].rm_so;
      // printf(" match length %d\n", match_len);
      char* m = malloc((match_len+1)* sizeof(char) );
      m[match_len] = '\0';
      strncpy(m, &argv[i][pmatch[1].rm_so],match_len);
      // printf("%d \n",strlen(m));
      // printf("args is %s\n",m);

      for(j = 0; j < match_len;j++){
        m[j] = tolower(m[j]);
        switch (m[j]){
          case 'l':
            // printf("it's l\n");
            options[LINE] = 1;
            break;
          case 'w':
            // printf("it's w\n");
            options[WORD] = 1;
            break;
          case 'c':
            // printf("it's c\n");
            options[CHARACTER] = 1;

            break;
        }
      }
    }
    //illegal options
    else if(argv[i][0] == '-'){
      printf("wc: illegal option -- %s\n",argv[i]);
    }
    //file process
    else{
      char* fname = argv[i];
      filename[filecount] = i;
      // printf("file name is %s\n",fname);
      //read file
      fp = fopen(fname,"r");
      if(fp == NULL){
        printf("wc: %s: open: No such file or directory\n",fname);
        return 1;
      }
      //char process,do counting
      while( (ch = fgetc(fp))!= EOF){
        t_charcount++;
        charcount[filecount]++;
        if(isspace(ch)){
          isin_space = 1;
          if(ch == '\n'){
            linecount[filecount]++;
            t_linecount++;
          }
        }
        else{
          wordcount[filecount] +=isin_space;
          t_wordcount += isin_space;
          isin_space = 0;
        }
      }
      if(wordcount[filecount] > maxcount)
        maxcount = wordcount[filecount];

      filecount++;
    }
  }

  // reporting
  for(i = 0; i < filecount; i++){
    if(options[LINE])
      printf("%6lu ", linecount[i] );
    if(options[WORD])
      printf("%6lu ", wordcount[i] );
    if(options[CHARACTER])
      printf("%6lu ", charcount[i] );
    printf("%s\n", argv[filename[i]]);
  }
  if(filecount > 1){
    printf("%d %d %d total\n", t_linecount,t_wordcount, t_charcount );
  }


  regfree(&reg);

  return 0;
}