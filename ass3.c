//----------------------------------------------------------------------------
// ass3.c
//
// A Solitaire like Card Game
// 
// Group: Group 2 - study assistant Martin Ferdinand Haubenwallner
//
// Authors: Kilian Domes 11801231
// Jakob Dorighi 11823384
//
// Latest Changes: 23.12.2018 (by Kilian Domes)
//----------------------------------------------------------------------------
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXSTACK 7
#define DRAW_STACK 0
#define MATCHFIELDHEIGHT 16

//structure of a card with its color and value stored and the stack on which it
//currently lies. This structur is designed for generating a double linked list
//with the Card pointer to the next and previous card.
typedef struct _Card_
{
  char color_;
  int value_;
  struct _Stack_ *stack_;
  struct _Card_ *next_;
  struct _Card_ *previous_;

} Card;

//structure of a stack with the pointer to its top_card (the card which in
//reallity would be on top of a stack. I. e. the card you can draw of the
// draw stack), the pointer to its bottom card (the card lying on the
// desk/table) and the type of the stack (i.e. DRAW_STACK, GAME_STACK or
// DEPOSIT_STACK).
//In the further code there will be to names use to describe a struct Stack:
//Stack: is a non-movable Stack like the three types exampled above.
//Pile: a temporarily generated stack structure to simplify the moving process
//of one or more cards, with the pointer to its top card and the pointer to its
//bottom card. If you just move one card the bottom and top card pointer is the
//same.
typedef struct _Stack_ {
  Card *top_card_;
  char *stack_type_;
  Card *bottom_card_;
} Stack;

//possible returns of the programm on exit or for info messages (i.e. bad move)
//this code is adapted from the code of the assignment 1 template (ass1.c)
//written by Florian Bernhardt
typedef enum _ReturnState_
{
  OK = 0,
  INVALID_ARGS = 1,
  OUT_OF_MEMORY = 2,
  INVALID_CONFIG_FILE = 3,
  INVALID_COMMAND = -1,
  INVALID_MOVE = -2
} ReturnState;

//------------------------------------------------------------------------------
///
/// Function to find the next character in a string which is not a space char
///
/// @param char *string string to find the start of the next word in 
///
/// @return string pointer to the next word 
//

char *ignoreBlankspaces(char *string)
{
  while(string[0] == ' ')
  {
    string++;
  }
  return string;
}

//------------------------------------------------------------------------------
///
/// Function to set free the used heap memory for the cards
///
/// @param Stack *stack_array array of stacks to be able to access all *Cards
//

void freeAllCardMemories(Stack *stack_array)
{
  for(int current_stack = 0; current_stack < MAXSTACK; current_stack++)
  {
    Card *current_card = (stack_array + current_stack)->top_card_;
    Card *saved_card_pointer;
    while(current_card != NULL)
    {
      saved_card_pointer = current_card;
      current_card = current_card->next_;
      free(saved_card_pointer);
    }
  }
}

//------------------------------------------------------------------------------
///
/// Function for printing error messages
///
/// @param ReturnState return_value which error message to print
///
/// @return ReturnState return_value to end the program
///                     with the right return value
//

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
    case INVALID_COMMAND:
      printf("[INFO] Invalid command!\n");
      break;
    case INVALID_MOVE:
      printf("[INFO] Invalid move command!\n");
      break;
    case OK:
      //left blank because nothing should happen
      break;
  }
  return return_value;
}

//------------------------------------------------------------------------------
///
/// Function to transform a single card into a pile (struct Stack)
///
/// @param Card *card card card to make into a pile
///
/// @return Stack *pile stack of the 1 card
//

Stack *makeSingleCardToPile(Card *card)
{
  Stack *pile = malloc(sizeof(Stack));
  if(pile == NULL)
  {
    return NULL;
  }
  pile->bottom_card_ = card;
  pile->top_card_ = card;
  return pile;
}

//------------------------------------------------------------------------------
///
/// Function to add multiple or just one Card to a Stack
///
/// @param Stack *add_pile which pile to add to the stack
/// @param Stack *to_stack which stack to add it to 
//

void addPileToStackTop(Stack *add_pile, Stack *to_stack)
{
  Card *moving_bottom_card = add_pile->bottom_card_;
  moving_bottom_card->next_ = to_stack->top_card_;
  if(to_stack->top_card_ != NULL)
  {
    to_stack->top_card_->previous_ = moving_bottom_card;
  }
  to_stack->top_card_ = add_pile->top_card_;
  if(to_stack->bottom_card_ == NULL)
  {
    to_stack->bottom_card_ = moving_bottom_card;
  }
  while(moving_bottom_card != NULL)
  {
    moving_bottom_card->stack_ = to_stack;
    moving_bottom_card = moving_bottom_card->previous_;
  }
  free(add_pile);
}

//------------------------------------------------------------------------------
///
/// Function to determine if the move command of the user is valid
///
/// @param Stack *moving_pile pile to move
/// @param Stack *destination_stack stack to move to
///
/// @return ReturnState if the move is invalid or ok

ReturnState checkValidMove(Stack *moving_pile, Stack *destination_stack)
{
  Card *moving_card = moving_pile->bottom_card_;
  char *stack_type = destination_stack->stack_type_;
  Card *destination_card = destination_stack->top_card_;

  if (strcmp(destination_stack->stack_type_, "DRAW") == 0)
  {
    free(moving_pile);
    return INVALID_MOVE;
  }
  else if (destination_card == NULL) {
    if (strcmp(destination_stack->stack_type_, "DEPOSIT")
      == 0 && moving_card->value_ != 1) {
      free(moving_pile);
      return INVALID_MOVE;
    }
    else if (strcmp(destination_stack->stack_type_, "GAME") == 0
      && moving_card->value_ != 13)
    {
      free(moving_pile);
      return INVALID_MOVE;
    }
    else
    {
      return OK;
    }
  }
  else if (strcmp(stack_type, "DEPOSIT") == 0)
  {
    if(moving_card->value_ != (destination_card->value_ + 1)
       || moving_card->color_ != destination_card->color_)
    {
      free(moving_pile);
      return INVALID_MOVE;
    }
  }
  else
  {
    if (moving_card->value_ != (destination_card->value_ - 1)
      || moving_card->color_ == destination_card->color_)
    {
      free(moving_pile);
      return INVALID_MOVE;
    }
  }
  return OK;
}

//------------------------------------------------------------------------------
///
/// Function to find a Card by its color and value and returning the Pile
/// containing the found Card pointer as bottom_card_ and the last of all cards
/// above as top_card_
///
/// @param char color color of the card to search for
/// @param int value value of the card to search for
/// @param Stack *stack_array array of all card stacks
///
/// @return Stack *moving_pile pile where the matching card is its bottom card
//

Stack *findCardPileByColorValue (char color, int value, Stack *stack_array)
{
  Stack *moving_pile = malloc(sizeof(Stack));
  Card *current_card;
  if(moving_pile == NULL)
  {
    return moving_pile;
  }
  if((stack_array + DRAW_STACK)->top_card_ != NULL)
  {
    moving_pile->top_card_ = (stack_array + DRAW_STACK)->top_card_;
    moving_pile->bottom_card_ = moving_pile->top_card_;
    current_card = moving_pile->bottom_card_;
    if (current_card->color_ == color && current_card->value_ == value)
    {
      return moving_pile;
    }
  }
  for(int current_stack_number = 1; current_stack_number < (MAXSTACK - 2);
    current_stack_number++)
  {
    moving_pile->top_card_ = (stack_array + current_stack_number)->top_card_;
    current_card = (stack_array + current_stack_number)->top_card_;
    while(current_card != NULL)
    {
      if(current_card->color_ == color && current_card->value_ == value)
      {
        moving_pile->bottom_card_ = current_card;
        return moving_pile;
      }
      current_card = current_card->next_;
      if(current_card != NULL &&
        (current_card->color_ == current_card->previous_->color_ ||
        current_card->value_ != current_card->previous_->value_ + 1))
      {
        break;
      }
    }
  }
  moving_pile->top_card_ = NULL;
  moving_pile->bottom_card_ = NULL;
  return moving_pile;
}

//------------------------------------------------------------------------------
///
/// Function to move a pile of cards to another stack
///
/// @param the pile to move 
/// @param the stack to move the pile to
//

void movePile(Stack *pile_to_move, Stack *to_stack)
{
  Card *card_to_move = pile_to_move->bottom_card_;
  card_to_move->stack_->top_card_ = card_to_move->next_;
  if(card_to_move->next_ != NULL)
  {
    card_to_move->next_->previous_ = NULL;
  }
  else
  {
    card_to_move->stack_->bottom_card_ = NULL;
  }
  addPileToStackTop(pile_to_move, to_stack);
}

//------------------------------------------------------------------------------
///
/// Function to deal out the initial cards
///
/// @param Stack *stack_array array of all stacks
//
void initialHandOut (Stack *stack_array)
{
  for(int offset = 0; offset < 4; offset++)
  {
    for(int distribute = (1 + offset); distribute <= 4; distribute++)
    {
     // printf("distribute: %i\n", distribute);
      Stack *pile_to_move =
        makeSingleCardToPile((stack_array + DRAW_STACK)->top_card_);
      movePile(pile_to_move, stack_array + distribute);
    }
  }
}

//------------------------------------------------------------------------------
///
/// Function to check if there is no double card in the config file
///
/// @param char color color of the card 
/// @param int value value of the card
///
/// @return Returnstate is the config file ok or invalid
//

ReturnState checkCard (char color, int value)
{
  static int initialized_cards[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  if(color == 'R')
  {
    value += 13;
  }
  ReturnState returnValue = (++(initialized_cards[value - 1]) == 1)
    ? OK : INVALID_CONFIG_FILE;
  return returnValue;
}

//------------------------------------------------------------------------------
///
/// Function to transform the card value as character into integer values
///
/// @param char current_character the character that has to be transformed
///
/// @return int value of the card as integer with a special treatment of the
///         value "10"
//

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

//------------------------------------------------------------------------------
///
/// Function to create a new card
///
/// @param char color color of the card
/// @param int value value of the card 
/// @param Stack *stack stack to put the card on
///
/// @return *Card pointer to created card
//

Card *createNewCard(char color, int value, Stack *stack)
{
  Card *new_card = malloc(sizeof(Card));
  if(new_card == NULL)
  {
    return NULL;
  }
  new_card->color_ = color;
  new_card->value_ = value;
  new_card->stack_ = stack;
  new_card->next_ = NULL;
  new_card->previous_ = NULL;
  return new_card;
}

//------------------------------------------------------------------------------
///
/// Function to read the config file 
///
/// @param const char *path_to_config_file filepath of the config file
/// @param Stack *draw_stack stack to put the initialized cards on
///
/// @return ReturnState is the config file invalid or ok
//

ReturnState readInitFile(const char *path_to_config_file, Stack *draw_stack)
{
  FILE *file_pointer = fopen(path_to_config_file, "r");
  if(file_pointer == NULL)
  {
    return INVALID_CONFIG_FILE;
  }
  //("readInitFile\n");
  char current_color;
  int current_value;
  int status = 1;
  int card_count = 0;
  while(!feof(file_pointer))
  {
    int current_character = fgetc(file_pointer);
    if(current_character == '\r')
    {
      continue;
    }
    switch(status)
    {
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
        else if (current_character == '\n' || current_character == ' ')
        {
          status = 1;
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
        if(current_character == ' ')
        {
          continue;
        }
        current_value = getValueAsInt(current_character);
        if(current_value == 0)
        {
          current_character = fgetc(file_pointer);
          current_value = getValueAsInt(current_character);
        }
        ReturnState check_card_return = checkCard(current_color, current_value);
        if(check_card_return != OK)
        {
          return check_card_return;
        }
        card_count++;
        Card *new_card =
          createNewCard(current_color, current_value, draw_stack);
        if(new_card == NULL)
        {
          return OUT_OF_MEMORY;
        }
        Stack *new_pile = makeSingleCardToPile(new_card);
        if(new_pile == NULL)
        {
          return OUT_OF_MEMORY;
        }
        addPileToStackTop(new_pile, draw_stack);
        status = 1;
        break;
      case -1:
      default:
        return INVALID_CONFIG_FILE;

    }
  }
  ReturnState return_value = (card_count == 26) ? OK : INVALID_CONFIG_FILE;
  fclose(file_pointer);
  return return_value;
}

//------------------------------------------------------------------------------
///
/// Function to convert the integer value of a card back to the face value [A-K]
/// as string
///
/// @param int value value of the card 
/// @param chat *pointer_to_result_string pointer to the string where the
///        result of the function has to be copied in
//

void getValueAsString(int value, char *pointer_to_result_string)
{
  switch (value)
  {
    case 1:
      strcpy(pointer_to_result_string, "A ");
      break;
    case 10:
      strcpy(pointer_to_result_string, "10");
      break;
    case 11:
      strcpy(pointer_to_result_string, "J ");
      break;
    case 12:
      strcpy(pointer_to_result_string, "Q ");
      break;
    case 13:
      strcpy(pointer_to_result_string, "K ");
      break;
    default:
      pointer_to_result_string[0] = '0' + value;
      pointer_to_result_string[1] = ' ';
      pointer_to_result_string[2] = '\0';
      break;
  }
}

//------------------------------------------------------------------------------
///
/// Function to print the game layout 
///
/// @param Stack *stack_array array of the game stacks
//

void printMatchfield (Stack *stack_array)
{
  printf("0   | 1   | 2   | 3   | 4   | DEP | DEP\n");
  printf("---------------------------------------\n");
  Card *current_pointer[MAXSTACK];
  for(int pointer_copy = 0; pointer_copy < MAXSTACK; pointer_copy++)
  {
    current_pointer[pointer_copy] = (stack_array + pointer_copy)->bottom_card_;
  }
  for(int current_matchfield_row = 0; current_matchfield_row < MATCHFIELDHEIGHT;
    current_matchfield_row++)
  {
    for(int current_col = 0; current_col < MAXSTACK; current_col++)
    {
      char current_column_end[4] = "\0";
      char current_color;
      int current_value;
      char current_value_to_string[3] = "\0";
      (current_col != (MAXSTACK - 1)) ? strcpy(current_column_end, " | ")
        : strcpy(current_column_end, "\0");
      Card *current_card = current_pointer[current_col];
      if(current_card == NULL)
      {
        current_color = ' ';
        strcpy(current_value_to_string, "  ");
      }
      else if (current_col == DRAW_STACK && current_card->previous_ != NULL)
      {
        current_color = 'X';
        strcpy(current_value_to_string, "  ");
      }
      else
      {
        current_color = current_card->color_;
        current_value = current_card->value_;
        getValueAsString(current_value, current_value_to_string);
      }
      printf("%c%s%s", current_color,
        current_value_to_string, current_column_end);
      current_pointer[current_col] = (current_card != NULL)
        ? current_card->previous_ : NULL;

    }
    printf("\n");
  }
}

//------------------------------------------------------------------------------
///
/// Function for evaluating and executing the "MOVE" user command
///
/// @param char *input pointer to the input-string
/// @param Stack *stack_array pointer to array of stacks
///
/// @return Returnstate is the command invalid or ok
//

ReturnState doMove(char *input, Stack *stack_array)
{
  char input_color;
  int input_value;
  int input_stack;
  input = ignoreBlankspaces(input);
  if(strncmp(input, "RED", 3) == 0)
  {
    input_color = 'R';
    input += 3;
  }
  else if(strncmp(input, "BLACK", 5) == 0)
  {
    input_color = 'B';
    input += 5;
  }
  else
  {
    return INVALID_COMMAND;
  }
  input = ignoreBlankspaces(input);
  if(input[0] == '1' && input[1] == '0')
  {
    input_value = 10;
    input += 2;
  }
  else if((input_value = getValueAsInt(input[0])) > 0)
  {
    input += 1;
  }
  else
  {
    return INVALID_COMMAND;
  }
  input = ignoreBlankspaces(input);
  if(strncmp(input, "TO", 2) == 0)
  {
    input += 2;
  }
  else
  {
    return INVALID_COMMAND;
  }
  input = ignoreBlankspaces(input);
  if(input[0] > '0' && input[0] < '7')
  {
    input_stack = input[0] - '0';
    input++;
  }
  else
  {
    return INVALID_COMMAND;
  }
  input = ignoreBlankspaces(input);
  if(input[0] != '\0')
  {
    return INVALID_COMMAND;
  }
  Stack *pile_to_move =
    findCardPileByColorValue(input_color, input_value, stack_array);
  if(pile_to_move == NULL)
  {
    return OUT_OF_MEMORY;
  }
  else if (pile_to_move->bottom_card_ == NULL)
  {
    free(pile_to_move);
    return INVALID_MOVE;
  }
  else
  {
    ReturnState return_value =
      checkValidMove(pile_to_move, stack_array + input_stack);
    if(return_value == OK)
    {
      movePile(pile_to_move, stack_array + input_stack);
    }
    return return_value;
  }
}

//------------------------------------------------------------------------------
///
/// Entry function of the program for ass3
///
/// @param argc number of arguments
/// @param argv string array of program arguments
///
/// @return value of ReturnState which defines type of error
//

int main(int argc, char *argv[])
{
  Stack stack_array[7] = {{NULL, "DRAW", NULL},
                          {NULL, "GAME", NULL},
                          {NULL, "GAME", NULL},
                          {NULL, "GAME", NULL},
                          {NULL, "GAME", NULL},
                          {NULL, "DEPOSIT", NULL},
                          {NULL, "DEPOSIT", NULL}};
  if(argc != 2)
  {
    printErrorMessage(INVALID_ARGS);
    return INVALID_ARGS;
  }
  ReturnState return_value = readInitFile(argv[1], stack_array + DRAW_STACK);

  if(return_value != OK)
  {
    return printErrorMessage(return_value);
  }

  initialHandOut(stack_array);
  printMatchfield(stack_array);
  while(1)
  {

    printf("esp> ");

    char *input_memory_location = NULL;

    unsigned int current_input_size = 0;
    char current_input_char;
    while((current_input_char = getchar()) != '\n'
      && current_input_char != '\r' && current_input_char != EOF)
    {
      input_memory_location =
        realloc(input_memory_location, current_input_size + 1);
      if(input_memory_location == NULL)
      {
        freeAllCardMemories(stack_array);
        return OUT_OF_MEMORY;
      }
      input_memory_location[current_input_size] = toupper(current_input_char);
      current_input_size++;
    }
    input_memory_location =
      realloc(input_memory_location, current_input_size + 1);
    if(input_memory_location == NULL)
    {
      freeAllCardMemories(stack_array);
      return OUT_OF_MEMORY;
    }
    input_memory_location[current_input_size] = '\0';
    if(current_input_char == EOF)
    {
      freeAllCardMemories(stack_array);
      free(input_memory_location);
      return 0;
    }
    if(current_input_size == 0)
    {
      free(input_memory_location);
      continue;
    }
    char *input = input_memory_location;

    input = ignoreBlankspaces(input);
    if(strncmp(input, "HELP", 4) == 0)
    {
      free(input_memory_location);
      printf("possible command:\n"
             " - move <color> <value> to <stacknumber>\n"
             " - help\n"
             " - exit\n");
    }
    else if(strncmp(input, "MOVE", 4) == 0)
    {
      input += 4;
      ReturnState move_return_state = doMove(input, stack_array);
      free(input_memory_location);
      if(move_return_state == OK)
      {
        printMatchfield(stack_array);
      }
      else if(move_return_state < 0)
      {
        printErrorMessage(move_return_state);
      }
      else
      {
        freeAllCardMemories(stack_array);
        return move_return_state;
      }
    }
    else if(strncmp(input, "EXIT", 4) == 0 || input[0] == EOF)
    {
      free(input_memory_location);
      freeAllCardMemories(stack_array);
      return 0;
    }
    else
    {
      printErrorMessage(INVALID_COMMAND);
      free(input_memory_location);
    }
  }
}