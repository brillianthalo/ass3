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
#define MATCHFIELDHEIGHT 18
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
int initialized_cards[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//forward function declaration
ReturnValue readInitFile(const char *file, Stack draw_stack);
int initStackArray();
int moveCard(Stack *from_stack, Stack *to_stack);
int initialHandOut();

ReturnValue printErrorMessage(ReturnValue return_value);
int printStack(Stack *stack);

int main(int argc, char* argv[]) {


//  initializeStacks();
//  initStackArray();
  printf("argv: %s\n", argv[1]);
  ReturnValue return_value = readInitFile(argv[1], stack_array[DRAW_STACK]);

  if(return_value != EVERYTHING_OK)
  {
    return printErrorMessage(return_value);
  }

  initialHandOut();

  printStack(&stack_array[DRAW_STACK]);
  printf("top card: %c %i, bottom card: %c %i\n", stack_array[DRAW_STACK].top_card->color, stack_array[DRAW_STACK].top_card->value, stack_array[DRAW_STACK].bottom_card->color, stack_array[DRAW_STACK].bottom_card->value);
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

int addCardToStackTop(Card *new_card, Stack *card_stack)
{

  new_card->next = card_stack->top_card;
  if(card_stack->top_card != NULL)
  {
    card_stack->top_card->previous = new_card;
  }
  new_card->previous = NULL;
  card_stack->top_card = new_card;
  if(card_stack->bottom_card == NULL)
  {
    card_stack->bottom_card = new_card;
  }
}

int moveCard(Stack *from_stack, Stack *to_stack)
{
  Card *moving_card = from_stack->top_card;
  from_stack->top_card = moving_card->next;
  from_stack->top_card->previous = NULL;
  if(from_stack->top_card == NULL)
  {
    from_stack->bottom_card == NULL;
  }
  addCardToStackTop(moving_card, &to_stack);

}

int initialHandOut ()
{
  for(int offset = 0; offset < 4; offset++)
  {
    printf("offset: %i\n", offset);
    for(int distribute = (0 + offset); distribute < 4; distribute++)
    {
      printf("distribute: %i\n", distribute);
      moveCard(&stack_array[DRAW_STACK], &stack_array[distribute + 1]);
    }
  }
}

ReturnValue checkCard (char color, int value)
{
  if(color == 'R')
  {
    value += 13;
  }
  ReturnValue returnValue = (++(initialized_cards[value - 1]) == 1) ? EVERYTHING_OK : INVALID_FILE;
  return returnValue;
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
  int card_count = 0;
  while(!feof(fp))
  {
    int current_character = fgetc(fp);
    //printf("current_character: %c, status: %i\n", current_character, status);
    if(current_character == ' ' || current_character == '\n')
    {
      //printf("continue\n");
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
        ReturnValue check_card_return = checkCard(current_color, current_value);
        if(check_card_return != EVERYTHING_OK)
        {
          return check_card_return;
        }
        card_count++;
        //printf("color: %c, value: %i\n",current_color, current_value);
        Card *new_card;
        new_card = malloc(sizeof(Card));
        new_card->color = current_color;
        new_card->value = current_value;
        addCardToStackTop(new_card, &stack_array[DRAW_STACK]);
        status = 0;
        break;
      case -1:
      default:
        return INVALID_FILE;

    }
  }
  ReturnValue return_value = (card_count == 26) ? EVERYTHING_OK : INVALID_FILE;
  return return_value;
}
int printStack(Stack *stack)
{
  Card *actual_card = stack->bottom_card;
  while(actual_card != NULL)
  {
    printf("%c %i\n", actual_card->color, actual_card->value);
    actual_card = actual_card->previous;
  }
  return 0;
}

int printMatchfield ()
{
  printf("0   | 1   | 2   | 3   | 4   | DEP | DEP\n");
  printf("---------------------------------------\n");
  Stack stack_array_save[] = stack_array;
  for(int current_matchfield_row = 0; current_matchfield_row < MATCHFIELDHEIGHT;
    current_matchfield_row++)
  {

    for(int current_col; current_col < MAXSTACK; current_col++)
    {


      printf("%c%i%c  |",);

    }
    printf("\n");
  }
}