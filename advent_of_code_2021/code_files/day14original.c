//Although I use memoization for this problem it's not that much better
//than using brute force method time wise
//It's slightly better space wise, but overall I could not solve for part2 with this approach

#include "header.h"

typedef struct treeNode{
  char* input;
  char** output;
  struct treeNode* left;
  struct treeNode* right;
} treeNode;

treeNode* mainTree = NULL;
char* characterList = NULL;

treeNode* createTreeNode(char* input, char* output, int steps){
  treeNode* temp = (treeNode*) calloc(1, sizeof(treeNode));
  temp -> input = (char*) calloc(strlen(input) + 1, sizeof(char));
  temp -> input = strncat(temp -> input, input, strlen(input));
  temp -> output = (char**) calloc(steps, sizeof(char*));
  temp -> output[0] = (char*) calloc(strlen(output) + 1, sizeof(char));
  temp -> output[0] = strncat(temp -> output[0], output, strlen(output));
  return temp;
}

treeNode* insertTreeNode(treeNode* head, treeNode* node){
  if (head == NULL)
    return node;

  if (!strcmp(head -> input, node -> input))
    free(node);
  else if ( strcmp(head -> input, node -> input) < 0)
    head -> right = insertTreeNode(head -> right, node);
  else 
    head -> left = insertTreeNode(head -> left, node);
  return head;
}

treeNode* insertBaseRuleSet(treeNode* head, char* input, char output, int steps){
  char* tempInput = (char*) malloc(3 * sizeof(char));
  tempInput = strncpy(tempInput, input, 2 * sizeof(char));
  char* tempOutput = (char*) malloc(4 * sizeof(char));
  tempOutput[0] = input[0];
  tempOutput[1] = output;
  tempOutput[2] = input[1];
  tempOutput[3] = '\0';
  treeNode* tempNode = createTreeNode(tempInput, tempOutput, steps);
  head = insertTreeNode(head, tempNode);
  return head;
}

char* fileReadandCreateRule(char *filePath, int steps){
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

  while ( (seek = read(fd, buf, 1)) > 0){
    char* word = (char*) malloc(10 * sizeof(char));
    while(strncmp(buf, "\n", sizeof(char)) != 0){
      word = strncat(word, buf, sizeof(char));
      seek = read(fd, buf, 1);
    }
    mainTree = insertBaseRuleSet(mainTree, word, word[strlen(word) - 1], steps);
    free(word);
  }
  return baseTemplate;
}

char* join(const char* str1, const char* str2){
  char* temp;

  if(str2 == NULL){
    temp = calloc(strlen(str1) + 1, sizeof(char));
    temp = strcpy(temp, str1);
  }
  else{
    temp = calloc(strlen(str1) + strlen(str2), sizeof(char));
    temp = strcat(temp, str1);
    temp = strcat(temp, str2 + 1);
  }

  return temp;
}

treeNode* searchBST(treeNode* head, const char* input){
  if (head == NULL) return NULL;
  
  int comp = strcmp(head -> input, input);
  if (!comp)
    return head;
  if (comp < 0)
    return searchBST(head -> right, input);
  return searchBST(head -> left, input);
}

//Checks if output is stored in mainTree, if not calculates the output for 1 step,
//breaks it into two parts and calls itself again
char* eval(char* input, int steps){
  treeNode* relevantNode = searchBST(mainTree, input);
  char* storedOutput = relevantNode -> output[steps - 1];
  if (storedOutput)
    return storedOutput;
  const char* baseOutput = relevantNode -> output[0];

  char* part1 = calloc(3, sizeof(char));
  part1 = strncpy(part1, baseOutput, 2);
  char* part2 = calloc(3, sizeof(char));
  part2 = strncpy(part2, baseOutput + 1, 2);

  char* output = join(eval(part1, steps - 1), eval(part2, steps - 1));
  relevantNode -> output[steps - 1] = output;
  return output;
}

//Breaks the input into two-lettered chunks and then calls eval()
//It's essential since mainTree stores two-lettred inputs only
char* breakAndEval(char* template, int steps){
  if (strlen(template) == 2) return eval(template, steps);
  char* miniInput = calloc(3, sizeof(char));
  miniInput = strncpy(miniInput, template, 2);
  return join(eval(miniInput, steps), breakAndEval(template + 1, steps));
}

//Gets all characters possible from given rules
//Then setUniqueChars() calculates the number of uniqueChars
char* baseCharList(treeNode* head, char* countYet){
  if (head == NULL) return countYet;
  if (countYet == NULL)
    countYet = calloc(3, sizeof(char));
  else
    countYet = realloc(countYet, strlen(countYet) + 3);
  countYet = strcat(countYet, head -> input);
  countYet = baseCharList(head -> left, countYet);
  countYet = baseCharList(head -> right, countYet);
  return countYet;
}

int setUniqueChars(){
  char* template = baseCharList(mainTree, NULL);
  int uniqueChars = 1;
  characterList = calloc(uniqueChars, sizeof(char));
  characterList = strncat(characterList, template, 1);


  for (int i = 1; i < strlen(template); i++){
    int appears = 0;
    for (int j = 0; j < uniqueChars; j++){
      if (characterList[j] == template[i]){
        appears = 1;
        break;
      }
    }

    if (!appears){
      uniqueChars++;
      characterList = realloc(characterList, uniqueChars);
      characterList[uniqueChars - 1] = template[i];
    }
  }

  return uniqueChars;
}

int main(int argc, char** argv){
  if (argc < 2) return 0;
  int steps = atoi(argv[1]);
  char* baseTemplate = fileReadandCreateRule("../input_files/day14input.txt", steps);

  char* template = breakAndEval(baseTemplate, steps);

  int uniqueChars = setUniqueChars();
  int* uniqueCharsFreq = calloc(uniqueChars, sizeof(int));

  for(int i = 0; i < uniqueChars; i++){
    for(int j = 0; j < strlen(template); j++){
      if (characterList[i] == template[j])
        uniqueCharsFreq[i]++;
    }
  }
  int maxFreq = maxList(uniqueCharsFreq, uniqueChars);
  int minFreq = minList(uniqueCharsFreq, uniqueChars);

  printf("%d\n", maxFreq - minFreq);

  return 0;
}
