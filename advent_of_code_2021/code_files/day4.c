#include "header.h"

//This time we go with arrays as we only need to store them
int*** boards = NULL;
int* digits = NULL;
int digitCounter = 0;
int boardCounter = 0;

int** allocateBoard(){
  int** board = (int**) malloc(sizeof(int*) * 5);
  for(int i = 0; i < 5; i++)
    board[i] = (int*) calloc(5, sizeof(int));
  return board;
}

void fillArrays(){
  int seek, num;
  char* buf = (char*)malloc(sizeof(char));;
  char* word = (char*) malloc(sizeof(char) * 10);
  int fd = open("../input_files/day4input.txt", O_RDONLY);
  //Initial size is set to 8. No reason
  int size = 8;

  //First we fill the digits
  digits = (int*) malloc(sizeof(int) * size);
  while(strncmp(buf, "\n", sizeof(char)) != 0){
    seek = read(fd, buf, 1);
    if(strncmp(buf, ",", sizeof(char)) == 0){
      num = atoi(word);
      digits[digitCounter] = num;
      digitCounter++;
      //Ensures that for next element there would be space
      if (digitCounter == size){
        size = size * 2;
        digits = (int*) realloc(digits, sizeof(int) * size);
      }
      word = (char*) malloc(sizeof(char) * 10);
    }
    else{
      word = strncat(word, buf, sizeof(char));
    }
  }
  digits[digitCounter++] = atoi(word);
  word = (char*) malloc(sizeof(char) * 10);

  //Now time for filling boards
  seek = lseek(fd, 1, SEEK_CUR);
  size = 8;
  boards = (int ***)malloc(sizeof(int **) * size);
  boards[0] = allocateBoard();
  int row = 0, column;

  char* line;
  while ( (seek = read(fd, buf, 1)) > 0){
    line = (char*) malloc(sizeof(char) * 15);
    while (strncmp(buf, "\n", sizeof(char)) != 0){
      strncat(line, buf, sizeof(char));
      seek = read(fd, buf, 1);
    }
    if (strlen(line) != 0){
      column = 0;
      char delim[] = " ";
      char *ptr = strtok(line, delim);

      while(ptr != NULL)
      {
        boards[boardCounter][row][column] = atoi(ptr);
        column++;
        ptr = strtok(NULL, delim);
      }
      row++;
      if (row == 5){
        boardCounter++;
        if (boardCounter == size){
          size = size * 2;
          boards = (int***) realloc(boards, sizeof(int**) * size);
        }
        boards[boardCounter] = allocateBoard();
        row = 0;
      }
    }
  }
  close(fd);

  //Now fill in the attendance arrays
  for(int i = 0; i < boardCounter; i++)
    boards[boardCounter + i] = allocateBoard();
}

int didBoardWin(int** arr){
  int rowCount, columnCount;
  for(int i = 0; i < 5; i++){
    rowCount = 0;
    columnCount = 0;
    for(int j = 0; j < 5; j++){
      if (arr[i][j] == 1) rowCount++;
      if (arr[j][i] == 1) columnCount++;
    }
    if (rowCount == 5 || columnCount == 5) return 1;
  }
  return -1;
}

int markBoard(int num, int index, char* mode){
  for(int i = 0; i < 5; i++){
    for(int j = 0; j < 5; j++){
      if(boards[index][i][j] == num){
        if ( strncmp(mode, "first", 5) == 0)
          boards[index + boardCounter][i][j] = 1;
        else
          boards[index + boardCounter][i][j] = 0;
        return 1;
      }
    }
  }
  return -1;
}

//Score calculation: The name is misleading. It doesn't calculate the full score. We still have to multiply with called number at the end
//It is what it is(I was too lazy to change it)
int scoreCalculator(int index){
  int sum = 0;
  for(int row = 0; row < 5; row++){
    for(int column = 0; column < 5; column++){
      if(boards[index + boardCounter][row][column] == 0)
        sum = sum + boards[index][row][column];
    }
  }
  return sum;
}

//This function gets the first board that wins.
//It exits as soon as one board wins
long unsigned int playBingo(){
  int calledNum;
  for(int i = 0; i < digitCounter; i++){
    calledNum = digits[i];
    for(int j = 0; j < boardCounter; j++){
      if (markBoard(calledNum, j, "first") > 0){
        if (didBoardWin(boards[j + boardCounter]) > 0){
          int sum = scoreCalculator(j);
          return (calledNum * sum);
        }
      }
    }
  }
  return 0;
}

//This function keeps filling the attendance boards until roll call is finished.
void markAllAttendanceBoards(){
  int calledNum;
  for(int i = 0; i < digitCounter; i++){
    calledNum = digits[i];
    for(int j = 0; j < boardCounter; j++){
      markBoard(calledNum, j, "first");
    }
  }
}

//After marking of all attendance boards, we go backwards and check which board won last
int lastBoard(){
  int winBeforeMarking, winAfterMarking;
  int calledNum;
  for(int i = digitCounter - 1; i >= 0; i--){
    calledNum = digits[i];
    for(int j = 0; j < boardCounter; j++){
      winBeforeMarking = didBoardWin(boards[j + boardCounter]);
      markBoard(calledNum, j, "last");
      winAfterMarking = didBoardWin(boards[j + boardCounter]);
      if (winBeforeMarking != winAfterMarking){
        markBoard(calledNum, j, "first");
        return (scoreCalculator(j) * calledNum);
      }
    }
  }
  return -1;
}

int main(){
  fillArrays();
  //Since file parsing was tough for me I put print statements just to check if I got all info correct
  int i;
  for(i = 0; i < digitCounter; i++)
    printf("%d ", digits[i]);
  printf("\n");
  for(i = 0; i < boardCounter; i++)
    print2DIntArray(boards[i], 5, 5);
  printf("\n");

  //part 1
  long unsigned int winningScore;
  if ( (winningScore =  playBingo()) < 1) printf("what is this\n");
  else{
    printf("winningScore is %lu\n", winningScore);
  }

  //part 2
  markAllAttendanceBoards();
  int lastBoardScore;
  if ( (lastBoardScore = lastBoard()) > 0)
    printf("Last board score is %d\n", lastBoardScore);
  return 0;
}
