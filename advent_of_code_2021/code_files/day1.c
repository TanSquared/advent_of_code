#include "header.h"

node *head = NULL;
node *tail = NULL;

node* fill_list(node* p){
  int num, seek;
  char* buf = (char*)malloc(sizeof(char));;
  char* word = (char*) malloc(sizeof(char) * 10);
  int fd = open("../input_files/day1input.txt", O_RDONLY);
  while ( (seek = read(fd, buf, 1)) > 0){
    if (strncmp(buf, "\n", sizeof(char)) == 0){
      //lseek is not needed here as read() is already taking care of shifting the offset
      //seek = lseek(fd, sizeof(char), SEEK_CUR);
      num = atoi(word);
      p = insert(p, num);
      free(word);
      word = (char*) malloc(sizeof(char) * 10);
    }
    else{
      word = strncat(word, buf, sizeof(char));
    }
  }
  close(fd);
  return p;
}

node* fill_list_triad(node* source, node* dest){
  node* saki;
  node* temp;
  int sum = source -> data;
  if ( (saki = source -> next) == NULL) return dest;
  sum = sum + saki -> data;
  if ( (temp = saki -> next) == NULL) return dest;
  sum = sum + temp -> data;
  dest = insert(dest, sum);
  dest = fill_list_triad(saki, dest);
  return dest;
}

int count_increase(node* p){
  if (p == NULL) return 0;
  int counter = 0;
  node* temp = p;
  while(temp -> next != NULL){
    if (temp -> data < temp -> next -> data) counter++;
    temp = temp -> next;
  }
  return counter;
}

int main(){
  head = fill_list(head);
  printf("counter increased %d times \n", count_increase(head));
  node* triad_head = NULL;
  triad_head = fill_list_triad(head, triad_head);
  printf("counter increased %d times \n", count_increase(triad_head));
  return 0;
}
