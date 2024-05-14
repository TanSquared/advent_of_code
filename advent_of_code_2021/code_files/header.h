#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdbool.h>
#include<math.h>

typedef struct Node
{
    int data;
    struct Node *next;
} node;

typedef struct charNode
{
  char data;
  struct charNode *next;
} charNode;

void print(node *p)
{
    printf("[");
    node *temp = p;
    while (temp->next != NULL)
    {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("%d]\n", temp->data);
}

node* insert(node* head, int num){
  node* newNode = (node*) malloc(sizeof(node));
  newNode -> data = num;
  newNode -> next = NULL;
  if (head == NULL)
    head = newNode;
  else{
    node* temp = head;
    while (temp -> next)
      temp = temp -> next;
    temp -> next = newNode;
  }
  return head;
}

int max(int a, int b){
  if (b > a) return b;
  return a;
}

int min(int a, int b){
  if (b < a) return b;
  return a;
}

int maxList(int* list, int size){
  int max = list[0];
  for(int i = 1; i < size; i++){
    if (list[i] > max)
      max = list[i];
  }
  return max;
}

int minList(int* list, int size){
  int min = list[0];
  for(int i = 1; i < size; i++){
    if (list[i] < min)
      min = list[i];
  }
  return min;
}

long unsigned int maxListLU(long unsigned int* list, long unsigned int size){
  long unsigned int max = list[0];
  for(long unsigned int i = 1; i < size; i++){
    if (list[i] > max)
      max = list[i];
  }
  return max;
}

long unsigned int minListLU(long unsigned int* list, long unsigned int size){
  long unsigned int min = list[0];
  for(long unsigned int i = 1; i < size; i++){
    if (list[i] < min)
      min = list[i];
  }
  return min;
}

int binaryToIntConverter(char* arr){
  char* start = &arr[0];
  int total = 0;
  while(*start){
    total *= 2;
    if (*start++ == '1') total += 1;
  }
  return total;
}

char* intToBinary(char* word, int num){
  //Important that we don't have a buffer overrun
  int initial_size = 0;
  int size = 8;
  word = (char*) malloc(8 * sizeof(char));
  while(num){
    if(num % 2) word = strncat(word, "1", 2);
    else word = strncat(word, "0", 2);
    initial_size++;
    if (initial_size == size){
      size = size * 2;
      word = (char*) realloc(word, sizeof(char) * size);
    }
    num = num/2;
  }
  size = strlen(word);
  //We do reverse because while calculating binary this way we always add numbers in opposite order, and C gives no direct way to prepend char to string
  char* reverseWord = (char*)malloc(sizeof(char) * size);
  for(initial_size = 0; initial_size < size; initial_size++)
    reverseWord[initial_size] = word[size - initial_size - 1];
  return reverseWord;
}

void print2DIntArray(int** arr, int rowSize, int columnSize){
  for(int i = 0; i < rowSize; i++){
    for(int j = 0; j < columnSize; j++)
      printf("%d ", arr[i][j]);
    printf("\n");
  }
  printf("\n");
}

