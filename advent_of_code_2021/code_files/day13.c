#include "header.h"

int **initialGraph, **graph;
int foldCounter = 0, lineSize = 0;
int row, column;
int firstInstruction;

typedef struct foldInstruction{
  int direction;
  int coordinate;
} foldInstruction;

foldInstruction* foldInstructions;

void readFromFile(char *filePath){
  int fd = open(filePath, O_RDONLY);
  char* buf = (char*) malloc(sizeof(char));
  char* word = (char*) calloc(10, sizeof(char));
  //I like 8 being initial size 
  int size = 8;
  int i;
  initialGraph = (int**) malloc(2 * sizeof(int*));
  initialGraph[0] = (int*) malloc(size * sizeof(int));
  initialGraph[1] = (int*) malloc(size * sizeof(int));
  while( read(fd, buf, sizeof(char))){
    int num;
    if (buf[0] == 'f'){
      size = 8;
      foldInstructions = (foldInstruction*) malloc(size * sizeof(foldInstruction));
      while( read(fd, buf, sizeof(char)) ){
        if (buf[0] == '='){
          if(word[strlen(word) - 1] == 'x')
            foldInstructions[foldCounter].direction = 1;
          else
            foldInstructions[foldCounter].direction = 0;
          free(word);
          word = (char*) calloc(30, sizeof(char));
        }
        else if(buf[0] == '\n'){
          num = atoi(word);
          foldInstructions[foldCounter++].coordinate = num;
          if (foldCounter == size){
            size = size * 2;
            foldInstructions = (foldInstruction*) realloc(foldInstructions, size * sizeof(foldInstruction));
          }
        }
        else
          word = strncat(word, buf, sizeof(char));
      }
    }
    else if (! ((buf[0] == '\n') || (buf[0] == ',')) ){
      word = strncat(word, buf, sizeof(char));
    }
    else{
      num = atoi(word);
      free(word);
      word = (char*) calloc(10, sizeof(char));
      if (buf[0] == '\n'){
        initialGraph[0][lineSize++] = num;
        if (num > row)
          row = num;
        if (lineSize == size){
          size = size * 2;
          initialGraph[0] = (int*) realloc(initialGraph[0], size * sizeof(int));
          initialGraph[1] = (int*) realloc(initialGraph[1], size * sizeof(int));
        }
      }
      else{
        initialGraph[1][lineSize] = num;
        if (num > column)
          column = num;
      }
    }
  }
  close(fd);
  lineSize--;
  row++;
  column++;
  graph = (int**) malloc(row * sizeof(int*));
  for (i = 0; i < row; i++)
    graph[i] = (int*) malloc(column * sizeof(int));
  for (i = 0; i < lineSize; i++){
    graph[initialGraph[0][i]][initialGraph[1][i]] = 1;
  }

}

int foldHorizontal(int coordinate){
  int i, j, diff, diffTwice;
  for (i = 0; i < coordinate; i++){
    diff = coordinate - i;
    diffTwice = 2 * diff;
    for(j = 0; j < column; j++)
      graph[i][j] = graph[i][j] || graph[i + diffTwice][j];
  }
  for(i = coordinate; i < row; i++)
    free(graph[i]);
  row = coordinate;
  int dots = 0;
  for(i = 0; i < row; i++){
    for(j = 0; j < column; j++){
      if (graph[i][j])
        dots++;
    }
  }
  return dots;
}

int foldVertical(int coordinate){
  int i, j, diff, diffTwice;
  for (j = 0; j < coordinate; j++){
    diff = coordinate - j;
    diffTwice = 2 * diff;
    for(i = 0; i < row; i++)
      graph[i][j] = graph[i][j] || graph[i][j + diffTwice];
  }
  column = coordinate;
  for(i = 0; i < row; i++)
    graph[i] = (int *) realloc(graph[i], column * sizeof(int));
  int dots = 0;
  for(i = 0; i < row; i++){
    for(j = 0; j < column; j++){
      if (graph[i][j])
        dots++;
    }
  }
  return dots;
}

int main(){
  readFromFile("../input_files/day13input.txt");
  int k, i, j;
  for (k = 0; k < foldCounter; k++){
    if (foldInstructions[k].direction)
      printf("%d\n", foldVertical(foldInstructions[k].coordinate));
    else
      printf("%d\n", foldHorizontal(foldInstructions[k].coordinate));
  }
  for(i = 0; i < row; i++){
    for(j = 0; j < column; j++){
      if (graph[i][j])
        printf("# ");
      else
        printf("  ");
    }
    printf("\n");
  }
  return 0;
}
