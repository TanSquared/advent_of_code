#include "header.h"

int* zeroList = NULL;
int* oneList = NULL;

typedef struct charNode {
  char* number;
  struct charNode* next;
} charNode;

void printCharNode(charNode* head){
  charNode* temp = head;
  while(temp){
    printf("%s\n", temp -> number);
    temp = temp -> next;
  }
}

//Read the file and add data to a linked list. We use linked list because in part 2 of the problem we have to remove numbers from the list from random indexes
charNode* fillList(){
  int seek;
  char* buf = (char*)malloc(sizeof(char));;
  char* word = (char*) malloc(sizeof(char) * 10);
  int fd = open("../input_files/day3input.txt", O_RDONLY);

  charNode* head = (charNode*)malloc(sizeof(charNode));
  charNode* temp = head;
  //Just to keep track of node previous to temp
  charNode* prev = NULL;

  while ( (seek = read(fd, buf, 1)) > 0){
    if (strncmp(buf, "\n", sizeof(char)) == 0){
      //lseek is not needed here as read() is already taking care of shifting the offset
      temp -> number = word;
      //This adds a new node whenever "\n" is seen. This algo makes us add a node after the last line, which is useless
      temp -> next = (charNode*)malloc(sizeof(charNode));
      prev = temp;
      temp = temp -> next;
      word = (char*) malloc(sizeof(char) * 10);
    }
    else{
      word = strncat(word, buf, sizeof(char));
    }
  }
  close(fd);
  //We remove the last node from the list, as it's useless
  free(temp);
  prev -> next = NULL;
  return head;
}

//Although a new list is being formed, the location of strings storing the numbers are the same
//Only pointers to them are copied. So any change to a string would reflect across all copies
charNode* duplicateList(const charNode* head){
  charNode* newList = NULL;
  const charNode* tempHead = head;
  charNode* tempNewList = NULL;
  while(tempHead){
    charNode* newNode = (charNode*) malloc(sizeof(charNode));
    newNode -> number = tempHead -> number;
    if(newList == NULL){
      newList = newNode;
      tempNewList = newList;
    }
    else{
      while(tempNewList -> next)
        tempNewList = tempNewList -> next;
      tempNewList -> next = newNode;
    }
    tempHead = tempHead -> next;
  }
  return newList;
}

//Populate the oneList and zeroList before calculating power_consumption()
void populate_lists(charNode* head){
  zeroList = (int*) calloc(12, sizeof(int));
  oneList = (int*) calloc(12, sizeof(int));
  charNode* temp = head;
  while(temp){
    char* number = temp -> number;
    for(int i = 0; i < 12; i++){
      if (number[i] == '0')
        zeroList[i]++;
      else
        oneList[i]++;
    }
    temp = temp -> next;
  }
}

long unsigned int power_consumption(){
  int epsilon_rate = 0, gamma_rate = 0;
  int least, most, weight;
  for (int i = 0; i < 12; i++){
    weight = 1;
    if (oneList[i] > zeroList[i]){
      most = 1;
      least = 0;
    }
    else{
      most = 0;
      least = 1;
    }
    //For binary to int conversion. Later I wrote a function binaryToIntConverter, but since this piece was already written by then didn't bother changing
    for(int j = 0; j < 12 - i - 1; j++)
      weight = 2 * weight;

    gamma_rate = gamma_rate + weight * most;
    epsilon_rate = epsilon_rate + weight * least;
  }
  return (gamma_rate * epsilon_rate);
}

//Function to let us know the siginificant digit in the specific index
char bit_counter(charNode* head, char* mode, int index){
  int zeroes = 0, ones = 0;
  //if (head == NULL) return -1;
  charNode* temp = head;
  char digit;
  while(temp){
    digit = temp -> number[index];
    if (digit == '0') zeroes++;
    else ones++;
    temp = temp -> next;
  }
  // rules of oxygen scrubber_rating:
  // To find oxygen generator rating, determine the most common value (0 or 1) in the current bit position, and keep only numbers with that bit in that position. If 0 and 1 are equally common, keep values with a 1 in the position being considered
  if ( strncmp(mode, "oxygen", 6) == 0){
    if (zeroes > ones) return '0';
    return '1';
  }
  // rules of co2 scrubber_rating:
  // To find CO2 scrubber rating, determine the least common value (0 or 1) in the current bit position, and keep only numbers with that bit in that position. If 0 and 1 are equally common, keep values with a 0 in the position being considered
  else if ( strncmp(mode, "co2", 3) == 0){
    if (ones < zeroes) return '1';
    return '0';
  }
  else return 'x';
}

long unsigned int scrubber_rating(charNode* p, char* mode){
  charNode* head = duplicateList(p);
  //We insert a dummy node behind head
  //The dummy would ensure that even if head has to be removed there'd be no issue
  charNode* dummy = (charNode*) malloc(sizeof(charNode));
  dummy -> next = head;

  for(int i = 0; (i < 12 && dummy -> next != NULL); i++){
    char digit = bit_counter(dummy -> next, mode, i);
    charNode* temp = dummy -> next;
    //In case only one node is left, no need to go further
    if (temp -> next == NULL) break;
    charNode* prev = dummy;

    while(temp){
      if (temp -> number[i] == digit){
        prev = temp;
        temp = temp -> next;
      }
      else{
        temp = temp -> next;
        prev -> next = temp;
      }
    }
  }
  return (dummy -> next == NULL ? 0: binaryToIntConverter(dummy -> next -> number));
}

int main(){
  //part1 (and data input from file into our list)
  charNode* head = NULL;
  populate_lists(head = fillList());
  printf("power consumption is %lu\n", power_consumption());

  //part2
  long unsigned int os = scrubber_rating(head, "oxygen");
  long unsigned int cs = scrubber_rating(head, "co2");
  printf("oxygen scrubber_rating is %lu\tco2 scrubber_rating is %lu\t and life support rating is %lu\n", os, cs, os * cs);
  return 0;
}

