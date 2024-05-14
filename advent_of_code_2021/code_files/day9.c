#include "header.h"

int** heightmap = NULL;
int row = 0, column = 0;
int size = 8;
int** attendance = NULL;

typedef struct indicia{
  int row;
  int column;
  int value;
} indicia;

indicia* lowPoints = NULL;
indicia** basins = NULL;

typedef struct indiciaNode{
  indicia index;
  struct indiciaNode* next;
} indiciaNode;

typedef struct queue{
  indiciaNode* head;
  indiciaNode* tail;
} queue;

queue* basinQueue = NULL;

void fillHeightMap(){
  int seek, counter = 0;
  char* buf = (char*)malloc(sizeof(char));
  int fd = open("../input_files/day9input.txt", O_RDONLY);
  //To find out number of columns
  seek = read(fd, buf, 1);
  while ( strncmp(buf, "\n", sizeof(char)) ){
    counter++;
    seek = read(fd, buf, 1);
  }
  lseek(fd, 0, SEEK_SET);
  heightmap = (int**) malloc(size * sizeof(int*));

  while ( (seek = read(fd, buf, 1)) > 0){
    heightmap[row] = (int*) malloc(counter * sizeof(int));
    while (strncmp(buf, "\n", sizeof(char)) != 0){
      heightmap[row][column] = atoi(buf);
      column++;
      seek = read(fd, buf, 1);
    }
    row++;
    if(row == size){
      size = size * 2;
      heightmap = (int**) realloc(heightmap, size * sizeof(int*));
    }
    column = 0;
  }
  close(fd);
  column = counter;
}

//This fills up the lowPoints array as well as returning the number of lowPoints there are
int countLowPoints(){
  int counter = 0;
  lowPoints = (indicia*) malloc(300 * sizeof(indicia));
  int i, j, miniCounter;

  for(i = 0; i < row; i++){
    for(j = 0; j < column; j++){
      miniCounter = 0;
      if (i - 1 < 0)
        miniCounter++;
      else{
        if (heightmap[i][j] < heightmap[i - 1][j])
          miniCounter++;
      }
      if (i + 1 >= row)
        miniCounter++;
      else{
        if (heightmap[i][j] < heightmap[i + 1][j])
          miniCounter++;
      }
      if (j - 1 < 0)
        miniCounter++;
      else{
        if (heightmap[i][j] < heightmap[i][j - 1])
          miniCounter++;
      }
      if (j + 1 >= column)
        miniCounter++;
      else{
        if (heightmap[i][j] < heightmap[i][j + 1])
          miniCounter++;
      }
      if (miniCounter == 4){
        lowPoints[counter].row = i;
        lowPoints[counter].column = j;
        lowPoints[counter].value= heightmap[i][j];
        counter++;
      }
    }
  }
  return counter;
}

//Some queue helper functions

void enqueue(indicia index){
  indiciaNode* temp = (indiciaNode*) malloc(sizeof(indiciaNode));
  temp -> index = index;
  temp -> next = NULL;
  if(basinQueue == NULL){
    basinQueue -> head = temp;
    basinQueue -> tail = temp;
  }
  else{
    basinQueue -> tail -> next = temp;
    basinQueue -> tail = temp;
  }
}

indicia dequeue(){
  indiciaNode* temp = basinQueue -> head;
  if(basinQueue -> head == basinQueue -> tail)
    basinQueue = NULL;
  else
    basinQueue -> head = basinQueue -> head -> next;
  return temp -> index;
}

indicia createIndex(int row, int column){
  int value = heightmap[row][column];
  indicia index =  {row, column, value};
  return index;
}

void fillNeighbours(indicia index){
  int indexRow = index.row;
  int indexColumn = index.column;
  int indexValue = index.value;
  if (indexRow > 0){
    if (heightmap[indexRow - 1][indexColumn] > indexValue)
      enqueue(createIndex(indexRow - 1, indexColumn));
  }
  if (indexRow < row - 1){
    if (heightmap[indexRow + 1][indexColumn] > indexValue)
      enqueue(createIndex(indexRow + 1, indexColumn));
  }
  if (indexColumn > 0){
    if (heightmap[indexRow][indexColumn - 1] > indexValue)
      enqueue(createIndex(indexRow, indexColumn - 1));
  }
  if (indexColumn > column - 1){
    if (heightmap[indexRow][indexColumn + 1] > indexValue)
      enqueue(createIndex(indexRow, indexColumn + 1));
  }
}

void fillBasins(int lowPointCounter){
  basins = (indicia**) malloc(lowPointCounter * sizeof(indicia*));
  int i, j, counter;
  for(i = 0; i < lowPointCounter; i++){
    printf("we are in %d\n", i);
    basins[i] = (indicia*) malloc(300 * sizeof(indicia));
    enqueue(lowPoints[i]);
    counter = 0;
    while(basinQueue){
      indicia index = basinQueue -> head -> index;
      fillNeighbours(index);
      dequeue();
      attendance[index.row][index.column] = 1;
      basins[i][counter] = index;
      counter++;
    }
    for(j = 0; j < counter; j++){
      printf("%d", basins[i][counter].row);
      printf("%d", basins[i][counter].column);
      printf("%d\t", basins[i][counter].value);
    }
    printf("\n");
  }
  printf("\n");
}

int main(){
  fillHeightMap();
  int i;
  int counter = countLowPoints();
  int riskValue = counter;
  for(i = 0; i < counter; i++)
    riskValue = riskValue + lowPoints[i].value;
  printf("%d\n", riskValue);

  attendance = (int**)calloc(row, sizeof(int*));
  for(i = 0; i <column; i++)
    attendance[i] = (int*)calloc(column, sizeof(int));
  fillBasins(counter);
  return 0;
}
