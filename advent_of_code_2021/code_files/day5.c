#include "header.h"

//This time we go with arrays as we only need to store them
int** ranges = NULL;
int rangeCounter = 0;
int length = 0, width = 0;
int** frequencyGraph = NULL;

void fillArrays(){
  int seek;
  char* buf = (char*)malloc(sizeof(char));;
  int fd = open("../input_files/day5input.txt", O_RDONLY);
  //Initial size is set to 8. No reason
  int size = 8;

  int pos;
  ranges = (int**) malloc(size * sizeof(int*));
  char* word;
  while ( (seek = read(fd, buf, 1)) > 0){
    ranges[rangeCounter] = (int*) malloc(4 * sizeof(int));
    pos = 0;
    //Some undefined behaviour happens when I don't free the memory before allocating it
    //when the size reaches 256 (dynamically)
    free(word);
    word = (char*) malloc(sizeof(char) * 10);
    while (strncmp(buf, "\n", sizeof(char)) != 0){
      if (strncmp(buf, ",", 1) == 0){
        ranges[rangeCounter][pos] = atoi(word);
        free(word);
        word = (char*)malloc(10 * sizeof(char));
        pos++;
      }
      else if (strncmp(buf, "-", 1) == 0){
        ranges[rangeCounter][pos] = atoi(word);
        free(word);
        word = (char*)malloc(10 * sizeof(char));
        pos++;
      }
      else if (strncmp(buf, " ", 1) == 0 || strncmp(buf, ">", 1) == 0){
      }
      else{
        word = strncat(word, buf, 1);
      }
      seek = read(fd, buf, 1);
    }
    ranges[rangeCounter][pos] = atoi(word);
    rangeCounter++;
    if (rangeCounter == size){
      size = size * 2;
      ranges = (int**) realloc(ranges, size * sizeof(int*));
    }
  }
  close(fd);
  //finding out the maximum ranges of values
  for(int i = 0; i < rangeCounter; i++){
    if (ranges[i][0] > length)
      length = ranges[i][0];
    if (ranges[i][2] > length)
      length = ranges[i][2];
    if (ranges[i][1] > width)
      width = ranges[i][1];
    if (ranges[i][3] > width)
      width = ranges[i][3];
  }
  length++;
  width++;
}

void fillFrequencyGraph(char* mode){
  frequencyGraph = (int**) malloc(length * sizeof(int*));
  int i, start, end, j;
  int x1, x2, xAdd, y1, y2, yAdd, xTemp, yTemp;
  for(i = 0; i < length; i++)
    frequencyGraph[i] = (int*) calloc(width, sizeof(int));
  for(i = 0; i < rangeCounter; i++){
    x1 = ranges[i][0];
    x2 = ranges[i][2];
    y1 = ranges[i][1];
    y2 = ranges[i][3];
    if (x2 > x1) xAdd = 1;
    else if (x1 > x2) xAdd = -1;
    else xAdd = 0;
    if (y2 > y1) yAdd = 1;
    else if (y1 > y2) yAdd = -1;
    else yAdd = 0;
    xTemp = x1;
    yTemp = y1;
    int isDiag = (xAdd != 0) && (yAdd != 0);
    int needDiag = strncmp (mode, "diag", 4) == 0;
    if (isDiag && !needDiag) continue;
    while( (xTemp != x2) || (yTemp != y2) ){
      frequencyGraph[xTemp][yTemp]++;
      xTemp += xAdd;
      yTemp += yAdd;
    }
    frequencyGraph[x2][y2]++;
  }
}

int countSafePoints(){
  int counter = 0;
  for(int i = 0; i < length; i++){
    for(int j = 0; j < width; j++){
      if (frequencyGraph[i][j] > 1){
        counter++;
      }
    }
  }
  return counter;
}

int main(int argc, char* argv[]){
  printf("okay\n");
  fillArrays();
  fillFrequencyGraph(argv[1]);
  printf("answer is %d\n", countSafePoints());
  return 0;
}
