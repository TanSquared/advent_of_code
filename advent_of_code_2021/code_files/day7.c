#include "header.h"

int* crabs = NULL;
int crabCounter = 0;
int size = 8;

void fillCrabArray(){
  int seek;
  char* buf = (char*)malloc(sizeof(char));;
  char* word = (char*) malloc(10 * sizeof(char));
  int fd = open("../input_files/day7input.txt", O_RDONLY);
  crabs = (int*) malloc(size * sizeof(int));

  while ( (seek = read(fd, buf, 1)) > 0){
    if (crabCounter == size){
      size = size * 2;
      crabs = (int*) realloc(crabs, size * sizeof(int));
    }
    if (strncmp(buf, ",", 1) == 0){
      crabs[crabCounter] = atoi(word);
      free(word);
      word = (char*)malloc(10 * sizeof(char));
      crabCounter++;
    }
    else
      word = strncat(word, buf, 1);
  }
  crabs[crabCounter] = atoi(word);
  crabCounter++;
  free(word);
  close(fd);
}

int costCalculate(int part){
  int minCrabPos = minList(crabs, crabCounter);
  int maxCrabPos = maxList(crabs, crabCounter);
  int movement, cost, least = -1;
  for(int i = minCrabPos; i <= maxCrabPos; i++){
    cost = 0;
    for(int j = 0; j < crabCounter; j++){
      movement = abs(crabs[j] - i);
      if (part == 1)
        cost = movement + cost;
      else
        //for part two the cost adjustment was like this:
        //For first step moved the cost is one, for second the cost is three, third cost is four
        //e.g. for movement of 3 the cost would become 1 + 2 + 3 = 6
        cost = (movement * (movement + 1))/2 + cost;
    }
    if ((least < 0) || (least > cost))
      least = cost;
  }
  return least;
}

int main(){
  fillCrabArray();
  printf("%d\n", costCalculate(1));
  printf("%d\n", costCalculate(2));
  return 0;
}
