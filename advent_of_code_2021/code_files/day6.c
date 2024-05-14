#include "header.h"

long unsigned int** memoArray = NULL;
int* fishes = NULL;
int fishCounter = 0;
int size = 8;

void fillFishArray(){
  int seek;
  char* buf = (char*)malloc(sizeof(char));;
  char* word = (char*) malloc(10 * sizeof(char));
  int fd = open("../input_files/day6input.txt", O_RDONLY);
  fishes = (int*) malloc(size * sizeof(int));

  while ( (seek = read(fd, buf, 1)) > 0){
    if (fishCounter == size){
      size = size * 2;
      fishes = (int*) realloc(fishes, size * sizeof(int));
    }
    if (strncmp(buf, ",", 1) == 0){
      fishes[fishCounter] = atoi(word);
      free(word);
      word = (char*)malloc(10 * sizeof(char));
      fishCounter++;
    }
    else
      word = strncat(word, buf, 1);
  }
  fishes[fishCounter] = atoi(word);
  fishCounter++;
  free(word);
  close(fd);
}

long unsigned int countOrganims(int days_left, int state){
  long unsigned int count;
  if ( (count = memoArray[days_left][state]) > 0)
    return count;
  if (days_left <= state){
    memoArray[days_left][state] = 1;
    return 1;
  }
  if ( state == 0)
    count = countOrganims(days_left - 1, 6) + countOrganims(days_left - 1, 8);
  else count = countOrganims(days_left - 1, state - 1);
  memoArray[days_left][state] = count;
  return count;
}

int main(int argc, char* argv[]){
  int days_left = atoi(argv[1]);
  int i;
  fillFishArray();
  memoArray = (long unsigned int**) malloc((days_left + 1) * sizeof(long unsigned int*));
  for (i = 0; i <= days_left; i++)
    memoArray[i] = (long unsigned int*) calloc(8, sizeof(long unsigned int));
  long unsigned int count = 0;
  for(i = 0; i < fishCounter; i++)
    count = count + countOrganims(days_left, fishes[i]);
  printf("%lu\n", count);
  return 0;
}
