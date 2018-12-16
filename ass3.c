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

#define MAXSTACK 7
#define DRAW_STACK 0
#define GAME_STACK_1 1
#define GAME_STACK_2 2
#define GAME_STACK_3 3
#define GAME_STACK_4 4
#define DEP_STACK_1 5
#define DEP_STACK_2 6

typedef struct _Card_
{
  char color;
  int value;
  struct _Card_ *next;
  struct _Card_ *previous;

} Card;

typedef struct _Stack_ {
  Card *top_card;
  Card *bottom_card;
} Stack;
typedef enum _ReturnValue_
{
  EVERYTHING_OK = 0,
  INVALID_ARG_COUNT = -1,
  INVALID_FILE = -2,
} ReturnValue;

Stack stack_array[7] = {{NULL, NULL},
                        {NULL, NULL},
                        {NULL, NULL},
                        {NULL, NULL},
                        {NULL, NULL},
                        {NULL, NULL},
                        {NULL, NULL}};

FILE *fp;
//variale initiation

//forward function declaration
ReturnValue readInitFile(const char *file, Stack draw_stack);
int initStackArray();

ReturnValue printErrorMessage(ReturnValue return_value);

int main(int argc, char* argv[]) {


//  initializeStacks();
 // initStackArray();
  printf("argv: %s\n", argv[1]);
  ReturnValue return_value = readInitFile(argv[1], stack_array[DRAW_STACK]);

  if(return_value != EVERYTHING_OK)
  {
    return printErrorMessage(return_value);
  }
  return 0;
}

int initStackArray()
{
  for(int stack_number = 0; stack_number < MAXSTACK; stack_number++)
  {
    stack_array[stack_number].top_card = NULL;
    stack_array[stack_number].bottom_card = NULL;
  }
  return 0;
}

ReturnValue printErrorMessage(ReturnValue return_value)
{
  switch(return_value)
  {
    case INVALID_ARG_COUNT:
      printf("[ERR] Wrong number of parameters. Usage ./ass1 <config-file>\n");
      break;
    case INVALID_FILE:
      printf("[ERR] Config-File is invalid.\n");
      break;

    case EVERYTHING_OK:
      //left blank intentionally
      break;
  }
  return return_value;
}

int addCardToStackTop(char color, int value, Stack card_stack)
{
  Card *new_card;
  new_card = malloc(sizeof(Card));
  new_card->color = color;
  new_card->value = value;
  new_card->next = card_stack.top_card;
  new_card->previous = NULL;
  card_stack.top_card = new_card;
  if(card_stack.bottom_card == NULL)
  {
    card_stack.bottom_card = new_card;
  }
}
ReturnValue readInitFile(const char *path, Stack draw_stack)
{
  fp = fopen(path, "r");
  if(fp == NULL)
  {
    printf("file is NULL\n");
    return INVALID_FILE;
  }
  printf("readInitFile\n");
  char current_color;
  int current_value;
  int status = 0;
  while(!feof(fp))
  {
    int current_character = fgetc(fp);
    printf("current_character: %c, status: %i\n", current_character, status);
    if(current_character == ' ' || current_character == '\n')
    {
      printf("continue\n");
      continue;
    }
    switch(status)
    {
      case 0:
        status = -1;
        if(current_character == 'B')
        {
          status = 1;
        }
        else if (current_character == 'R')
        {
          status = 5;
        }
        break;
      case 1:
        status = -1;
        if(current_character == 'L')
        {
          status = 2;
        }
        break;
      case 2:
        status = -1;
        if(current_character == 'A')
        {
          status = 3;
        }
        break;
      case 3:
        status = -1;
        if(current_character == 'C')
        {
          status = 4;
        }
        break;
      case 4:
        status = -1;
        if(current_character == 'K')
        {
          current_color = 'B';
          status = 7;
        }
        break;
      case 5:
        status = -1;
        if(current_character == 'E')
        {
          status = 6;
        }
        break;
      case 6:
        status = -1;
        if(current_character == 'D')
        {
          current_color = 'R';
          status = 7;
        }
        break;
      case 7:
        switch(current_character)
        {
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':
          case '8':
          case '9':
            current_value = current_character - '0';
            break;
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
          case '1':
            current_character = fgetc(fp);
            if(current_character == '0')
            {
              current_value = 10;
            }
            else
            {
              return INVALID_FILE;
            }
            break;
          default:
            return INVALID_FILE;
        }
        printf("color: %c, value: %i\n",current_color, current_value);
        addCardToStackTop(current_color, current_value, stack_array[0]);
        status = 0;
        break;
      case -1:
      default:
        return INVALID_FILE;

    }
  }
  return EVERYTHING_OK;
}
