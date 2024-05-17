//One key thing to note is that we can also move left and up along with right and down
//I use A* algorithm here. It takes less than a sec for Part1 and about 14 min for Part2 in my PC
#include "header.h"
int** graph = NULL;
int row, column;

typedef struct indexNode{
  long unsigned int total;
  int row;
  int column;
  struct indexNode *next;
} indexNode;

//Store list of nodes to evaluate in a min heap for indexPath
indexNode **indexPath, **visitedPaths  = NULL;
int size, lastIndex, visited;

int fileReadandCreateRule(char *filePath){
  int fd = open(filePath, O_RDONLY);
  if (fd < 0) return 0;

  char* buf = (char*)malloc(sizeof(char));;
  int seek, i;

  //calculating the column (and therefore row)
  column = 0;
  while(strncmp(buf, "\n", sizeof(char)) != 0){
    column++;
    read(fd, buf, 1);
  }
  column = column - 1;
  row = column;
  graph = calloc(row, sizeof(int*));
  for(i = 0; i < row; i++)
    graph[i] = calloc(column, sizeof(int));

  //Now that we know row and column value, we set the counter back to start of file
  lseek(fd, 0, SEEK_SET);

  int current_row = 0;
  int current_column = 0;
  while ( (seek = read(fd, buf, 1)) > 0){
    while(strncmp(buf, "\n", 1) != 0){
      graph[current_row][current_column++] = atoi(buf);
      seek = read(fd, buf, 1);
    }
    current_row++;
    current_column = 0;
  }

  return 1;
}

indexNode* createIndex(int x, int y, long unsigned int total){
  indexNode* temp = calloc(1, sizeof(indexNode));
  temp -> row = x;
  temp -> column = y;
  temp -> total = total;
  return temp;
}

void swap(int a, int b){
  indexNode *c = *(indexPath + a);
  *(indexPath + a) = *(indexPath + b);
  *(indexPath + b) = c;
}

void heapifyUp(int index){
  if (index == 0) return;
  int parent = (index - 1)/2;
  if (indexPath[parent] -> total > indexPath[index] -> total){
    swap(parent, index);
    heapifyUp(parent);
  }
}

void heapifyDown(int index){
  int leftChild = 2 * index + 1;
  int rightChild = 2 * index + 2;
  int smallest = index;
  if ( (leftChild < lastIndex) && (indexPath[leftChild] -> total < indexPath[index] -> total) )
    smallest = leftChild;
  if ( (rightChild < lastIndex) && (indexPath[rightChild] -> total < indexPath[smallest] -> total) )
    smallest = rightChild;
  if (smallest != index){
    swap(smallest, index);
    heapifyDown(smallest);
  }
}

indexNode** insertIndex(indexNode* nodeToInsert){
  indexPath[lastIndex] = nodeToInsert;
  heapifyUp(lastIndex);
  lastIndex++;
  if (lastIndex == size){
    size = size * 2;
    indexPath = realloc(indexPath, size * sizeof(indexNode*));
  }
  return indexPath;
}

int alreadyVisited(int current_row, int current_column){
  for(int i = 0; i < visited; i++){
    if (visitedPaths[i] -> row == current_row && visitedPaths[i] -> column == current_column)
      return 1;
  }
  return 0;
}

indexNode* analyze(int factor){
  lastIndex = 0;
  visited = 0;
  int bigRow = row * factor;
  int bigColumn = column * factor;
  //Assuming that this much size would be nearly enough
  //Anyways, if it's not we'll realloc
  size = bigRow * bigColumn;
  indexPath = calloc(size, sizeof(indexNode*));
  visitedPaths = calloc(size, sizeof(indexNode*));
  indexPath = insertIndex(createIndex(0, 0, 0));

  int newRow, newColumn;
  indexNode* temp;
  long unsigned int cycles = 0;
  while(lastIndex){
    cycles++;

    temp = indexPath[0];
    
    if (temp -> row == bigRow - 1 && temp -> column == bigColumn - 1) return temp;

    int current_row = temp -> row;
    int current_column = temp -> column;
    long unsigned int total = temp -> total;
    int newTotal, baseRow, baseColumn;
    int repeat, skip;
    for(int i = -1; i < 2; i++){
      for(int j = -1; j < 2; j++){
        if (abs(i) == abs(j)) continue;
        newRow = current_row + i;
        newColumn = current_column + j;
        if ( (newRow >= 0 && newColumn >= 0) && (newRow < bigRow && newColumn < bigColumn) && (alreadyVisited(newRow, newColumn) == 0) ){
          skip = 0;
          repeat = newRow/row + newColumn/column;
          baseRow = newRow % row;
          baseColumn = newColumn % column;
          newTotal = (graph[baseRow][baseColumn] + repeat - 1) % 9 + 1;
          for(int k = 0; k < lastIndex; k++){
            if (indexPath[k] -> row == newRow && indexPath[k] -> column == newColumn){
              if (indexPath[k] -> total > total + newTotal)
                indexPath[k] -> total = total + newTotal;
              skip = 1;
            }
          }
          if (skip) continue;
          indexNode* newNode = createIndex(newRow, newColumn, total + newTotal);
          indexPath = insertIndex(newNode);
        }
      }
    }
    lastIndex--;
    swap(lastIndex, 0);
    heapifyDown(0);
    visitedPaths[visited++] = temp;
    if (cycles % 5000 == 0)
        printf("Cycles = %lu\n", cycles);

  }

  free(indexPath);
  free(visitedPaths);
  return NULL;
}

int main(){
  if (fileReadandCreateRule("code/advent_of_code_2021/day15input.txt")){
    indexNode* resultPart1 = analyze(1);
    indexNode* resultPart2 = analyze(5);
    printf("Part1: %lu\nPart2: %lu\n", resultPart1 -> total, resultPart2 -> total);
  }

  return 0;
}
