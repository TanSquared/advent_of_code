//This solution has led to a lot of lines of code, which I'm sure could be simplified, but I'm content with this state
//This is not a hard problem per say, but you'd have to spend some time with pen and paper before you even begin to understand what's happening
#include "header.h"

char**** displayData = NULL;
char** signalArray = NULL;
int lineCounter = 0;
int size = 8;

void fillDisplayDataArray(){
  int seek, wordCounter = 0, outputSwitch = 0;
  char* buf = (char*)malloc(sizeof(char));
  char* word;
  int fd = open("../input_files/day8input.txt", O_RDONLY);
  displayData = (char****)malloc(size * sizeof(char***));

  while ( (seek = read(fd, buf, 1)) > 0){
    displayData[lineCounter] = (char***)malloc(2 * sizeof(char**));
    displayData[lineCounter][0] = (char**) malloc(10 * sizeof(char*));
    displayData[lineCounter][1] = (char**) malloc(4 * sizeof(char*));
    outputSwitch = 0;
    wordCounter = 0;
    word = (char*) malloc(10 * sizeof(char));
    while (strncmp(buf, "\n", sizeof(char)) != 0){
      if (strncmp(buf, "|", 1) == 0){
        outputSwitch = 1;
        wordCounter = 0;
        seek = read(fd, buf, 1);
      }
      else if (strncmp(buf, " ", 1) == 0){
        displayData[lineCounter][outputSwitch][wordCounter] = (char*) malloc(strlen(word) * sizeof(char));
        displayData[lineCounter][outputSwitch][wordCounter] = strcpy(displayData[lineCounter][outputSwitch][wordCounter], word);
        free(word);
        word = (char*) malloc(10 * sizeof(char));
        wordCounter++;
      }
      else{
        word = strncat(word, buf, 1);
      }
      seek = read(fd, buf, 1);
    }
    displayData[lineCounter][outputSwitch][wordCounter] = (char*) malloc(strlen(word) * sizeof(char));
    displayData[lineCounter][outputSwitch][wordCounter] = strcpy(displayData[lineCounter][outputSwitch][wordCounter], word);
    lineCounter++;
    if(lineCounter == size){
      size = size * 2;
      displayData = (char****) realloc(displayData, size * sizeof(char***));
    }
  }
  free(word);
  close(fd);
}

int countUniqueDigitsOutput(){
  int result = 0;
  char* word = (char*) malloc(10 * sizeof(char));
  for(int i = 0; i < lineCounter; i++){
    for(int j = 0; j < 4; j++){
      word = strcpy(word, displayData[i][1][j]);
      if(strlen(word) == 2 || strlen(word) == 4 || strlen(word) == 3 || strlen(word) == 7)
        result++;
    }
  }
  return result;
}

char* charArray = NULL;

int existsIn(char alphabet, char* word, int size){
  for(int i = 0; i < size; i++){
    if(alphabet == word[i]) return 1;
  }
  return 0;
}

int existsInArrayAll(char alphabet, char** wordArr, int size, int arrSize){
  int counter = 0;
  for(int i = 0; i < arrSize; i++){
    if( existsIn(alphabet, wordArr[i], size) ) counter++;
  }
  if (counter == arrSize) return 1;
  return 0;
}

/*
The seven segment display is something like:
   ---
  |   |
  |   |
   ---
  |   |
  |   |
   ---

If I have to code the segments, I do it like this:

   0
  1 2
   3
  4 5
   6
Depending on which segment is on/off, the number is displayed. So for each display, I'm trying to store the information of 
which character represents which segment in a string, which the function below does (fillCharArray).

I solve it in a few stages. First I see the string with only 2 characters. This has to represent a 1, so segments 2 and 5
must be on. I store them in an intermediate array twoFive, since I'm not sure which is which.
Now I look at the string with four characters, that must represent a 4.
Since they share their characters with twoFive, I see which two characters were left out, and store them in intermediate array oneThree (since segments one and three must be on along with two and five for representing 4).

Now we look at the string with three characters. That must represent a 7. Only character they add to twoFive is segment 0. Since I'm sure this character represents the 0 segment, I store it in charArray. Now we check which characters we haven't encountered until now. There can be only two, representing segments 4 and 6, so I store them in intermediate array fourSix. Onto second stage!

This time we can start putting the characters from intermediate arrays to charArray. Method I chose is to look at all five-lettered strings. There can be three such strings. Only segments 3 and 6 should be present in all three strings, so that helps us figure out the characters for segments 2, 5, 1 and 3.
Next, among the five-lettered strings, which represent 2, 3 and 5, segment 4 and 5 can never be on together. This gives us a clue to solve for the remaining segments 4 and 6.

Hurray! After doing so much work we are halfway done -_-
*/

void fillCharArray(int index){
  int i, counter = 0;
  char* twoFive = (char*) malloc(2 * sizeof(char));
  char* oneThree = (char*) malloc(2 * sizeof(char));
  char* fourSix = (char*) malloc(2 * sizeof(char));
  char* threeLetter = (char*) malloc(3 * sizeof(char));
  char* fourLetter = (char*) malloc(4 * sizeof(char));
  char* sevenLetter = (char*) malloc(7 * sizeof(char));
  char** fiveLetterWords = (char**) malloc(3 * sizeof(char*));
  for(i = 0; i < 3; i++)
    fiveLetterWords[i] = (char*) malloc(10 * sizeof(char));

  for(i = 0; i < 10; i++){
    if(strlen(displayData[index][0][i]) == 2)
      twoFive = strcpy(twoFive, displayData[index][0][i]);
    if(strlen(displayData[index][0][i]) == 3)
      threeLetter = strcpy(threeLetter, displayData[index][0][i]);
    if(strlen(displayData[index][0][i]) == 4)
      fourLetter = strcpy(fourLetter, displayData[index][0][i]);
    if(strlen(displayData[index][0][i]) == 7)
      sevenLetter = strcpy(sevenLetter, displayData[index][0][i]);
    if(strlen(displayData[index][0][i]) == 5){
      fiveLetterWords[counter] = strcpy(fiveLetterWords[counter], displayData[index][0][i]);
      counter++;
    }
  }
  counter = 0;
  for(i = 0; i < 4; i++){
    if (! existsIn(fourLetter[i], twoFive, 2) ){
      oneThree[counter] = fourLetter[i];
      counter++;
    }
  }
  counter = 0;
  for(i = 0; i < 3; i++){
    if (! existsIn(threeLetter[i], twoFive, 2) ){
      charArray[0] = threeLetter[i];
      break;
    }
  }
  for(i = 0; i < 7; i++){
    if ( !existsIn(sevenLetter[i], twoFive, 2) && !existsIn(sevenLetter[i], oneThree, 2) && (sevenLetter[i] != charArray[0]) ){
      fourSix[counter] = sevenLetter[i];
      counter++;
    }
  }

  char alphabet = oneThree[0];
  if ( existsInArrayAll(alphabet, fiveLetterWords, 5, 3) ){
    charArray[3] = alphabet;
    charArray[1] = oneThree[1];
  }
  else{
    charArray[1] = alphabet;
    charArray[3] = oneThree[1];
  }
  alphabet = fourSix[0];
  if ( existsInArrayAll(alphabet, fiveLetterWords, 5, 3) ){
    charArray[6] = alphabet;
    charArray[4] = fourSix[1];
  }
  else{
    charArray[4] = alphabet;
    charArray[6] = fourSix[1];
  }

  char* wordWithFour = (char*) malloc(5 * sizeof(char));
  for(i = 0; i < 3; i++){
    if (existsIn(charArray[4], fiveLetterWords[i], 5)){
      wordWithFour = strcpy(wordWithFour, fiveLetterWords[i]);
      break;
    }
  }

  if (existsIn(twoFive[0], wordWithFour, 5)){
    charArray[2] = twoFive[0];
    charArray[5] = twoFive[1];
  }
  else{
    charArray[5] = twoFive[0];
    charArray[2] = twoFive[1];
  }
}

//Because we don't know if characters would be in same order in each string.
int strcmp_WithoutOrder(char* word1, char* word2){
  int len;
  if ( (len = strlen(word1)) != strlen(word2) )
    return 0;
  for(int i = 0; i < len; i++){
    if (!existsIn(word1[i], word2, len)) return 0;
  }
  return 1;
}

int returnOutputValue (int index, char** signalArray){
  int result = 0;
  int power = 3;
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 10; j++){
      if (strcmp_WithoutOrder(signalArray[j], displayData[index][1][i])){
        result = result + j * pow(10, power);
        break;
      }
    }
    power--;
  }
  return result;
}

int main(){
  fillDisplayDataArray();
  int i, j;
  long unsigned int displayDataResult = 0;
  charArray = (char*) malloc(7 * sizeof(char));
  char** signalArray = (char**) malloc(10 * sizeof(char*));
  for(i = 0; i < lineCounter; i++){

    /*
    Lot of code written for this part.
    At this point we've figured  out which character represents which segment. Now we tell the program to display 
    the numbers 1 - 10 which segments (characters) are required.
    e.g. for displaying 0 we need 0, 1, 2, 4, 5, 6
    for displaying 5 we need 0, 1, 3, 5, 6

    Once that is done, we check against provided input to see which number it is and add them to our result.
    */
    fillCharArray(i);
    signalArray[0] = (char*) malloc(6 * sizeof(char));
    signalArray[0] = strncpy(signalArray[0], charArray, 3);
    signalArray[0] = strncat(signalArray[0], charArray + 4, 3);

    signalArray[1] = (char*) malloc(2 * sizeof(char));
    signalArray[1] = strncpy(signalArray[1], charArray + 2, 1);
    signalArray[1] = strncat(signalArray[1], charArray + 5, 1);

    signalArray[2] = (char*) malloc(5 * sizeof(char));
    signalArray[2] = strncpy(signalArray[2], charArray, 1);
    signalArray[2] = strncat(signalArray[2], charArray + 2, 3);
    signalArray[2] = strncat(signalArray[2], charArray + 6, 1);

    signalArray[3] = (char*) malloc(5 * sizeof(char));
    signalArray[3] = strcpy(signalArray[3], signalArray[2]);
    signalArray[3][3] = charArray[5];

    signalArray[4] = (char*) malloc(4 * sizeof(char));
    signalArray[4] = strncpy(signalArray[4], charArray + 1, 3);
    signalArray[4][3] = charArray[5];

    signalArray[5] = (char*) malloc(5 * sizeof(char));
    signalArray[5] = strncpy(signalArray[5], signalArray[3], 5);
    signalArray[5][1] = charArray[1];

    signalArray[6] = (char*) malloc(6 * sizeof(char));
    signalArray[6] = strncpy(signalArray[6], signalArray[5], 3);
    signalArray[6] = strncat(signalArray[6], charArray + 4, 3);

    signalArray[7] = (char*) malloc(3 * sizeof(char));
    signalArray[7][0] = charArray[0];
    signalArray[7] = strncat(signalArray[7], signalArray[1], 2);

    signalArray[8] = (char*) malloc(7 * sizeof(char));
    signalArray[8] = strncpy(signalArray[8], charArray, 7);

    signalArray[9] = (char*) malloc(6 * sizeof(char));
    signalArray[9] = strncpy(signalArray[9], signalArray[0], 6);
    signalArray[9][3] = charArray[3];

    displayDataResult = displayDataResult + returnOutputValue(i, signalArray);
    for(j = 0; j < 10; j++)
      free(signalArray[j]);
  }

  printf("%d\n", countUniqueDigitsOutput());
  printf("%lu\n", displayDataResult);

  return 0;
}
