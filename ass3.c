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

Card stack_array[7];

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
  readInitFile(fp, stack_array);
  return 0;
}
int initializeStacks()
{
  for(int i = 0; i < sizeof(stack_array); i++)
  {
    Card *stack_initiation;
    stack_initiation = malloc(sizeof(Card));
    stack_array[i] = *stack_initiation;
    stack_array[i].value = 0;
    stack_array[i].color = '\0';
    stack_array[i].next = NULL;
  }
}
int addCardToStack(char color, int value, Card *card_stack)
{
  Card *new_card;
  new_card = malloc(sizeof(Card));
  new_card->color = color;
  new_card->value = value;
  new_card->next = card_stack;
  *card_stack = new_card;
}
int readInitFile(FILE *file, Card *draw_stack)
{
  char current_color;
  int current_value;
  while(!feof(file))
  {
    int status;
    char current_character = fgetc(file);
    if(current_character == ' ')
    {
      continue;
    }
    switch(status)
    {
      case 1:
        if(current_character == 'B')
        {
          status = 2;
        }
        break;
      case 2:
        if(current_character == 'K')
        {
          current_color = 'B';
          status = 3;
        }
        break;
      case 3:
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
        addCardToStack(current_color, current_value, **stack_array[0]);

    }
  }
}
