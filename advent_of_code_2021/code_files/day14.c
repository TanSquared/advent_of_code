//This solution is inspired by
//https://www.reddit.com/r/adventofcode/comments/rfzq6f/comment/i5vq9n2/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
#include "header.h"

typedef struct keyValue{
  char* key;
  char letterToAdd;
} keyValue;

int uniqueChars = 0, rules = 0;
char* uniqueCharsList = NULL;
long unsigned int* uniqueCharsFreq = NULL;
char** pairs = NULL;
long unsigned int* pairFreq = NULL;
char** ruleSetKeys = NULL;
char* ruleSetValues = NULL;

void insertBaseRuleSet(const char* str){
  rules++;
  int i = rules - 1;
  ruleSetKeys[i] = calloc(3, sizeof(char));
  ruleSetKeys[i] = strncpy(ruleSetKeys[i], str, 2);
  ruleSetValues[i] = str[strlen(str) - 1];
  return;
}

char* fileReadandCreateRule(char *filePath){
  int fd = open(filePath, O_RDONLY);
  char* buf = (char*)malloc(sizeof(char));;
  char* word = (char*) malloc(30 * sizeof(char));
  int seek;
  while(strncmp(buf, "\n", sizeof(char)) != 0){
    word = strncat(word, buf, sizeof(char));
    read(fd, buf, 1);
  }
  int initialSize = strlen(word);
  char* baseTemplate = (char*) calloc(initialSize + 1, sizeof(char));
  baseTemplate = strncpy(baseTemplate, word, initialSize * sizeof(char));

  lseek(fd, 1, SEEK_CUR);

  //Randomly assigning it a size of 100
  ruleSetKeys = calloc(100, sizeof(char*));
  ruleSetValues = calloc(100, sizeof(int));
  while ( (seek = read(fd, buf, 1)) > 0){
    char* word = (char*) malloc(10 * sizeof(char));
    while(strncmp(buf, "\n", 1) != 0){
      word = strncat(word, buf, 1);
      seek = read(fd, buf, 1);
    }
    insertBaseRuleSet(word);
    free(word);
  }
  return baseTemplate;
}

//Gets all characters possible from given rules
//Then setUniqueCharsList() calculates the number of uniqueChars
char* baseCharList(){
  char* template = calloc(rules * 2 + 1, sizeof(char));
  for(int i = 0; i < rules; i++)
    template = strncat(template, ruleSetKeys[i], 2);
  return template;
}

void setUniqueCharsList(){
  char* template = baseCharList();
  uniqueChars = 1;
  uniqueCharsList = calloc(uniqueChars, sizeof(char));
  uniqueCharsList = strncat(uniqueCharsList, template, 1);


  for (int i = 1; i < strlen(template); i++){
    int appears = 0;
    for (int j = 0; j < uniqueChars; j++){
      if (uniqueCharsList[j] == template[i]){
        appears = 1;
        break;
      }
    }

    if (!appears){
      uniqueChars++;
      uniqueCharsList = realloc(uniqueCharsList, uniqueChars);
      uniqueCharsList[uniqueChars - 1] = template[i];
    }
  }

  return;
}

//Sets frequency of each unique characters from the base template
void setUniqueCharsFreq(char* template){
  uniqueCharsFreq = calloc(uniqueChars, sizeof(long unsigned int));
  for(int i = 0; i < strlen(template); i++){
    for(int j = 0; j < uniqueChars; j++){
      if (template[i] == uniqueCharsList[j])
        uniqueCharsFreq[j]++;
    }
  }
  return;
}

//Sets frequency of each pair from the base template
void setPairs(char* template){
  pairs = calloc(rules, sizeof(char*));
  pairFreq = calloc(rules, sizeof(long unsigned int));
  int i;

  for(i = 0; i < rules; i++){
    pairs[i] = calloc(3, sizeof(char));
    pairs[i] = strncpy(pairs[i], ruleSetKeys[i], 2);
  }

  for(i = 0; i < strlen(template) - 1; i++){
    for(int j = 0; j < rules; j++){
      char input[] = {template[i], template[i + 1], '\0'};
      if (strncmp(pairs[j], input, 2) == 0)
        pairFreq[j]++;
    }
  }
  return;
}

//calculates frequency of all characters and pairs for each step
//suppose NN gives C
//So we remove frequency of pair NN and add it to NC and CN
//and add the frequency of letter C by that amount
void eval(){
  char** newPairs = calloc(rules, sizeof(char*));
  long unsigned int* newPairsFreq = calloc(rules, sizeof(long unsigned int));
  long unsigned int value;
  int i, j;
  for(i = 0; i < rules; i++){
    newPairs[i] = calloc(3, sizeof(char));
    newPairs[i] = strncpy(newPairs[i], pairs[i], 2);
  }

  for(i = 0; i < rules; i++){
    value = pairFreq[i];
    char newPair1[] = {ruleSetKeys[i][0], ruleSetValues[i], '\0'};
    char newPair2[] = {ruleSetValues[i], ruleSetKeys[i][1], '\0'};
    for(j = 0; j < rules; j++){
      if (! strcmp(newPair1, ruleSetKeys[j]))
        newPairsFreq[j] += value;
      if (! strcmp(newPair2, ruleSetKeys[j]))
        newPairsFreq[j] += value;
    }
    for(j = 0; j < uniqueChars; j++){
      if (uniqueCharsList[j] == ruleSetValues[i])
        uniqueCharsFreq[j] += value;
    }
  }
  free(pairs);
  free(pairFreq);
  pairs = newPairs;
  pairFreq = newPairsFreq;
  return;
}

int main(int argc, char** argv){
  if (argc < 2) return 0;
  int steps = atoi(argv[1]);
  char* baseTemplate = fileReadandCreateRule("../input_files/day14input.txt");

  setUniqueCharsList();
  setUniqueCharsFreq(baseTemplate);
  setPairs(baseTemplate);

  for(int s = 0; s < steps; s++)
    eval();

  long unsigned int maxFreq = maxListLU(uniqueCharsFreq, uniqueChars);
  long unsigned int minFreq = minListLU(uniqueCharsFreq, uniqueChars);

  printf("%lu\n", maxFreq - minFreq);

  return 0;
}
