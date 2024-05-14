#include "header.h"

char** syntaxArr = NULL;
int lines = 0;
charNode* stack = NULL;
char* openingBrackets = "([{<";
char* closingBrackets = ")]}>";
int scorePart1[] = {3, 57, 1197, 25137};
int scorePart2[] = {1, 2, 3, 4};

void fillSyntaxArr(){
  int seek;
  int size = 8;
  char* buf = (char*)malloc(sizeof(char));
  int fd = open("../input_files/day10input.txt", O_RDONLY);
  syntaxArr = (char**) malloc(size * sizeof(char*));

  while ( (seek = read(fd, buf, 1)) > 0){
    syntaxArr[lines] = (char*) calloc(201, sizeof(char));
    while (strncmp(buf, "\n", sizeof(char)) != 0){
      syntaxArr[lines] = strncat(syntaxArr[lines], buf, sizeof(char));
      seek = read(fd, buf, 1);
    }
    lines++;
    if(lines == size){
      size = size * 2;
      syntaxArr = (char**) realloc(syntaxArr, size * sizeof(char*));
    }
  }
  close(fd);
}

charNode* insertChar(charNode* head, char character){
  charNode* newNode = (charNode*) malloc(sizeof(charNode));
  newNode -> data = character;
  newNode -> next = head;
  return newNode;
}

charNode* pop(charNode* head){
  if (head == NULL) return NULL;
  charNode* temp = head -> next;
  if (temp == NULL) return NULL;
  free(head);
  return temp;
}

int existsClosingBracket(char character){
  for(int i = 0; i < 4; i++){
    if (character == closingBrackets[i])
      return i;
  }
  return -1;
}

void printStack(){
  charNode* temp = stack;
  while(temp){
    printf("%c, ", temp -> data);
    temp = temp -> next;
  }
  printf("\n");
}

//This does 2 things.
//If the string is corrupt, it throws back the error score 
//If it's incomplete, it keeps them in the stack and returns 0 so that incompleteBracketsScore() can then calculate on the stack
int findCulprit(char* arr){
  free(stack);
  stack = NULL;
  int comparison = 0;
  if ( (comparison = existsClosingBracket(arr[0])) >= 0){
    return scorePart1[comparison];
  }
  stack = insertChar(stack, arr[0]);
  for(int i = 1; i < strlen(arr); i++){
    if ( (comparison = existsClosingBracket(arr[i])) >= 0){
      if (stack -> data == openingBrackets[comparison])
        stack = pop(stack);
      else{
        return scorePart1[comparison];
      }
    }
    else
      stack = insertChar(stack, arr[i]);
  }
  return 0;
}

long unsigned int incompleteBracketsScore(){
  long unsigned int ans = 0;
  char character;
  int index;
  charNode* temp = stack;
  while(temp){
    character = temp -> data;
    for(int i = 0; i < 4; i++){
      if (character == openingBrackets[i])
        index = i;
    }
    ans = ans * 5 + scorePart2[index];
    temp = temp -> next;
  }
  return ans;
}

long unsigned int* sort(long unsigned int* arr, int len){
  for(int i = 0; i < len; i++){
    for(int j = i + 1; j < len; j++){
      if (arr[i] > arr [j]){
        arr[i] = arr[i] + arr[j];
        arr[j] = arr[i] - arr[j];
        arr[i] = arr[i] - arr[j];
      }
    }
  }
  return arr;
}

int main(){
  fillSyntaxArr();
  int i;
  int part1 = 0;
  int corruptBracketScore = 0;
  long unsigned int* incompleteLines = (long unsigned int*) malloc(lines * sizeof(long unsigned int));
  int incompleteLinesCounter = 0;

  for(i = 0; i < lines; i++){
    corruptBracketScore = findCulprit(syntaxArr[i]);
    part1 = part1 + corruptBracketScore;
    if (!corruptBracketScore){
      incompleteLines[incompleteLinesCounter++] = incompleteBracketsScore();
    }
  }

  incompleteLines = sort(incompleteLines, incompleteLinesCounter);

  printf("%d\n", part1);
  printf("%lu\n", incompleteLines[incompleteLinesCounter/2]);
  return 0;
}
