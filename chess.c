#include <stdio.h>
#include <string.h>
#include <malloc.h>

// macros for pieces
#define KING	10
#define QUEEN	9
#define ROOK	5
#define KNIGHT	4
#define BISHOP	3
#define PAWN	1

// macros for directions
#define NORTH		1
#define NORTH_EAST	2
#define EAST		3
#define SOUTH_EAST	4
#define SOUTH		5
#define SOUTH_WEST	6
#define WEST		7
#define NORTH_WEST	8

// macros for board configuration
#define INVALID		42
#define BOARD_SIZE	12
#define BLANK		0

// macros that can be toggled
#define SHOW_VALUES	0
#define SHOW_FEN	1
#define SHOW_KILL	1
#define SHOW_MOVES	1

int board[BOARD_SIZE][BOARD_SIZE]; /* Board representation, white as positive, black as negative, blanks with zeroes */

typedef struct node {
  int row;
  int column;
  struct node *next;
}move;

move *moves = NULL;

/* 
   Just watch the sign of a piece, move in each direction you can until non-zeroes are obstructions, opposite signs are possible captures.
   If you move a piece, place zero at that position, and place it where you can, make a list of moves if you need that for later use.
   TODO. Make functions for all eight directions, each function should return last non-zero position, meaning border or piece position.Each function should take a starting position, and number of moves allowed before exhaustion. Knight can be given a special L function, which takes a direction and return a pointer to list of moves it can make. But, then eight direction functions will be helper functions to major peice functions.

   Note.
   KING.   Allowed to move one step in eight directions.
   QUEEN.  Allowed to move infinty steps in eight directions.
   ROOK.   Allowed to move infinty steps in four straight directions.
   BISHOP. Allowed to move infinty steps in four diagonal directions.
   KNIGHT. Allowed to move one step in eight strange directions.
   PAWN.   Allowed to move one step forward in three directions.
   
*/

void initialise_board() {
  for(int i = 0; i < BOARD_SIZE; i++) {
    for(int j = 0; j < BOARD_SIZE; j++) {
      if(i == 0 || i == 1 || j ==0 || j == 1 || i == (BOARD_SIZE - 2) || j == (BOARD_SIZE - 2) || i == (BOARD_SIZE - 1) || j == (BOARD_SIZE - 1))
	board[i][j] = INVALID;
      else
	board[i][j] = BLANK;
    }
  }
}

char pretty_print(int piece) {
  switch(piece) {
  case INVALID:		return 'X';
  case BLANK:		return '`';
  case PAWN:		return 'P';
  case BISHOP:		return 'B';
  case KNIGHT:		return 'N';
  case ROOK:		return 'R';
  case QUEEN:		return 'Q';
  case KING:		return 'K';
  case -PAWN:		return 'p';
  case -BISHOP:		return 'b';
  case -KNIGHT:		return 'n';
  case -ROOK:		return 'r';
  case -QUEEN:       	return 'q';
  case -KING:		return 'k';
  }
}

int is_in_moves_list(int rownum, int colnum) {
  move *temp = moves;
  while(temp != NULL) {
    if(temp -> row == rownum && temp -> column == colnum)
      return 1;
    temp = temp -> next;
  }
  return 0;
}

void show_board() {
  if (SHOW_VALUES) {
    printf("\n");
    for(int i = 0; i < BOARD_SIZE; i++) {
      for(int j = 0; j < BOARD_SIZE; j++) {
	if(SHOW_KILL) {
	  if(is_in_moves_list(i, j)) {
	    switch(board[i][j]) {
	    case BLANK: printf("+\t");
	      break;
	    default: printf("#\t");
	    }
	  }
	  else
	    printf("%d\t",board[i][j]);
	}
      }
      printf("\n");
    }
  }
  else {
    printf("\n   ");
    for(int i = 2; i < (BOARD_SIZE - 2); i++)
      printf("%c  ", (i+95));
    printf("\n  ");
    for(int i = 2; i < 14; i++)
      printf("--");
    printf("\n");
    for(int i = 2; i < (BOARD_SIZE - 2); i++) {
      printf("%d |", (BOARD_SIZE - 2) - i);
      for(int j = 2; j < (BOARD_SIZE - 2); j++)
	printf("%c| ",pretty_print(board[i][j]));
      printf("%d\n", (BOARD_SIZE - 2) - i);
    }
    printf("  ");
    for(int i = 2; i < (BOARD_SIZE - 2); i++)
      printf("---");
    printf("\n   ");
    for(int i = 2; i < (BOARD_SIZE - 2); i++)
      printf("%c  ", (i+95));
    
    if(SHOW_KILL) {
      printf("\n  ");
      for(int i = 2; i < 14; i++)
	printf("--");
      printf("\n");
      for(int i = 2; i < (BOARD_SIZE - 2); i++) {
	printf("%d |", (BOARD_SIZE - 2) - i);
	for(int j = 2; j < (BOARD_SIZE - 2); j++) {
	  if(is_in_moves_list(i, j)) {
	    switch(board[i][j]) {
	    case BLANK: printf("+| ");
	      break;
	    default: printf("#| ");
	    }
	  }
	  else
	    printf("%c| ",pretty_print(board[i][j]));
	}
	printf("%d\n", (BOARD_SIZE - 2) - i);
      }
      printf("  ");
      for(int i = 2; i < (BOARD_SIZE - 2); i++)
	printf("---");
      printf("\n   ");
      for(int i = 2; i < (BOARD_SIZE - 2); i++)
	printf("%c  ", (i+95));
    }
  }
}

void read_fen() {
  char str[64];
  fgets(str, 64, stdin);
  if(SHOW_FEN) {
    fprintf(stdout, "FEN: %s", str);
  }
  for (int i = 0, row = 2, column = 2; i < strlen(str); i++ ) {
    char ch = str[i];
    if(ch == '/') {
      row++;
      column = 2;
    }
    else if(ch >= 65 && ch <= 90) {
      switch(ch) {
      case 'P': board[row][column++] = (PAWN);
	break;
      case 'B': board[row][column++] = (BISHOP);
	break;
      case 'N': board[row][column++] = (KNIGHT);
	break;
      case 'R': board[row][column++] = (ROOK);
	break;
      case 'Q': board[row][column++] = (QUEEN);
	break;
      case 'K': board[row][column++] = (KING);
	break;
      }
    }
    else if(ch >= 97 && ch <= 122) {
      switch(ch) {
      case 'p': board[row][column++] = -(PAWN);
	break;
      case 'b': board[row][column++] = -(BISHOP);
	break;
      case 'n': board[row][column++] = -(KNIGHT);
	break;
      case 'r': board[row][column++] = -(ROOK);
	break;
      case 'q': board[row][column++] = -(QUEEN);
	break;
      case 'k': board[row][column++] = -(KING);
	break;
      }
    }
    else if(ch >=48 && ch <= 58) {
      int n = ch - 48;
      while(n != 0) {
	board[row][column++] = BLANK;
	n--;
      }
    }
  }
}

void clear_moves() {
  move *temp = NULL;
  while(moves != NULL) {
    temp = moves;
    moves = moves -> next;
    free(temp);
  }
}

void translate_move(int row, int column) {
  if(SHOW_MOVES) {
    switch(column) {
    case 2: printf("a");
      break;
    case 3: printf("b");
      break;
    case 4: printf("c");
      break;
    case 5: printf("d");
      break;
    case 6: printf("e");
      break;
    case 7: printf("f");
      break;
    case 8: printf("g");
      break;
    case 9: printf("h");
      break;
    }
    printf("%d ", (BOARD_SIZE - 2) - row);
  }
  else
    printf("%d-%d ", row - 1, column - 1);
}

void show_moves() {
   move *temp = moves;
  if(temp != NULL)
    printf("\nmoves: ");
  while(temp != NULL) {
    translate_move(temp-> row, temp -> column);
    temp = temp -> next;
  }
}

void push_move(int rownum, int colnum) {
  if (board[rownum][colnum] == INVALID) // discard invalid moves
    return;
  move *temp = (move *)malloc(sizeof(move));
  temp -> row = rownum;
  temp -> column = colnum;
  temp -> next = moves;
  moves = temp;
}

void change_row_column(int *row, int *column, int *direction) {
  switch(*direction) {
  case NORTH: (*row)--;
    break;
  case SOUTH: (*row)++;
    break;
  case EAST: (*column)++;
    break;
  case WEST: (*column)--;
    break;
  case NORTH_EAST: (*row)--; (*column)++;
    break;
  case NORTH_WEST: (*row)--; (*column)--;
    break;
  case SOUTH_EAST: (*row)++; (*column)++;
    break;
  case SOUTH_WEST: (*row)++; (*column)--;
    break;
  }
}

void move_in_direction(int row, int column, int direction) {
  // This function is designed to be used for bishops, rooks and queens.
  if(board[row][column] > 0) {	// if piece is white
    while(board[row][column] != INVALID) {
      if(board[row][column] > 0)	// break if piece was of same color
	break;
      push_move(row, column);
      if(board[row][column] < 0)	// break if piece was of opposite color
	break;
      change_row_column(&row, &column, &direction); // move to next position
    }
  }
  else if(board[row][column] < 0) {     // else piece is black
    while(board[row][column] != INVALID) {
      if(board[row][column] < 0)	// break if piece was of same color
	break;
      push_move(row, column);
      if(board[row][column] > 0)	// break if piece was of opposite color
	break;
      change_row_column(&row, &column, &direction); // move to next position
    }
  }
}

void move_diagonally(int row, int column) {
  // This function is designed to be used for bishops and queens.
  move_in_direction(row, column, NORTH_EAST);
  move_in_direction(row, column, SOUTH_EAST);
  move_in_direction(row, column, SOUTH_WEST);
  move_in_direction(row, column, NORTH_WEST);
}

void move_straight(int row, int column) {
  // This function is designed to be used for rooks and queens.
  move_in_direction(row, column, NORTH);
  move_in_direction(row, column, EAST);
  move_in_direction(row, column, SOUTH);
  move_in_direction(row, column, WEST);
}

void moves_of_pawn(int row, int column) {
  // initialises linked list pointer 'moves' with list of moves that the pawn at (row,column) can make.
    clear_moves();
  if(board[row][column] > 0) {	// if piece is white
    if(board[row - 1][column] == BLANK) 
      push_move(row - 1, column);
    if(row == 8 && board[row - 2][column] == BLANK) /* initial case for a pawn */
      push_move(row - 2, column);
    if(board[row - 1][column - 1] < 0 && board[row - 1][column - 1] != BLANK)
      push_move(row - 1, column - 1);
    if(board[row - 1][column + 1] < 0  && board[row - 1][column + 1] != BLANK)
      push_move(row - 1, column + 1);
  }
  else if(board[row][column] < 0) {			// else piece is black
    if(board[row + 1][column] == BLANK)
      push_move(row + 1, column);
    if(row == 3 && board[row + 2][column] == BLANK) /* initial case for a pawn */
      push_move(row + 2, column);
    if(board[row + 1][column - 1] > 0 && board[row + 1][column - 1] != BLANK)
      push_move(row + 1, column - 1);
    if(board[row + 1][column + 1] > 0 && board[row + 1][column + 1] != BLANK)
      push_move(row + 1, column + 1);
  }
}

void moves_of_bishop(int row, int column) {
  // initialises linked list pointer 'moves' with list of moves that the bishop at (row,column) can make.
  clear_moves();
  move_diagonally(row, column);
}

void moves_of_knight(int row, int column) {
  // initialises linked list pointer 'moves' with list of moves that the knight at (row,column) can make.
  clear_moves();
  if(board[row][column] > 0) {	// if piece is white
    if(board[row - 1][column - 2] == BLANK || board[row - 1][column - 2] < 0)
      push_move(row - 1, column - 2);
    if(board[row - 1][column + 2] == BLANK || board[row - 1][column + 2] < 0)
      push_move(row - 1, column + 2);
    if(board[row - 2][column - 1] == BLANK || board[row - 2][column - 1] < 0)
      push_move(row - 2, column - 1);
    if(board[row - 2][column + 1] == BLANK || board[row - 2][column + 1] < 0)
      push_move(row - 2, column + 1);
    if(board[row + 1][column - 2] == BLANK || board[row + 1][column - 2] < 0)
      push_move(row + 1, column - 2);
    if(board[row + 1][column + 2] == BLANK || board[row + 1][column + 2] < 0)
      push_move(row + 1, column + 2);
    if(board[row + 2][column - 1] == BLANK || board[row + 2][column - 1] < 0)
      push_move(row + 2, column - 1);
    if(board[row + 2][column + 1] == BLANK || board[row + 2][column + 1] < 0)
      push_move(row + 2, column + 1);
  }
  else if(board[row][column] < 0) {			// else piece is black
    if(board[row - 1][column - 2] == BLANK || board[row - 1][column - 2] > 0)
      push_move(row - 1, column - 2);
    if(board[row - 1][column + 2] == BLANK || board[row - 1][column + 2] > 0)
      push_move(row - 1, column + 2);
    if(board[row - 2][column - 1] == BLANK || board[row - 2][column - 1] > 0)
      push_move(row - 2, column - 1);
    if(board[row - 2][column + 1] == BLANK || board[row - 2][column + 1] > 0)
      push_move(row - 2, column + 1);
    if(board[row + 1][column - 2] == BLANK || board[row + 1][column - 2] > 0)
      push_move(row + 1, column - 2);
    if(board[row + 1][column + 2] == BLANK || board[row + 1][column + 2] > 0)
      push_move(row + 1, column + 2);
    if(board[row + 2][column - 1] == BLANK || board[row + 2][column - 1] > 0)
      push_move(row + 2, column - 1);
    if(board[row + 2][column + 1] == BLANK || board[row + 2][column + 1] > 0)
      push_move(row + 2, column + 1);
  }
}

void moves_of_rook(int row, int column) {
  // initialises linked list pointer 'moves' with list of moves that the rook at (row,column) can make.
  clear_moves();
  move_straight(row, column);
}

void moves_of_queen(int row, int column) {
  // initialises linked list pointer 'moves' with list of moves that the queen at (row,column) can make.
  clear_moves();
  move_diagonally(row, column);
  move_straight(row, column);  
}

void moves_of_king(int row, int column) {
  // initialises linked list pointer 'moves' with list of moves that the king at (row,column) can make.
  // this function needs some work afterwards, like king can't take a piece which will put it in a check.
  clear_moves();
  if(board[row][column] > 0) {	// if piece is white
    if(board[row - 1][column] == BLANK || board[row - 1][column] < 0)
      push_move(row - 1, column);
    if(board[row - 1][column - 1] == BLANK || board[row - 1][column - 1] < 0)
      push_move(row - 1, column - 1);
    if(board[row - 1][column + 1] == BLANK || board[row - 1][column + 1] < 0)
      push_move(row - 1, column + 1);
    if(board[row][column - 1] == BLANK || board[row][column - 1] < 0)
      push_move(row, column - 1);
    if(board[row][column + 1] == BLANK || board[row][column + 1] < 0)
      push_move(row, column + 1);
    if(board[row + 1][column] == BLANK || board[row + 1][column] < 0)
      push_move(row + 1, column);
    if(board[row + 1][column - 1] == BLANK || board[row + 1][column - 1] < 0)
      push_move(row + 1, column - 1);
    if(board[row + 1][column + 1] == BLANK || board[row + 1][column + 1] < 0)
      push_move(row + 1, column + 1);
  }
  else if(board[row][column] < 0) {			// else piece is black
    if(board[row - 1][column] == BLANK || board[row - 1][column] > 0)
      push_move(row - 1, column);
    if(board[row - 1][column - 1] == BLANK || board[row - 1][column - 1] > 0)
      push_move(row - 1, column - 1);
    if(board[row - 1][column + 1] == BLANK || board[row - 1][column + 1] > 0)
      push_move(row - 1, column + 1);
    if(board[row][column - 1] == BLANK || board[row][column - 1] > 0)
      push_move(row, column - 1);
    if(board[row][column + 1] == BLANK || board[row][column + 1] > 0)
      push_move(row, column + 1);
    if(board[row + 1][column] == BLANK || board[row + 1][column] > 0)
      push_move(row + 1, column);
    if(board[row + 1][column - 1] == BLANK || board[row + 1][column - 1] > 0)
      push_move(row + 1, column - 1);
    if(board[row + 1][column + 1] == BLANK || board[row + 1][column + 1] > 0)
      push_move(row + 1, column + 1);
  }
}

void move_by_row_column (int row, int column) {
  switch(board[row][column]) {
  case PAWN:
  case -PAWN:
    moves_of_pawn(row, column);
    break;
  case BISHOP:
  case -BISHOP:
    moves_of_bishop(row, column);
    break;
  case KNIGHT:
  case -KNIGHT:
    moves_of_knight(row, column);
    break;
  case ROOK:
  case -ROOK:
    moves_of_rook(row, column);
    break;
  case QUEEN:
  case -QUEEN:
    moves_of_queen(row, column);
    break;
  case KING:
  case -KING:
    moves_of_king(row, column);
    break;
  }
}

void move_by_notation(char *str) {
  int row = 0, column = 0;
  switch(str[0]) {
  case 'a':
  case 'A':
    column = 2;
    break;
  case 'b':
  case 'B':
    column = 3;
    break;
  case 'c':
  case 'C':
    column = 4;
    break;
  case 'd':
  case 'D':
    column = 5;
    break;
  case 'e':
  case 'E':
    column = 6;
    break;
  case 'f':
  case 'F':
    column = 7;
    break;
  case 'g':
  case 'G':
    column = 8;
    break;
  case 'h':
  case 'H':
    column = 9;
    break;
  }
  if(str[1] > 48 && str[1] < 57) {
    row = str[1] - '0';
    row = (BOARD_SIZE - 2) - row;
  }
  if(row != 0 && column != 0)
    move_by_row_column(row, column);
}

int main() {
  freopen("input.txt", "r", stdin);
  initialise_board();
  read_fen();

  move_by_notation("f4");

  show_moves();
  show_board();
  return 1;
}

