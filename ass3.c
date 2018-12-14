//TODO:
//  - config einlesen und überprüfen
//  - Fehlerverwaltung
//      - out of memory
//      - invalid file
//      - invalid command
//  - Spielfeld - Generierung
//  - Initaler Spielstart
//  - Usereingaben einlesen und verarbeiten
//  - Spielmechaniken implementieren
//  - help implementieren
//  - exit UND EOF implementieren
//  - win- und lose-condition implementieren
//


#include <stdio.h>

typeof struct _CardStack_
{
  char color;
  int value;
  struct cardStack *next;

} CardSack;

//variale initiation
char *drawStack;

//forward function declaration
int readInitFile(FILE *file, char *cardStack);
int checkDeck(*cardStack);

int main(int argc, char* argv) {

  printf("Hello, World!\n");
  return 0;
}