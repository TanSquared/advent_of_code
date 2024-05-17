//One key thing to note is that we can also move left and up along with right and down
//I use A* algorithm here
#include "header.h"

int** part1graph = NULL;
int row, column;
//I am not going to solve for part2, as I fear for my computer
//I will just evaluate the graph, so if I want I can just run analyze() on it
int** part2graph = NULL;

//We don't really need the prev pointer, but it was helpful for debugging so I'm keeping it
typedef struct indexNode{
  long unsigned int total;
  int row;
  int column;
  struct indexNode *next;
  struct indexNode *prev;
} indexNode;

indexNode* indexPath = NULL;

void fileReadandCreateRule(char *filePath){
  int fd = open(filePath, O_RDONLY);
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
  part1graph = calloc(row, sizeof(int*));
  for(i = 0; i < row; i++)
    part1graph[i] = calloc(column, sizeof(int));

  //Now that we know row and column value, we set the counter back to start of file
  lseek(fd, 0, SEEK_SET);

  int current_row = 0;
  int current_column = 0;
  while ( (seek = read(fd, buf, 1)) > 0){
    while(strncmp(buf, "\n", 1) != 0){
      part1graph[current_row][current_column++] = atoi(buf);
      seek = read(fd, buf, 1);
    }
    current_row++;
    current_column = 0;
  }

  //Now we fill part2graph
  int j, k;
  part2graph = calloc(5 * row, sizeof(int*));
  for(i = 0; i < 5 * row; i++)
    part2graph[i] = calloc(5 * column, sizeof(int));
  for(i = 0; i < row; i++){
    for(j = 0; j < column; j++)
      part2graph[i][j] = part1graph[i][j];
  }
  i = 0;
  j = 0;
  for(k = 1; k < 5; k++){
    for(i = 0; i < row; i++){
      for(j = (k - 1) * column; j < k * column; j++)
        part2graph[i][j + column] = (part2graph[i][j]) % 9 + 1;
    }
  }

  for(k = 1; k < 5; k++){
    for(i = (k -1) * row; i < k * row; i++){
      for(j = 0; j < 5 * column; j++)
        part2graph[i + row][j] = (part2graph[i][j]) % 9 + 1;
    }
  }

}

indexNode* createIndex(int x, int y, long unsigned int total){
  indexNode* temp = calloc(1, sizeof(indexNode));
  temp -> row = x;
  temp -> column = y;
  temp -> total = total;
  return temp;
}

void printIndexPath(){
  indexNode* debug = indexPath;
  printf("{");
  while(debug){
    printf("[(%d, %d) -> %lu], ", debug -> row, debug -> column, debug -> total);
    debug = debug -> next;
  }
  printf("}\n");
}

indexNode* insertIndex(indexNode* nodeToInsert){
  if (nodeToInsert == NULL) return indexPath;
  if (indexPath == NULL) return nodeToInsert;

  if (indexPath -> total >= nodeToInsert -> total){
    nodeToInsert -> next = indexPath;
    return nodeToInsert;
  }

  indexNode *temp, *saki;
  temp = indexPath;
  saki = NULL;

  //For the case if we find an indentical node with different total
  while(temp != NULL){
    if (temp -> row == nodeToInsert -> row && temp -> column == nodeToInsert -> column){
      if (temp -> total > nodeToInsert -> total){
        nodeToInsert -> next = temp -> next;
        if (saki) saki -> next = nodeToInsert;
      }
      return indexPath;
    }
    saki = temp;
    temp = temp -> next;
  }

  temp = indexPath;
  while( (temp != NULL) && (temp -> total < nodeToInsert -> total) ){
    saki = temp;
    temp = temp -> next;
  }

  nodeToInsert -> next = temp;
  saki -> next = nodeToInsert;

  return indexPath;
}

//long unsigned int analyze(){
indexNode* analyze(int** graph){
  indexPath = createIndex(0, 0, 0);

  while(indexPath != NULL){
    indexNode* temp = indexPath;
    indexPath = indexPath -> next;

    int current_row = temp -> row;
    int current_column = temp -> column;
    int nextRow = current_row + 1;
    int nextColumn = current_column + 1;
    int prevRow = current_row - 1;
    int prevColumn = current_column - 1;
    indexNode *rightChild = NULL, *downChild = NULL, *upChild = NULL, *leftChild = NULL;

    if (nextRow < row){
      downChild = createIndex(nextRow, current_column, temp -> total + graph[nextRow][current_column]);
      downChild -> prev = temp;
      if ( (downChild -> row == row - 1) && (downChild -> column == column - 1) )
        return downChild;
    }
    if (prevRow >= 0){
      upChild = createIndex(prevRow, current_column, temp -> total + graph[prevRow][current_column]);
      upChild -> prev = temp;
      if ( (upChild -> row == row - 1) && (upChild -> column == column - 1) )
        return upChild;
    }
    if (nextColumn < column){
      rightChild = createIndex(current_row, nextColumn, temp -> total + graph[current_row][nextColumn]);
      rightChild -> prev = temp;
      if ( (rightChild -> row == row - 1) && (rightChild -> column == column - 1) )
        return rightChild;
    }
    if (prevColumn >= 0){
      leftChild = createIndex(current_row, prevColumn, temp -> total + graph[current_row][prevColumn]);
      leftChild -> prev = temp;
      if ( (leftChild -> row == row - 1) && (leftChild -> column == column - 1) )
        return leftChild;
    }

    indexPath = insertIndex(downChild);
    indexPath = insertIndex(upChild);
    indexPath = insertIndex(rightChild);
    indexPath = insertIndex(leftChild);

  }

  return NULL;
}

int main(){
  fileReadandCreateRule("../input_files/day15input.txt");
  indexNode* result = analyze(part1graph);
  printf("%lu\n", result -> total);
  //indexNode* result2 = analyze(part2graph);
  //printf("%lu\n", result2 -> total);

  return 0;
}
