//TODO:
//  - config einlesen und überprüfen CHECK
//  - Fehlerverwaltung
//      - out of memory
//      - invalid file
//      - invalid command
//  - Initaler Spielstart
//        - Erstes Kartenausteilen CHECK
//        - Spielfedgenerierung CHECK
//        - Userinput
//  - Usereingaben einlesen und verarbeiten JAKOB
//  - Spielmechaniken implementieren JAKOB
//  - help implementieren CHECK
//  - exit UND EOF implementieren
//  - win- und lose-condition implementieren
//  - DOKUMENTATION
//


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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
  struct _Stack_ *stack;
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
  INVALID_ARGS = 1,
  OUT_OF_MEMORY = 2,
  INVALID_CONFIG_FILE = 3,
} ReturnState;
typedef enum _InfoState_ {
  VALID_COMMAND = 0,
  INVALID_COMMAND = -1,
  INVALID_MOVE = -2
} InfoState;


//forward function declaration
ReturnState readInitFile(const char *path, Stack *draw_stack);
int movePile(Stack *moving_pile, Stack *to_stack);
int initialHandOut(Stack *stack_array);

ReturnState printErrorMessage(ReturnState return_value);
//int printStack(Stack *stack);
int printMatchfield(Stack *stack_array);
int printHelp();
Stack* findCardPileByColorValue(char color, int value, Stack *stack_array);
InfoState printInfoMessage(InfoState info_message);
int printHelp();
int freeAllCardMems(Stack *stack_array);
int exitGame(Stack *stack_array);
int getValueAsInt(char current_character);
char* ignoreBlankspaces(char *string);

int main(int argc, char* argv[]) {
  int exit_status = 1;

  char input_color;
  int input_value;
  int input_stack;
  Stack stack_array[7] = {{NULL, NULL},
                          {NULL, NULL},
                          {NULL, NULL},
                          {NULL, NULL},
                          {NULL, NULL},
                          {NULL, NULL},
                          {NULL, NULL}};

  //printf("argv: %s\n", argv[1]);
  ReturnState return_value = readInitFile(argv[1], &(stack_array[DRAW_STACK]));

  if(return_value != OK)
  {
    return printErrorMessage(return_value);
  }

  initialHandOut(stack_array);
  /*Stack* test_move = findCardPileByColorValue('B', 10, stack_array);
  if(test_move == NULL)
  {
    printErrorMessage(OUT_OF_MEMORY);
    return 1;
  }
  else if(test_move->bottom_card == NULL)
  {
    printInfoMessage(INVALID_MOVE);
  }
  else
  {
    movePile(test_move, stack_array + GAME_STACK_2);
    printf("\nhard coded moving of B9 to GAMESTACK 2\n\n");
  }
  printMatchfield(stack_array);
  test_move = findCardPileByColorValue('R', 6, stack_array);
  if(test_move == NULL)
  {
    printf("not valid move");
  }
  else
  {
    movePile(test_move, stack_array + GAME_STACK_4);
    printf("\nhard coded moving of R6 to GAMESTACK 4\n\n");
  }
  printMatchfield(stack_array);*/
  printMatchfield(stack_array);
  while(exit_status)
  {
    //char input_comm[5] = "\0";

    printf("esp> ");

    char *input_memory_location = calloc(1, 20);

    unsigned int current_input_size = 20;
    if(input_memory_location != NULL)
    {
      char current_input_char = EOF;
      unsigned int current_need_size = 0;
    while((current_input_char = getchar()) != '\n' && current_input_char != EOF)
      {
        /*if(current_input_char == ' ')
        {
          continue;
        }*/
        //printf("current_input_char: %i -> %c\n", current_input_char, current_input_char);
        input_memory_location[current_need_size] = toupper(current_input_char);
        current_need_size++;
        if(current_need_size == current_input_size)
        {
          input_memory_location = realloc(input_memory_location, ++current_input_size);
        }
      }
      //printf("input_mod: %s\n", input_memory_location);
    }
    if(current_input_size == 0)
    {
      continue;
    }
    input_memory_location[current_input_size] = '\0';
    char *input = input_memory_location;

    //printf("command: %s\n", input_comm);
    input = ignoreBlankspaces(input);
    if(strncmp(input, "HELP", 4) == 0)
    {
      printHelp();
    }
    else if(strncmp(input, "MOVE", 4) == 0)
    {
      printf("input command is move\n");
      input += 4;
      input = ignoreBlankspaces(input);
      if(strncmp(input, "RED", 3) == 0)
      {
        input_color = 'R';
        //printf("color: %c", input_color);
        input += 3;
      }
      else if(strncmp(input, "BLACK", 5) == 0)
      {
        input_color = 'B';
        //printf("color: %c", input_color);
        input += 5;
      }
      else
      {
        free(input_memory_location);
        printInfoMessage(INVALID_COMMAND);
        continue;
      }
      input = ignoreBlankspaces(input);
      if(input[0] == '1' && input[1] == '0')
      {
        input_value = 10;
        input += 2;
      }
      else if((input_value = getValueAsInt(*input)) > 0)
      {
        printf("value as int: %i", input_value);
        input += 1;
      } 
      else
      {
        free(input_memory_location);
        printInfoMessage(INVALID_COMMAND);
        continue;
      }
      input = ignoreBlankspaces(input);
      if(strncmp(input, "TO", 2) == 0)
      {
        input += 2;
      }
      else
      {
        free(input_memory_location);
        printInfoMessage(INVALID_COMMAND);
        continue;
      }
      input = ignoreBlankspaces(input);
      if(input[0] > '0' && input[0] < '7')
      {
        input_stack = input[0] - '0';
        input++;
      }
      else
      {
        free(input_memory_location);
        printInfoMessage(INVALID_COMMAND);
        continue;
      }
      input = ignoreBlankspaces(input);
      if (input[0] != '\0')
      {
        free(input_memory_location);
        printInfoMessage(INVALID_COMMAND);
        continue;
      }
      Stack *pile_to_move = findCardPileByColorValue(input_color, input_value, stack_array);
      if(pile_to_move == NULL)
      {
        printErrorMessage(OUT_OF_MEMORY);
        return OUT_OF_MEMORY;
      }
      else if (pile_to_move->bottom_card == NULL)
      {
        printInfoMessage(INVALID_MOVE);
        continue;
      }
      movePile(pile_to_move, &stack_array[input_stack]);
      free(input_memory_location);
      printMatchfield(stack_array);
    }
    else if(strncmp(input, "EXIT", 4) == 0)
    {
      freeAllCardMems(stack_array);
      exit_status = 0;
    }
    else
    {
        printInfoMessage(INVALID_COMMAND);
    }  
  }
  //printf("top card: %c %i, bottom card: %c %i\n", stack_array[DRAW_STACK].top_card->color, stack_array[DRAW_STACK].top_card->value, stack_array[DRAW_STACK].bottom_card->color, stack_array[DRAW_STACK].bottom_card->value);
  return 0;
}

char* ignoreBlankspaces(char *string)
{
  while(string[0] == ' ')
  {
    string++;
  }
  return string;
}

int freeAllCardMems(Stack *stack_array)
{
  for(int current_stack = 0; current_stack < MAXSTACK; current_stack++)
  {
    Card *head_card = (stack_array + current_stack)->top_card;
    Card *tmp_card;
    while(head_card != NULL)
    {
      tmp_card = head_card;
      head_card = head_card->next;
      free(tmp_card);
    }
  }
  return 0;
}

int printHelp()
{
  printf("possible command:\n - move <color> <value> to <stacknumber>\n - help\n - exit\n");
  return 0;
}

ReturnState printErrorMessage(ReturnState return_value)
{
  switch(return_value)
  {
    case INVALID_ARGS:
      printf("[ERR] Usage ./ass3 [file-name]\n");
      break;
    case INVALID_CONFIG_FILE:
      printf("[ERR] Invalid File!\n");
      break;
    case OUT_OF_MEMORY:
      printf("[ERR] Out of memory.\n");
      break;
    case OK:
      //left blank because nothing should happen
      break;
  }
  return return_value;
}

InfoState printInfoMessage(InfoState info_message)
{
  switch(info_message)
  {
    case INVALID_COMMAND:
      printf("[INFO] Invalid command!\n");
      break;
    case INVALID_MOVE:
      printf("[INFO] Invalid move command!\n");
      break;
    case VALID_COMMAND:
      //left blank because nothing should happen
      break;
  }
  return info_message;
}

Stack* makeSingleCardToPile(Card *card)
{
  Stack *pile = malloc(sizeof(Stack));
  pile->bottom_card = card;
  pile->top_card = card;
  return pile;
}

int addPileToStackTop(Stack *add_pile, Stack *card_stack)
{
  Card *new_card = add_pile->bottom_card;
  new_card->next = card_stack->top_card;
  if(card_stack->top_card != NULL)
  {
    card_stack->top_card->previous = new_card;
  }
  card_stack->top_card = add_pile->top_card;
  if(card_stack->bottom_card == NULL)
  {
    card_stack->bottom_card = new_card;
  }
  while(new_card != NULL)
  {
    new_card->stack = card_stack;
    new_card = new_card->previous;
  }
  free(add_pile);
  return 0;
}

Stack* findCardPileByColorValue (char color, int value, Stack *stack_array)
{
  Stack *moving_pile = malloc(sizeof(Stack));
  if(moving_pile == NULL)
  {
    return moving_pile;
  }

  moving_pile->top_card = (stack_array + DRAW_STACK)->top_card;
  moving_pile->bottom_card = moving_pile->top_card;
  Card* current_card = moving_pile->bottom_card;
  if (current_card->color == color && current_card->value == value)
  {
    return moving_pile;
  }
  for(int current_stack_number = 1; current_stack_number < (MAXSTACK - 2);
    current_stack_number++)
  {
    moving_pile->top_card = (stack_array + current_stack_number)->top_card;
    current_card = (stack_array + current_stack_number)->top_card;
    while(current_card != NULL)
    {
      if(current_card->color == color && current_card->value == value)
      {
        moving_pile->bottom_card = current_card;
        return moving_pile;
      }
      current_card = current_card->next;
    }
  }
  moving_pile->top_card = NULL;
  moving_pile->bottom_card = NULL;
  return moving_pile;
}

int movePile(Stack *moving_pile, Stack *to_stack)
{
  Card* moving_card = moving_pile->bottom_card;
  moving_card->stack->top_card = moving_card->next;
  if(moving_card->next != NULL)
  {
    moving_card->next->previous = NULL;
  }
  else
  {
    moving_card->stack->bottom_card = NULL;
  }
  addPileToStackTop(moving_pile, to_stack);
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
      Stack *move_pile = makeSingleCardToPile((stack_array + DRAW_STACK)->top_card);
      movePile(move_pile, &stack_array[distribute]);
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
int getValueAsInt(char current_character)
{
  int current_value;
  static int was_current_character_one;
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
      was_current_character_one = 0;
      current_value = current_character - '0';
      break;
    case 'A':
      was_current_character_one = 0;
      current_value = 1;
      break;
    case 'J':
      was_current_character_one = 0;
      current_value = 11;
      break;
    case 'Q':
      was_current_character_one = 0;
      current_value = 12;
      break;
    case 'K':
      was_current_character_one = 0;
      current_value = 13;
      break;
    case '1':
      was_current_character_one = 1;
      current_value = 0;
      break;
    case '0':
      if(was_current_character_one == 1)
      {
        current_value = 10;
      }
      else
      {
        return -1;
      }
      break;
    default:
      was_current_character_one = 0;
      return -1;
  }
  return current_value;
}

Card* createNewCard(char color, int value, Stack *stack)
{
  Card *new_card;
  new_card = malloc(sizeof(Card));
  if(new_card == NULL)
  {
    return NULL;
  }
  new_card->color = color;
  new_card->value = value;
  new_card->stack = stack;
  new_card->next = NULL;
  new_card->previous = NULL;
  return new_card;
}

ReturnState readInitFile(const char *path, Stack *stack)
{
  FILE *fp = fopen(path, "r");
  if(fp == NULL)
  {
    return INVALID_CONFIG_FILE;
  }
  //("readInitFile\n");
  char current_color;
  int current_value;
  int status = 1;
  int card_count = 0;
  while(!feof(fp))
  {
    int current_character = fgetc(fp);
    //printf("current_character: %i = %c, status: %i\n", current_character, current_character, status);
    if(current_character == ' ' ||  current_character == 13)
    {
      //printf("continue\n");
      continue;
    }
    //printf("current_character: %c post continue\n", current_character);
    switch(status)
    {
      case 0:
        status = -1;
        if(current_character == 10)
        {
          status = 1;
        }
        break;
      case 1:
        status = -1;
        if(current_character == 'B')
        {
          status = 2;
        }
        else if (current_character == 'R')
        {
          status = 6;
        }
        break;
      case 2:
        status = -1;
        if(current_character == 'L')
        {
          status = 3;
        }
        break;
      case 3:
        status = -1;
        if(current_character == 'A')
        {
          status = 4;
        }
        break;
      case 4:
        status = -1;
        if(current_character == 'C')
        {
          status = 5;
        }
        break;
      case 5:
        status = -1;
        if(current_character == 'K')
        {
          current_color = 'B';
          status = 8;
        }
        break;
      case 6:
        status = -1;
        if(current_character == 'E')
        {
          status = 7;
        }
        break;
      case 7:
        status = -1;
        if(current_character == 'D')
        {
          current_color = 'R';
          status = 8;
        }
        break;
      case 8:
        current_value = getValueAsInt(current_character);
        if(current_value == 0)
        {
          continue;
        }
        ReturnState check_card_return = checkCard(current_color, current_value);
        if(check_card_return != OK)
        {
          return check_card_return;
        }
        card_count++;
        //printf("color: %c, value: %i\n",current_color, current_value);
        Card *new_card = createNewCard(current_color, current_value, stack);
        if(new_card == NULL)
        {
          return OUT_OF_MEMORY;
        }
        Stack *new_pile = makeSingleCardToPile(new_card);
        if(new_pile == NULL)
        {
          return OUT_OF_MEMORY;
        }
        addPileToStackTop(new_pile, stack);
        status = 0;
        break;
      case -1:
      default:
        return INVALID_CONFIG_FILE;

    }
  }
  ReturnState return_value = (card_count == 26) ? OK : INVALID_CONFIG_FILE;
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

int getValueAsString(int value, char *current_value_to_s)
{
  switch (value)
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
      current_value_to_s[0] = '0' + value;
      current_value_to_s[1] = ' ';
      current_value_to_s[2] = '\0';
      break;
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
      char current_col_end[4] = "\0\0\0";
      char current_color;
      int current_value;
      char current_value_to_s[3] = "\0\0";
      //printf("current_col: %i", current_col);
      (current_col != (MAXSTACK - 1)) ? strcpy(current_col_end, " | ")
        : strcpy(current_col_end, "\0");
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
        getValueAsString(current_value, current_value_to_s);
      }
      printf("%c%s%s", current_color, current_value_to_s, current_col_end);
      current_pointer[current_col] = (current_card != NULL)
        ? current_card->previous : NULL;

    }
    printf("\n");
  }
  return 0;
}