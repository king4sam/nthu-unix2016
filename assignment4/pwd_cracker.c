#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _XOPEN_SOURCE
#define _GNU_SOURCE
#include <unistd.h>
#include <crypt.h>

#define MAX_WORD 20
#define NUM_WORDS 34

const char* to_guess = "$6$naIJPKfO$SMkeSkFM36M6u3mZIyf2hAtt31WxuYtoTwLMjF9Fv49cprYPKtR1K88Ox5xvQdLdoBrAOmCnomRvaHc7VDiqQ0";


int main(int argc, char **argv){

  if(argc < 2){
    fprintf(stderr, "usage: ./decrypt <DICTIONARY FILE NAME>\n");
  }


  FILE* dic_f = fopen(argv[1],"r");
  char* tar = strdup(to_guess);
  char *tmpstr = NULL;
  char* algo_no;
  char* salt;
  char* cryptstr;

  algo_no = strtok(tar, "$");
  salt = strtok(NULL, "$");
  cryptstr = strtok(NULL, "$");
  char* salt_str = malloc(sizeof(char) * (strlen(salt)+4));
  sprintf (salt_str,"$%s$%s",algo_no,salt);

  char** words =malloc(sizeof(char*) * NUM_WORDS);
  int word_lengs[NUM_WORDS];
  for (int i = 0; i < NUM_WORDS; i++){
    words[i] = (char * )malloc(sizeof(char) * MAX_WORD);
  }

  int max_word = 0;
  int linecount = 0;
  while(fgets(words[linecount],MAX_WORD,dic_f) != NULL){
    word_lengs[linecount] = strlen(words[linecount])-1;
    words[linecount][word_lengs[linecount]] = '\0';//remove \n
    linecount++;
  }

  char guessword[MAX_WORD*3];
  char* g_crypt;
  char * cs;
  int lengs[3];
  // linecount = 10;
  for (int i = 0; i < linecount; i++){
    printf("i %d\n", i);
    guessword[0] = '\0';
    strcpy(guessword,words[i]);
    lengs[0] = word_lengs[i];
    for (int j = 0; j < linecount; j++){
      // printf("j %d\n", j);
      guessword[lengs[0]] = '\0';
      strcat(guessword,words[j]);
      lengs[1] = word_lengs[j];
      for (int k = 0; k < linecount; k++){
        guessword[lengs[0]+lengs[1]] = '\0';
        strcat(guessword,words[k]);
        g_crypt = crypt(guessword,salt_str);
        // printf("guess : %s\n", guessword);
        // printf("after crypt : %s \n",g_crypt);
        strtok(g_crypt, "$");
        strtok(NULL, "$");
        cs = strtok(NULL, "$");
        if(strcmp(cs,cryptstr) == 0){
          printf("hit , pwd is %s\n", guessword);
          return 0;
        }

        // crypt(guessword,salt_str);

        // printf("after crypt : %s \n",cs);
        // printf("guess %s\n", guessword);
      }
    }
  }

  return 0;
}