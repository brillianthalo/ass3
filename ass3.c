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
#include <string.h>
#include <stdlib.h>

#define MAXSTACK 7
#define DRAW_STACK 0
#define GAME_STACK_1 1
#define GAME_STACK_2 2
#define GAME_STACK_3 3
#define GAME_STACK_4 4
#define DEP_STACK_1 5
#define DEP_STACK_2 6
#define MATCHFIELDHEIGHT 16
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
typedef enum _ReturnState_
{
  OK = 0,
  INVALID_ARGS = -1,
  INVALID_CONFIG_FILE = -2,
} ReturnState;



//forward function declaration
ReturnState readInitFile(const char *file, Stack *draw_stack);
int initStackArray();
int moveCard(Stack *from_stack, Stack *to_stack);
int initialHandOut();

ReturnState printErrorMessage(ReturnState return_value);
int printStack(Stack *stack);
int printMatchfield();

int main(int argc, char* argv[]) {

  Stack stack_array[7] = {{NULL, NULL},
                          {NULL, NULL},
                          {NULL, NULL},
                          {NULL, NULL},
                          {NULL, NULL},
                          {NULL, NULL},
                          {NULL, NULL}};
//  initializeStacks();
//  initStackArray();
  printf("argv: %s\n", argv[1]);
  ReturnState return_value = readInitFile(argv[1], &(stack_array[DRAW_STACK]));

  if(return_value != OK)
  {
    return printErrorMessage(return_value);
  }

  initialHandOut(&stack_array);
  //printStack(&stack_array[DRAW_STACK]);
  printMatchfield(&stack_array);

 // while(exit_status)
  //{
  //  fgets()
  //}
  //printf("top card: %c %i, bottom card: %c %i\n", stack_array[DRAW_STACK].top_card->color, stack_array[DRAW_STACK].top_card->value, stack_array[DRAW_STACK].bottom_card->color, stack_array[DRAW_STACK].bottom_card->value);
  return 0;
}
/*
int initStackArray(Stack *stack_array)
{
  for(int stack_number = 0; stack_number < MAXSTACK; stack_number++)
  {
    Stack act_stack = *(stack_array + stack_number);
    act_stack.top_card = NULL;
    act_stack.bottom_card = NULL;
  }
  return 0;
}*/

ReturnState printErrorMessage(ReturnState return_value)
{
  switch(return_value)
  {
    case INVALID_ARGS:
      printf("[ERR] Wrong number of parameters. Usage ./ass1 <config-file>\n");
      break;
    case INVALID_CONFIG_FILE:
      printf("[ERR] Config-File is invalid.\n");
      break;

    case OK:
      //left blank because nothing should happen
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
  return 0;
}

int moveCard(Stack *from_stack, Stack *to_stack)
{
  Card *moving_card = from_stack->top_card;
  from_stack->top_card = moving_card->next;
  from_stack->top_card->previous = NULL;
  if(from_stack->top_card == NULL)
  {
    from_stack->bottom_card = NULL;
  }
  addCardToStackTop(moving_card, to_stack);
  return 0;
}

int initialHandOut (Stack *stack_array)
{
  for(int offset = 0; offset < 4; offset++)
  {
    //printf("offset: %i\n", offset);
    for(int distribute = (1 + offset); distribute <= 4; distribute++)
    {
     // printf("distribute: %i\n", distribute);
      moveCard(&stack_array[DRAW_STACK], &stack_array[distribute]);
    }
  }
  return 0;
}

ReturnState checkCard (char color, int value)
{
  static int initialized_cards[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  if(color == 'R')
  {
    value += 13;
  }
  ReturnState returnValue = (++(initialized_cards[value - 1]) == 1) ? OK : INVALID_CONFIG_FILE;
  return returnValue;
}
ReturnState readInitFile(const char *path, Stack *stack)
{
  FILE *fp = fopen(path, "r");
  if(fp == NULL)
  {
    printf("file is NULL\n");
    return INVALID_CONFIG_FILE;
  }
  //("readInitFile\n");
  char current_color;
  int current_value;
  int status = 0;
  int card_count = 0;
  while(1)
  {
    if(feof(fp))
    {
      ReturnState return_value = (card_count == 26) ? OK : INVALID_CONFIG_FILE;
      return return_value;
    }
    int current_character = fgetc(fp);
    printf("current_character: %i = %c, status: %i\n", current_character, current_character, status);
    if(current_character == ' ' || current_character == 10 || current_character == 13)
    {
      printf("continue\n");
      continue;
    }
    printf("current_character: %c post continue\n", current_character);
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
              return INVALID_CONFIG_FILE;
            }
            break;
          default:
            return INVALID_CONFIG_FILE;
        }
        ReturnState check_card_return = checkCard(current_color, current_value);
        if(check_card_return != OK)
        {
          return check_card_return;
        }
        card_count++;
        //printf("color: %c, value: %i\n",current_color, current_value);
        Card *new_card;
        new_card = malloc(sizeof(Card));
        new_card->color = current_color;
        new_card->value = current_value;
        addCardToStackTop(new_card, stack);
        status = 0;
        break;
      case -1:
      default:
        return INVALID_CONFIG_FILE;

    }
  }

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

int printMatchfield (Stack *stack_array)
{
  printf("0   | 1   | 2   | 3   | 4   | DEP | DEP\n");
  printf("---------------------------------------\n");
  Card* current_pointer[MAXSTACK];
  for(int pointer_copy = 0; pointer_copy < MAXSTACK; pointer_copy++)
  {
    *(current_pointer + pointer_copy) = (stack_array + pointer_copy)->bottom_card;
  }
  for(int current_matchfield_row = 0; current_matchfield_row < MATCHFIELDHEIGHT;
    current_matchfield_row++)
  {
    for(int current_col = 0; current_col < MAXSTACK; current_col++)
    {
      char current_col_end[4] = "\0\0\0\0";
      char current_color;
      int current_value;
      char current_value_to_s[3] = "\0\0\0";
      //printf("current_col: %i", current_col);
      (current_col != (MAXSTACK - 1)) ? strcpy(current_col_end, " | ") : strcpy(current_col_end, "\0\0\0\0");
      Card* current_card = *(current_pointer + current_col);
      if(current_card == NULL)
      {
        current_color = ' ';
        strcpy(current_value_to_s, "  ");
      }
      else if (current_col == DRAW_STACK && current_card->previous != NULL)
      {
        current_color = 'X';
        strcpy(current_value_to_s, "  ");
      }
      else
      {
        current_color = current_card->color;
        current_value = current_card->value;
        switch (current_value)
        {
          case 1:
            strcpy(current_value_to_s, "A ");
            break;
          case 10:
            strcpy(current_value_to_s, "10");
            break;
          case 11:
            strcpy(current_value_to_s, "J ");
            break;
          case 12:
            strcpy(current_value_to_s, "Q ");
            break;
          case 13:
            strcpy(current_value_to_s, "K ");
            break;
          default:
            current_value_to_s[0] = '0' + current_value;
            current_value_to_s[1] = ' ';
            current_value_to_s[2] = '\0';
            break;
        }

      }
      printf("%c%s%s", current_color, current_value_to_s, current_col_end);
      current_pointer[current_col] = (current_card != NULL)
        ? current_card->previous : NULL;

    }
    printf("\n");
  }
  return 0;
}