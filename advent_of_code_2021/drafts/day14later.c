//This is left for me to debug some day later
//I am getting segmentation fault and I can't figure out why

#include "header.h"

typedef struct treeNode{
  char* input;
  char* output;
  struct treeNode* left;
  struct treeNode* right;
} treeNode;

treeNode** ruleSet = NULL;
long unsigned int finalTemplateSize;

long unsigned int totalAlphabets(int steps, int initialSize){
  long unsigned int size = initialSize;
  for(int i = 0; i < steps; i++){
    size = 2 * size - 1;
  }
  return size;
}

treeNode* createTreeNode(char* input, char* output){
  treeNode* temp = (treeNode*) calloc(1, sizeof(treeNode));
  temp -> input = (char*) calloc(strlen(input), sizeof(char));
  temp -> input = strncat(temp -> input, input, strlen(input));
  temp -> output = (char*) calloc(strlen(output), sizeof(char));
  temp -> output = strncat(temp -> output, output, strlen(output));
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

void printTreeNode(treeNode* head){
  if(head == NULL) return;
  printTreeNode(head -> left);
  printf("%s -> %s\n", head -> input, head -> output);
  printTreeNode(head -> right);
}

treeNode* insertBaseRuleSet(treeNode* head, char* input, char output){
  char* tempInput = (char*) malloc(2 * sizeof(char));
  tempInput = strncpy(tempInput, input, 2 * sizeof(char));
  char* tempOutput = (char*) malloc(3 * sizeof(char));
  tempOutput[0] = input[0];
  tempOutput[1] = output;
  tempOutput[2] = input[1];
  treeNode* tempNode = createTreeNode(tempInput, tempOutput);
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
  char* baseTemplate = (char*) malloc(initialSize * sizeof(char));
  baseTemplate = strncpy(baseTemplate, word, initialSize * sizeof(char));

  finalTemplateSize = totalAlphabets(steps, initialSize);
  printf("finalTemplateSize is %lu\n", finalTemplateSize);
  ruleSet = (treeNode**) calloc(finalTemplateSize, sizeof(treeNode*));
  lseek(fd, 1, SEEK_CUR);

  while ( (seek = read(fd, buf, 1)) > 0){
    char* word = (char*) malloc(10 * sizeof(char));
    while(strncmp(buf, "\n", sizeof(char)) != 0){
      word = strncat(word, buf, sizeof(char));
      seek = read(fd, buf, 1);
    }
  printf("works until here\n");
    ruleSet[1] = insertBaseRuleSet(ruleSet[1], word, word[strlen(word) - 1]);
    free(word);
  }
  printTreeNode(ruleSet[1]);
  return baseTemplate;
}

char* join(char* str1, char* str2, char* str3){
  char* temp = NULL;

  if ( (str1 == NULL) && (str3 == NULL) ){
    temp = (char*) calloc (strlen(str2), sizeof(char));
    temp = strncat(temp, str2, strlen(str2));
  }

  else if (str3 == NULL){
    temp = (char*) calloc ( (strlen(str1) + strlen(str2) - 1), sizeof(char));
    temp = strncat(temp, str1, strlen(str1));
    temp = strncat(temp, (str2 + 1), strlen(str2) - 1);
  }

  else if (str1 == NULL){
    temp = (char*) calloc ( (strlen(str2) + strlen(str3) - 1), sizeof(char));
    temp = strncat(temp, str2, strlen(str2));
    temp = strncat(temp, (str3 + 1), strlen(str3) - 1);
  }

  else{
    temp = (char*) calloc( (strlen(str1) + strlen(str2) + strlen(str3) - 2), sizeof(char));
    temp = strncat(temp, str1, strlen(str1));
    temp = strncat(temp, (str2 + 1), strlen(str2) - 1);
    temp = strncat(temp, (str3 + 1), strlen(str3) - 1);
  }
  return temp;
}

char* searchBST(treeNode* head, char* input){
  if (head == NULL) return NULL;
  
  int comp = strcmp(head -> input, input);
  if (!comp) return head -> output;
  if (comp < 0) return searchBST(head -> right, input);
  else return searchBST(head -> left, input);
}

char* evalAndStore(char* template, int size){
  if (template == NULL) return NULL;

  int strSize = strlen(template);
  if (size > strSize)
    return evalAndStore(template, strSize);

  int found = 0;
  int i;
  int steps = strSize + 1 - size;
  char* rightLeft = NULL, *leftLeft = NULL;
  char* miniOuput, *output, *input;

  for(i = 0; i < steps; i++){
    input = (char*) calloc(size, sizeof(char));
    input = strncat(input, template + i, size);
    if ( (miniOuput = searchBST(ruleSet[size - 1], input)) != NULL){
      found = 1;
      break;
    }
    free(input);
  }

  if (found){
    if (i > 0){
      leftLeft = (char*) calloc(i + 1, sizeof(char));
      leftLeft = strncat(leftLeft, template, i + 1);
    }
    if (i < steps - 1){
      rightLeft = (char*) calloc(strSize - i - size + 1, sizeof(char));
      rightLeft = strncat(rightLeft, template + i + size - 1, strSize - i - size + 1);
    }

    output = join(evalAndStore(leftLeft, size), miniOuput, evalAndStore(rightLeft, size));
  }

  else{
    output = evalAndStore(template, size - 1);
  }

  //Now the storing part
  //I am just reusing the variables. Don't mind
  if (size == 2) return output;

  /*
  strSize = totalAlphabets(1, size);
  found = i;
  for(i = 0; i < steps; i++){
    if (i != found){
      miniOuput = (char*) calloc(strSize, sizeof(char));
      input = (char*)calloc(size, sizeof(char));
      miniOuput = strncat(miniOuput, output + 2 * i, strSize);
      input = strncat(input, template + i, size);
      treeNode* temp = createTreeNode(input, miniOuput);
      ruleSet[size - 1] = insertTreeNode(ruleSet[size - 1], temp);
      //free(miniOuput);
      //free(input);
    }
  }
  */
  return output;
}

int main(int argc, char** argv){
  if (argc < 2) return 0;
  int steps = atoi(argv[1]);
  char* baseTemplate = fileReadandCreateRule("../input_files/day14input.txt", steps);

  char* template = baseTemplate;
  for(int i = 0; i < steps; i++){
    template = evalAndStore(template, strlen(template));
  }
  printf("%s\n", template);
  printf("NBBNBNBBCCNBCNCCNBBNBBNBBBNBBNBBCBHCBHHNHCBBCBHCB\n");
  return 0;
}
