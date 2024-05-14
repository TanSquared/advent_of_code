#include "header.h"

typedef struct point{
  int row;
  int column;
  int value;
} point;

int** graph = NULL;
int flashes = 0;

typedef struct pointNode{
  point point;
  struct pointNode* next;
} pointNode;

pointNode* enqueue(point inputPoint, pointNode* queue){
  pointNode* newNode = (pointNode*) malloc(sizeof(pointNode));
  newNode -> point = inputPoint;
  newNode -> next = NULL;
  //if (!queue) return newNode;
  if (queue == NULL) return newNode;
  pointNode* temp = queue;
  while(temp -> next)
    temp = temp -> next;
  temp -> next = newNode;
  return queue;
}

point createPoint(int i, int j){
  point ret = {i, j, graph[i][j]};
  return ret;
}

pointNode* checkNeighbourAndEval(int row, int column, pointNode* queue){
  //Assumption is that grid is always 10 * 10
  if (row < 0 || row > 9 || column < 0 || column > 9) return queue;
  //If index value is zero we don't increase it (it has recently flashed)
  if (!graph[row][column]) return queue;
  graph[row][column]++;
  //Ensures that each point is added only once to the queue
  if (graph[row][column] == 10)
    queue = enqueue(createPoint(row, column), queue);
  return queue;
}

pointNode* evalQueue(pointNode* queue){
  point headPoint = queue -> point;
  int row = headPoint.row;
  int column = headPoint.column;
  for(int i = -1; i < 2; i++){
    for(int j = -1; j < 2; j++){
      if (i == 0 && j == 0) continue;
      queue = checkNeighbourAndEval(row + i, column + j, queue);
    }
  }
  pointNode* nextNode = queue -> next;
  graph[row][column] = 0;
  flashes++;
  free(queue);
  return nextNode;
}

void flashCalculator(int loopNum){
  int i, j;
  pointNode *queue;
  int prevFlash;
  //So if loopNum is negative it would keep running till
  //state of simultaneous flashes are achieved
  for (int x = 0; x != loopNum; x++){
    prevFlash = flashes;
    queue = NULL;
    for(i = 0; i < 10; i++){
      for(j = 0; j < 10; j++){
        graph[i][j]++;
        if (graph[i][j] == 10){
          point newPoint = createPoint(i, j);
          queue = enqueue(newPoint, queue);
        }
      }
    }

    while(queue)
      queue = evalQueue(queue);

    if (flashes - prevFlash == 100){
      printf("simultaneous flash is at %d steps\n", x + 1);
      return;
    }
  }
}

void readFromFileIntoGraph(){
  int fd = open("../input_files/day11input.txt", O_RDONLY);
  char* buf = (char*) malloc(sizeof(char));
  int i, j;
  graph = (int**) malloc(10 * sizeof(int*));
  for(i = 0; i < 10; i++){
    graph[i] = (int*) malloc(10 * sizeof(int));
    for(j = 0; j < 10; j++){
      read(fd, buf, 1);
      graph[i][j] = atoi(buf);
    }
    lseek(fd, 1, SEEK_CUR);
  }
  close(fd);
}

int main(){
  readFromFileIntoGraph();
  int turns;
  //Enter a positive number for part 1. Negative for part2
  scanf("%d", &turns);
  flashCalculator(turns);
  printf("flashes are %d\n", flashes);

  return 0;
}
