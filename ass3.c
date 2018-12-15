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
#include <stdlib.h>

typedef struct _Card_
{
  char color;
  int value;
  struct Card *next;

} Card;

Card* stack_array[7] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};

FILE *fp;
//variale initiation

//forward function declaration
int readInitFile(FILE *file, Card *draw_stack);
int checkDeck(Card *card_stack);
int initializeStacks();

int main(int argc, char* argv) {

  printf("Hello, World!\n");

  initializeStacks();

  fp = fopen(&argv[1], "r");
  readInitFile(fp, *stack_array+0);
  return 0;
}
int addCardToStack(char color, int value, Card **card_stack)
{
  Card *new_card;
  new_card = malloc(sizeof(Card));
  new_card->color = color;
  new_card->value = value;
  new_card->next = *card_stack;
  *card_stack = new_card;
}
int readInitFile(FILE *file, Card *draw_stack)
{
  char current_color;
  int current_value;
  int status = 0;
  while(!feof(file))
  {

    char current_character = fgetc(file);
    if(current_character == ' ')
    {
      continue;
    }
    switch(status)
    {
      case 0:
        if(current_character == 'B')
        {
          status = 1;
        }
        else if (current_character == 'R')
        {
          status = 3;
        }
        break;
      case 1:
        if(current_character == 'K')
        {
          current_color = 'B';
          status = 2;
        }
        break;
      case 2:
        switch(current_character)
        {
          case 'A':
            current_value = 1;
            break;
          case 'J':
            current_value = 11;
            break;
          case 'Q':
            current_value = 12;
            break;
          case 'K':
            current_value = 13;
            break;
          default:
            current_value = atoi(*current_character);
            break;
        }
        addCardToStack(current_color, current_value, stack_array[0]);
        status = 0;
        break;
      case 3:
        if(current_character == 'D')
        {
          current_color = 'R';
          status = 2;
        }
        break;

    }
  }
}
