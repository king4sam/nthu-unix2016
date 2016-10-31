#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _XOPEN_SOURCE
#define _GNU_SOURCE
#include <unistd.h>
#include <crypt.h>

#define DIC_NAME "john.txt"
#define MAX_WORD 20
#define NUM_WORDS 3200

char* to_guess = "$6$naIJPKfO$SMkeSkFM36M6u3mZIyf2hAr31WxuYtoTwLMjF9Fv49cprYPKtR1K88Ox5xvQdLdoBrAOmCnomRvaHc7VDiqQ0";


int main(int argc, char **argv){
  FILE* dic_f = fopen(DIC_NAME,"r");
  char* tar = strdup(to_guess);
  char *tmpstr = NULL;
  char* algo_no;
  char* salt;
  char* cryptstr;

  algo_no = strtok(tar, "$");
  printf("algo_no %s\n", algo_no);
  salt = strtok(NULL, "$");
  printf("salt %s\n", salt);
  cryptstr = strtok(NULL, "$");
  printf("cryptstr %s\n", cryptstr);
  char* salt_str = malloc(sizeof(char) * (strlen(salt)+4));
  sprintf (salt_str,"$%s$%s",algo_no,salt);
  printf("saltstr %s\n",salt_str);

  char** words =malloc(sizeof(char*) * NUM_WORDS);
  for (int i = 0; i < NUM_WORDS; i++){
    words[i] = (char * )malloc(sizeof(char) * MAX_WORD);
  }

  printf("end init\n");
  int max_word = 0;
  int linecount = 0;
  while(fgets(words[linecount],MAX_WORD,dic_f) != NULL){
    words[linecount][strlen(words[linecount])-1] = '\0';//remove \n
    // printf("line : %s\n", words[linecount]);
    linecount++;
  }

  char guessword[MAX_WORD*3];
  char* g_crypt;
  char * cs;
  int lengs[3];
  // linecount = 10;
  for (int i = 0; i < 5; i++){
    printf("i %d\n", i);
    guessword[0] = '\0';
    strcpy(guessword,words[i]);
    lengs[0] = strlen(words[i]);
    for (int j = 0; j < linecount; j++){
      printf("j %d\n", j);
      guessword[lengs[0]] = '\0';
      strcat(guessword,words[j]);
      lengs[1] = strlen(words[j]);
      for (int k = 0; k < linecount; k++){
        guessword[lengs[0]+lengs[1]] = '\0';
        strcat(guessword,words[k]);
        // g_crypt = crypt_r(guessword,salt_str,&cdata);
        // printf("guess : %s%%%%\n", guessword);
        // printf("after crypt : %s \n",g_crypt);
        // strtok(g_crypt, "$");
        // strtok(NULL, "$");
        // cs = strtok(NULL, "$");
        // if(strcmp(cs,cryptstr) == 0)
        //   printf("hit , pwd is %s\n", guessword);
        // crypt(guessword,salt_str);

        // printf("after crypt : %s \n",cs);
        // printf("guess %s\n", guessword);
      }
    }
  }

  return 0;
}