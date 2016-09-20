#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>
#include <unistd.h>
#include <errno.h>

#define LINE 0
#define WORD 1
#define CHARACTER 2

int main (int argc, char **argv){
  int i,isin_space = 1;
  int is_rfile_failed = 0;
  unsigned long *charcount,*linecount,*wordcount,*filename;
  int maxcount;
  int options[3] = {0};// line word character

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
    fprintf(stderr,"There are too few args\n");
    return 1;
  }
  char c;
  while( (c=getopt(argc, argv, "lwc")) != -1 ){
    switch (c){
      case 'l':
        options[LINE] = 1;
        break;
      case 'w':
        options[WORD] = 1;
        break;
      case 'c':
        options[CHARACTER] = 1;
        break;
      default:
        fprintf(stderr,"Try './lwc --help' for more information.\n");
        return 1;
        break;
    }
  }
  for (int i = 1; i < argc; ++i){
    //process as a filename
    if(argv[i][0] != '-'){
      char* fname = argv[i];
      filename[filecount] = i;
      //read file
      fp = fopen(fname,"r");
      if(fp == NULL){
        fprintf(stderr,"%s: %s: %s\n",argv[0],fname,strerror(errno));
        is_rfile_failed = 1;
        continue;
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
      fclose(fp);
      filecount++;
    }
  }

  // reporting
  // cal output width
  int outputwidth = 1,maxcc;
  if(filecount > 1){
    maxcc = t_charcount;
    while(maxcc > 10){
      maxcc /= 10;
      outputwidth++;
    }
  }else{
    maxcc = charcount[0];
    while(maxcc > 10){
      maxcc /= 10;
      outputwidth++;
    }
  }
  //set format string
  char out_digit_str[1000];
  sprintf(out_digit_str,"%%%dlu ",outputwidth);

  for(i = 0; i < filecount; i++){
    if(options[LINE])
      printf(out_digit_str, linecount[i] );
    if(options[WORD])
      printf(out_digit_str, wordcount[i] );
    if(options[CHARACTER])
      printf(out_digit_str, charcount[i] );
    printf("%s\n", argv[filename[i]]);
  }
  if(filecount > 1){
    if(options[LINE])
      printf(out_digit_str, t_linecount );
    if(options[WORD])
      printf(out_digit_str, t_wordcount );
    if(options[CHARACTER])
      printf(out_digit_str, t_charcount );
    printf("total\n");
  }

  free(charcount);
  free(linecount);
  free(wordcount);
  free(filename);

  return is_rfile_failed;
}