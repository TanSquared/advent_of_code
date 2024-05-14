#include "header.h"

long int calculate_puz1(int seek, int counter, int num, char* buf, char* word, long int* position, int fd){
  while ( (seek = read(fd, buf, 1)) > 0){
    if (strncmp(buf, "\n", sizeof(char)) == 0){
      //lseek is not needed here as read() is already taking care of shifting the offset
      num = atoi(word);
      position[counter] = num + position[counter];
      free(word);
      word = (char*) malloc(sizeof(char) * 10);
    }
    else if (strncmp(buf, " ", sizeof(char)) == 0){
      if (strcmp(word, "forward") == 0) counter = 0;
      else if (strcmp(word, "up") == 0) counter = 1;
      else counter = 2;
      free(word);
      word = (char*) malloc(sizeof(char) * 10);
    }
    else{
      word = strncat(word, buf, sizeof(char));
    }
  }
  long int depth = position[2] - position[1];
  long int forward = position[0];
  return (forward * depth);
}

long int calculate_puz2(int seek, int counter, int num, char* buf, char* word, long int* position, int fd){
  while ( (seek = read(fd, buf, 1)) > 0){
    if (strncmp(buf, "\n", sizeof(char)) == 0){
      //lseek is not needed here as read() is already taking care of shifting the offset
      num = atoi(word);
      if (counter == 0){
        position[counter] = num + position[counter];
        position[1] = position[1] + num * position[2];
      }
      else if (counter == 1)
        position[2] = position[2] - num;
      else
        position[counter] = position[counter] + num;
      free(word);
      word = (char*) malloc(sizeof(char) * 10);
    }
    else if (strncmp(buf, " ", sizeof(char)) == 0){
      if (strcmp(word, "forward") == 0) counter = 0;
      else if (strcmp(word, "up") == 0) counter = 1;
      else counter = 2;
      free(word);
      word = (char*) malloc(sizeof(char) * 10);
    }
    else{
      word = strncat(word, buf, sizeof(char));
    }
  }
  //aim was position[2]
  long int depth = position[1];
  long int forward = position[0];
  return (forward * depth);
}

int main(){
  int seek = 0, counter = 0, num = 0;
  char* buf = (char*)malloc(sizeof(char));;
  char* word = (char*) malloc(sizeof(char) * 10);
  long int* position = (long int*) calloc(3, sizeof(long int));

  int fd = open("../input_files/day2input.txt", O_RDONLY);
  printf("%li\n", calculate_puz1(seek, counter, num, buf, word, position, fd));
  // The array was changed so again setting all values to zero
  memset(position, 0, sizeof(long int) * 3);
  // Since the offset was at the end of file after reading it during calculate_puz1(), we set it again to the beginning
  lseek(fd, 0, SEEK_SET);
  printf("%li\n", calculate_puz2(seek, counter, num, buf, word, position, fd));
  close(fd);
  return 0;
}
