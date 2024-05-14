#include "header.h"

int** graph = NULL;
char** uniquePoints = NULL;
int *bigCaves = NULL;
int counter = 1;

typedef struct pathNode{
  int* path;
  int size;
  int killCount;
  struct pathNode* next;
} pathNode;

pathNode *paths, *queue;
//Just so that managing them would be easier
pathNode *pathsLast, *queuesLast;

//This function ended up being bigger than I wanted
//First we read all the unique points in the file just to get the size of our graph to create
//Then we make a graph out of the connections
void readFromFile(char* filePath){
  int fd = open(filePath, O_RDONLY);
  int i;
  int same = 0;
  int graphCounter = 0;
  char* buf = (char*) malloc(sizeof(char));
  char* word = (char*) calloc(10, sizeof(char));
  uniquePoints = (char**) malloc(100 * sizeof(char*));
  uniquePoints[0] = "start";
  char*** initialGraph = (char***) malloc(200 * sizeof(char**));
  while( read(fd, buf, sizeof(char)) ){
    if (! ((buf[0] == '\n') || (buf[0] == '-')) ){
      word = strncat(word, buf, sizeof(char));
    }
    else{
      same = 0;
      for(i = 0; i < counter; i++){
        if (!strcmp(uniquePoints[i], word)){
          same = 1;
          break;
        }
      }
      if(!same){
        uniquePoints[counter] = (char*) malloc(10 * sizeof(char));
        strcpy(uniquePoints[counter++], word);
      }
      if (buf[0] == '-'){
        initialGraph[graphCounter] = (char**) malloc(2 * sizeof(char*));
        initialGraph[graphCounter][0] = (char*) malloc(10 * sizeof(char));
        strcpy(initialGraph[graphCounter][0], word);
      }
      else{
        initialGraph[graphCounter][1] = (char*) malloc(10 * sizeof(char));
        strcpy(initialGraph[graphCounter++][1], word);
      }
      free(word);
      word = (char*) calloc(10, sizeof(char));
    }
  }
  //"end" should always come at the last. My calculations are based on this assumption
  for(i = 0; i < counter - 1; i++){
    if (! strncmp(uniquePoints[i], "end", 3 * sizeof(char)) ){
      strcpy(uniquePoints[i], uniquePoints[counter - 1]);
      break;
    }
  }
  //"start" should always come at the beginning. My calculations are based on this assumption
  uniquePoints[counter - 1] = "end";
  for(i = 1; i < counter - 1; i++){
    if (! strncmp(uniquePoints[i], "start", 5 * sizeof(char)) ){
      strcpy(uniquePoints[i], uniquePoints[0]);
      break;
    }
  }
  uniquePoints[0] = "start";
  close(fd);

  bigCaves = (int*)calloc(counter, sizeof(int));
  for(i = 1; i < counter - 1; i++){
    if (uniquePoints[i][0] > 64 && uniquePoints[i][0] < 91)
      bigCaves[i] = 1;
  }
  printf("big caves are:");
  for(i = 1; i < counter - 1; i++){
    if (bigCaves[i])
      printf("%s ", uniquePoints[i]);
  }
  printf("\n");

  for(i = 0; i < counter; i++)
    printf("%s\n", uniquePoints[i]);
  for(i = 0; i < graphCounter; i++)
    printf("%s - %s\n", initialGraph[i][0], initialGraph[i][1]);
  graph = (int **) calloc(counter, sizeof(int*));
  for(i = 0; i < counter; i++)
    graph[i] = (int *) calloc(counter, sizeof(int));
  int j;
  char *pointA, *pointB;
  int startA, startB;

  for(i = 0; i < graphCounter; i++){
    pointA = initialGraph[i][0];
    pointB = initialGraph[i][1];
    for(j = 0; j < counter; j++){
      if (! strcmp(pointA, uniquePoints[j])){
        startA = j;
        break;
      }
    }
    for(j = 0; j < counter; j++){
      if (! strcmp(pointB, uniquePoints[j])){
        startB = j;
        break;
      }
    }
    graph[startA][startB] = 1;
    graph[startB][startA] = 1;
  }
  for(i = 0; i < counter; i++){
    for(j = 0; j < counter; j++)
      printf("%d ", graph[i][j]);
    printf("\n");
  }
  printf("\n");
  for(i = 0; i < graphCounter; i++)
    free(initialGraph[i]);
  free(initialGraph);
}

//Why this parameter killCount?
//Well, for part 2 it is said that one small cave can be added twice
//So to keep track of that I add that information to the node itself
pathNode* createNode(int prevSize, int* prevPath, int i, int killCount){
  int size = prevSize + 1;
  int* newPath = (int*) malloc(size * sizeof(int));
  newPath = memcpy(newPath, prevPath, prevSize * sizeof(int));
  newPath[prevSize] = i;
  pathNode* newNode = (pathNode*) malloc(sizeof(pathNode));
  newNode -> path = newPath;
  newNode -> size = size;
  newNode -> killCount = killCount;
  newNode -> next = NULL;
  return newNode;
}

int pathCounter = 0;
//I didn't want to write a similar function for part 2, but there
//are definitely better ways to handle this
void part1or2(int part){
  queue = createNode(0, NULL, 0, part - 1);
  queuesLast = queue;
  pathNode *temp;
  int* arr;
  int size, lastPos, i, j, goodToGo;
  while(queue){
    temp = queue;
    arr = temp -> path;
    size = temp -> size;
    lastPos = arr[size - 1];
    int killCount = temp -> killCount;
    for(i = 1; i < counter - 1; i++){
      if (graph[i][lastPos]){
        goodToGo = 1;
        if(!bigCaves[i]){
          for(j = 0; j < size; j++){
            if(arr[j] == i){
                goodToGo = 0;
                break;
            }
          }
        }
        if (goodToGo){
          queuesLast -> next = createNode(size, arr, i, killCount);
          queuesLast = queuesLast -> next;
        }
        else if (!goodToGo && killCount){
          queuesLast -> next = createNode(size, arr, i, killCount - 1);
          queuesLast = queuesLast -> next;
        }
      }
    }
    //paths ll is to keep track of complete loops (start - ... - end)
    //queue is for the rest
    if(graph[lastPos][counter - 1]){
      pathCounter++;
      temp = createNode(size, arr, counter - 1, 0);
      if(paths == NULL){
        paths = temp;
        pathsLast = temp;
      }
      else{
        pathsLast -> next = temp;
        pathsLast = temp;
      }
    }
    temp = queue;
    queue = queue -> next;
    free(temp -> path);
    free(temp);
  }
  temp = paths;
  while(temp){
    arr = temp -> path;
    size = temp -> size;
    for(j = 0; j < size; j++)
      printf("%s,", uniquePoints[arr[j]]);
    printf("\n");
    temp = temp -> next;
  }
  printf("number of paths are %d\n", pathCounter);
}

int main(){
  readFromFile("../input_files/day12input.txt");
  part1or2(1);
  return 0;
}
