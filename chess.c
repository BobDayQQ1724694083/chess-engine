/* Copyright (c) 2017 Rajat Pundir */
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

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
#define MOVES_MODE	1
#define DANGERS_MODE	1
#define DANGERS_HELP	1
#define SHOW_HARM	1

// macros that toogles wisdom
#define WISE_KING	1
#define WISE_QUEEN	0
#define WISE_ROOK	0
#define WISE_KNIGHT	0
#define WISE_BISHOP	0
#define WISE_PAWN	0

int board[BOARD_SIZE][BOARD_SIZE]; /* Board representation, white as positive, black as negative, blanks with zeroes */

typedef struct node {
  /* Please note the structure is used differently for moves and dangers list */
  int piece;
  int from_row;
  int from_column;
  int to_row;
  int to_column;
  struct node *next;
}move;

move *moves = NULL, *dangers = NULL;

/*
  Note.
  1. A function to remove unsafe moves from 'moves' list
  
  Some documentation goes here.

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

char* pretty_print_full_name(int piece) {
  switch(piece) {
  case INVALID:		return "Invalid";
  case BLANK:		return "Blank";
  case PAWN:		return "White Pawn";
  case BISHOP:		return "White Bishop";
  case KNIGHT:		return "White Knight";
  case ROOK:		return "White Rook";
  case QUEEN:		return "White Queen";
  case KING:		return "White King";
  case -PAWN:		return "Black Pawn";
  case -BISHOP:		return "Black Bishop";
  case -KNIGHT:		return "Black Knight";
  case -ROOK:		return "Black Rook";
  case -QUEEN:       	return "Black Queen";
  case -KING:		return "Black King";
  }
}

int is_in_moves_list(int rownum, int colnum) {
  move *temp = moves;
  while(temp != NULL) {
    if(temp -> to_row == rownum && temp -> to_column == colnum)
      return 1;
    temp = temp -> next;
  }
  return 0;
}

int is_in_dangers_list(int rownum, int colnum) {
  move *temp = dangers;
  while(temp != NULL) {
    if(temp -> from_row == rownum && temp -> from_column == colnum)
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
    if(SHOW_KILL && moves != NULL) {
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
    if(SHOW_HARM && dangers != NULL) {
      printf("\n  ");
      for(int i = 2; i < 14; i++)
	printf("--");
      printf("\n");
      for(int i = 2; i < (BOARD_SIZE - 2); i++) {
	printf("%d |", (BOARD_SIZE - 2) - i);
	for(int j = 2; j < (BOARD_SIZE - 2); j++) {
	  if(is_in_dangers_list(i, j))
	    printf("=| ");
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
  char str[75];
  fgets(str, 75, stdin);
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

int sign_of_piece(int piece) {
  if(piece > 0)
    return 1;
  return -1;
}

void clear_moves() {
  move *temp = NULL;
  while(moves != NULL) {
    temp = moves;
    moves = moves -> next;
    free(temp);
  }
}

void clear_dangers() {
  move *temp = NULL;
  while(dangers != NULL) {
    temp = dangers;
    dangers = dangers -> next;
    free(temp);
  }
}

void translate_move(int piece, int from_row, int from_column, int to_row, int to_column) {
  if(MOVES_MODE)		/* macro toggles way of displaying moves */
    printf("%c%c%d-%c%c%d ", pretty_print(piece), from_column + 95, (BOARD_SIZE - 2) - from_row, pretty_print(piece), to_column + 95, (BOARD_SIZE - 2) - to_row);
  else
    printf("r%d-c%d ", to_row, to_column);
}

void translate_danger(int piece, int from_row, int from_column, int to_row, int to_column) {
  if(DANGERS_MODE)		/* macro toggles way of displaying dangers */
    printf("%c%c%d-%c%c%d ", pretty_print(piece), from_column + 95, (BOARD_SIZE - 2) - from_row, pretty_print(piece), to_column + 95, (BOARD_SIZE - 2) - to_row);
  else
    printf("r%d-c%d -> r%dc%d ", from_row, from_column, to_row, to_column);
}

void show_moves() {
  move *temp = moves;
  if(temp != NULL)
    printf("\nmoves: ");
  while(temp != NULL) {
    translate_move(temp -> piece, temp -> from_row, temp -> from_column, temp -> to_row, temp -> to_column);
    temp = temp -> next;
  }
}

void show_dangers() {
  move *temp = dangers;
  if(temp != NULL)
    printf("\ndangers: ");
  while(temp != NULL) {
    translate_danger(temp -> piece, temp -> from_row, temp -> from_column, temp -> to_row, temp -> to_column);
    temp = temp -> next;
  }
  if(DANGERS_HELP) {
    int count = 1;
    move *temp = dangers;
    if(temp != NULL)
      while(temp != NULL) {
	printf("\n%d. %s at %c%d is attacking %c%d", count, pretty_print_full_name(temp -> piece), (temp -> from_column) + 95, (BOARD_SIZE - 2) - (temp -> from_row), (temp -> to_column) + 95, (BOARD_SIZE - 2) - (temp -> to_row));
	temp = temp -> next;
	count++;
      }
    printf("\n");
  }
}

void push_move(int piece, int from_rownum, int from_colnum, int to_rownum, int to_colnum) {
  if (board[to_rownum][to_colnum] == INVALID)
    return; // discard invalid moves
  if (from_rownum < 2 || from_rownum > (BOARD_SIZE - 3) || from_colnum < 2 || from_colnum > (BOARD_SIZE - 3))
    return; // discard invalid moves
  if (to_rownum < 2 || to_rownum > (BOARD_SIZE - 3) || to_colnum < 2 || to_colnum > (BOARD_SIZE - 3))
    return; // discard invalid moves
  move *temp = (move *)malloc(sizeof(move));
  temp -> piece = piece;
  temp -> from_row = from_rownum;
  temp -> from_column = from_colnum;
  temp -> to_row = to_rownum;
  temp -> to_column = to_colnum;
  temp -> next = moves;
  moves = temp;
}

void push_danger(int piece, int from_rownum, int from_colnum, int to_rownum, int to_colnum) {
  if (board[to_rownum][to_colnum] == INVALID) // discard invalid dangers
    return;
  move *temp = (move *)malloc(sizeof(move));
  temp -> piece = piece;
  temp -> from_row = from_rownum;
  temp -> from_column = from_colnum;
  temp -> to_row = to_rownum;
  temp -> to_column = to_colnum;
  temp -> next = dangers;
  dangers = temp;
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
  int piece = board[row][column], from_row = row, from_column = column;
  if(board[row][column] > 0) {	// if piece is white
    while(board[row][column] != INVALID) {
      change_row_column(&row, &column, &direction); // move to next position
      if(board[row][column] > 0)	// break if piece was of same color
	break;
      push_move(piece, from_row, from_column, row, column);
      if(board[row][column] < 0)	// break if piece was of opposite color
	break;
    }
  }
  else if(board[row][column] < 0) {     // else piece is black
    while(board[row][column] != INVALID) {
      change_row_column(&row, &column, &direction); // move to next position
      if(board[row][column] < 0)	// break if piece was of same color
	break;
      push_move(piece, from_row, from_column, row, column);
      if(board[row][column] > 0)	// break if piece was of opposite color
	break;
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

void move_in_direction_of_danger(int piece, int row, int column, int direction) {
  // This function is designed to detect dangers from enemy king, bishops, rooks and queens.
  int to_row = row, to_column = column;
  if(piece > 0) {	// if piece is white
    while(board[row][column] != INVALID) {
      change_row_column(&row, &column, &direction); // move to next position
      if(board[row][column] > 0)	// break if piece at (row, column) is blocking dangers
	break;
      if(board[row][column] < 0) {	// break after pushing danger
	switch(direction) {
	case NORTH:
	case SOUTH:
	case EAST:
	case WEST:
	  switch(board[row][column]) {
	    // dangers by enemy king, queens, and rooks
	  case -KING:
	    switch(direction) {
	    case NORTH:
	    case SOUTH:
	      if(abs(row - to_row) == 1)
		push_danger(board[row][column], row, column, to_row, to_column);
	      break;
	    case EAST:
	    case WEST:
	      if(abs(column - to_column) == 1)
		push_danger(board[row][column], row, column, to_row, to_column);
	      break;
	    }
	    break;
	  case -QUEEN:
	  case -ROOK:
	    push_danger(board[row][column], row, column, to_row, to_column);
	    break;
	  }
	  break;
	case SOUTH_EAST:
	case SOUTH_WEST:
	case NORTH_EAST:
	case NORTH_WEST:
	  switch(board[row][column]) {
	    // dangers by enemy king, queens and bishops
	  case -KING:
	  case -PAWN:
	    switch(direction) {
	    case SOUTH_EAST:
	    case SOUTH_WEST:
	      if(board[row][column] == -KING)
	        if(abs(row - to_row) == 1 && abs(column - to_column) == 1)
		  push_danger(board[row][column], row, column, to_row, to_column);	  
	      break;
	      // North-East and North-West options are valid only for White pieces.
	      // Options for Black pieces will be South-East and South-West.
	    case NORTH_EAST:
	    case NORTH_WEST:
	      if(abs(row - to_row) == 1 && abs(column - to_column) == 1)
		push_danger(board[row][column], row, column, to_row, to_column);	  
	      break;
	    }
	    break;
	  case -QUEEN:
	  case -BISHOP:
	    push_danger(board[row][column], row, column, to_row, to_column);
	    break;
	  }
	  break;
	}
	break;			/* break after pushing danger */
      }
    }
  }
  else if(piece < 0) {     // else piece is black
    while(board[row][column] != INVALID) {
      change_row_column(&row, &column, &direction); // move to next position
      if(board[row][column] < 0)	// break if piece at (row, column) is blocking dangers
	break;
      if(board[row][column] > 0) {	// break after pushing danger
	switch(direction) {
	case NORTH:
	case SOUTH:
	case EAST:
	case WEST:
	  switch(board[row][column]) {
	    // dangers by enemy king, queens, and rooks
	  case KING:
	    switch(direction) {
	    case NORTH:
	    case SOUTH:
	      if(abs(row - to_row) == 1)
		push_danger(board[row][column], row, column, to_row, to_column);
	      break;
	    case EAST:
	    case WEST:
	      if(abs(column - to_column) == 1)
		push_danger(board[row][column], row, column, to_row, to_column);
	      break;
	    }
	    break;
	  case QUEEN:
	  case ROOK:
	    push_danger(board[row][column], row, column, to_row, to_column);
	    break;
	  }
	  break;
	case SOUTH_EAST:
	case SOUTH_WEST:
	case NORTH_EAST:
	case NORTH_WEST:
	  switch(board[row][column]) {
	    // dangers by enemy king, queens and bishops
	  case KING:
	  case PAWN:
	    switch(direction) {
	    case NORTH_EAST:
	    case NORTH_WEST:
	      if(board[row][column] == KING)
	        if(abs(row - to_row) == 1 && abs(column - to_column) == 1)
		  push_danger(board[row][column], row, column, to_row, to_column);	  
	      break;
	      // South-East and South-West options are valid only for Black pieces.
	      // Options for White pieces will be North-East and North-West.
	    case SOUTH_EAST:
	    case SOUTH_WEST:
	      if(abs(row - to_row) == 1 && abs(column - to_column) == 1)
		push_danger(board[row][column], row, column, to_row, to_column);	  
	      break;
	    }
	    break;
	  case QUEEN:
	  case BISHOP:
	    push_danger(board[row][column], row, column, to_row, to_column);
	    break;
	  }
	  break;
	}
	break;			/* break after pushing danger */
      }
    }
  }
}

void danger_by_row_column(int piece, int row, int column) {
  // initialises linked list pointer 'dangers' with list of dangers that await for 'piece' which is at (row, column)
  clear_dangers();
  // dangers from enemy king, rooks, bishops, queens, and pawns.
  move_in_direction_of_danger(piece, row, column, NORTH);
  move_in_direction_of_danger(piece, row, column, EAST);
  move_in_direction_of_danger(piece, row, column, SOUTH);
  move_in_direction_of_danger(piece, row, column, WEST);
  move_in_direction_of_danger(piece, row, column, NORTH_EAST);
  move_in_direction_of_danger(piece, row, column, SOUTH_EAST);
  move_in_direction_of_danger(piece, row, column, NORTH_WEST);
  move_in_direction_of_danger(piece, row, column, SOUTH_WEST);
  // dangers from enemy knight
  if(piece > 0) {	      // if piece is white
    if(board[row - 1][column - 2] == -KNIGHT)
      push_danger(-KNIGHT, row - 1, column - 2, row, column);
    if(board[row - 1][column + 2] == -KNIGHT)
      push_danger(-KNIGHT, row - 1, column + 2, row, column);
    if(board[row - 2][column - 1] == -KNIGHT)
      push_danger(-KNIGHT, row - 2, column - 1, row, column);
    if(board[row - 2][column + 1] == -KNIGHT)
      push_danger(-KNIGHT, row - 2, column + 1, row, column);
    if(board[row + 1][column - 2] == -KNIGHT)
      push_danger(-KNIGHT, row + 1, column - 2, row, column);
    if(board[row + 1][column + 2] == -KNIGHT)
      push_danger(-KNIGHT, row + 1, column + 2, row, column);
    if(board[row + 2][column - 1] == -KNIGHT)
      push_danger(-KNIGHT, row + 2, column - 1, row, column);
    if(board[row + 2][column + 1] == -KNIGHT)
      push_danger(-KNIGHT, row + 2, column + 1, row, column);
  }
  else if(piece < 0) {	      // else piece is black
    if(board[row - 1][column - 2] == KNIGHT)
      push_danger(KNIGHT, row - 1, column - 2, row, column);
    if(board[row - 1][column + 2] == KNIGHT)
      push_danger(KNIGHT, row - 1, column + 2, row, column);
    if(board[row - 2][column - 1] == KNIGHT)
      push_danger(KNIGHT, row - 2, column - 1, row, column);
    if(board[row - 2][column + 1] == KNIGHT)
      push_danger(KNIGHT, row - 2, column + 1, row, column);
    if(board[row + 1][column - 2] == KNIGHT)
      push_danger(KNIGHT, row + 1, column - 2, row, column);
    if(board[row + 1][column + 2] == KNIGHT)
      push_danger(KNIGHT, row + 1, column + 2, row, column);
    if(board[row + 2][column - 1] == KNIGHT)
      push_danger(KNIGHT, row + 2, column - 1, row, column);
    if(board[row + 2][column + 1] == KNIGHT)
      push_danger(KNIGHT, row + 2, column + 1, row, column);
  }
}

void remove_unsafe_moves() {
  /* this function removes moves from the 'moves' list that are unsafe for the pieces in that list */
  move *temp = moves, *safe_moves = NULL;
  while(temp != NULL) {
    board[(temp -> from_row)][(temp -> from_column)] = BLANK;
    danger_by_row_column(temp -> piece, temp-> to_row, temp -> to_column);
    if(dangers == NULL) {
      // move is safe from enemy pieces, push it to 'safe_moves' list
      move *safe_temp = (move *)malloc(sizeof(move));
      safe_temp -> piece       =	temp -> piece;
      safe_temp -> from_row    =	temp -> from_row;
      safe_temp -> from_column =	temp -> from_column;
      safe_temp -> to_row      =	temp -> to_row;
      safe_temp -> to_column   =	temp -> to_column;
      safe_temp -> next	       =	safe_moves;
      safe_moves	       =	safe_temp;
    }
    board[temp -> from_row][temp -> from_column] = temp -> piece;
    temp = temp -> next;
  }
  clear_moves();
  moves = safe_moves;
}

void moves_of_pawn(int row, int column) {
  // initialises linked list pointer 'moves' with list of moves that the pawn at (row,column) can make.
  clear_moves();
  if(board[row][column] > 0) {	// if piece is white
    if(board[row - 1][column] == BLANK) 
      push_move(PAWN, row, column, row - 1, column);
    if(row == 8 && board[row - 2][column] == BLANK) /* initial case for a pawn */
      push_move(PAWN, row, column, row - 2, column);
    if(board[row - 1][column - 1] < 0 && board[row - 1][column - 1] != BLANK)
      push_move(PAWN, row, column, row - 1, column - 1);
    if(board[row - 1][column + 1] < 0  && board[row - 1][column + 1] != BLANK)
      push_move(PAWN, row, column, row - 1, column + 1);
  }
  else if(board[row][column] < 0) {			// else piece is black
    if(board[row + 1][column] == BLANK)
      push_move(-PAWN, row, column, row + 1, column);
    if(row == 3 && board[row + 2][column] == BLANK) /* initial case for a pawn */
      push_move(-PAWN, row, column, row + 2, column);
    if(board[row + 1][column - 1] > 0 && board[row + 1][column - 1] != BLANK)
      push_move(-PAWN, row, column, row + 1, column - 1);
    if(board[row + 1][column + 1] > 0 && board[row + 1][column + 1] != BLANK)
      push_move(-PAWN, row, column, row + 1, column + 1);
  }
  if(WISE_PAWN)
    remove_unsafe_moves();
}

void moves_of_bishop(int row, int column) {
  // initialises linked list pointer 'moves' with list of moves that the bishop at (row,column) can make.
  clear_moves();
  move_diagonally(row, column);
  if(WISE_BISHOP)
    remove_unsafe_moves();
}

void moves_of_knight(int row, int column) {
  // initialises linked list pointer 'moves' with list of moves that the knight at (row,column) can make.
  clear_moves();
  int sign_of_knight = sign_of_piece(board[row][column]);
  if(board[row - 1][column - 2] == BLANK || sign_of_piece(board[row - 1][column - 2]) != sign_of_knight)
    push_move(sign_of_knight * KNIGHT, row, column, row - 1, column - 2);
  if(board[row - 1][column + 2] == BLANK || sign_of_piece(board[row - 1][column + 2]) != sign_of_knight)
    push_move(sign_of_knight * KNIGHT, row, column, row - 1, column + 2);
  if(board[row - 2][column - 1] == BLANK || sign_of_piece(board[row - 2][column - 1]) != sign_of_knight)
    push_move(sign_of_knight * KNIGHT, row, column, row - 2, column - 1);
  if(board[row - 2][column + 1] == BLANK || sign_of_piece(board[row - 2][column + 1]) != sign_of_knight)
    push_move(sign_of_knight * KNIGHT, row, column, row - 2, column + 1);
  if(board[row + 1][column - 2] == BLANK || sign_of_piece(board[row + 1][column - 2]) != sign_of_knight)
    push_move(sign_of_knight * KNIGHT, row, column, row + 1, column - 2);
  if(board[row + 1][column + 2] == BLANK || sign_of_piece(board[row + 1][column + 2]) != sign_of_knight)
    push_move(sign_of_knight * KNIGHT, row, column, row + 1, column + 2);
  if(board[row + 2][column - 1] == BLANK || sign_of_piece(board[row + 2][column - 1]) != sign_of_knight)
    push_move(sign_of_knight * KNIGHT, row, column, row + 2, column - 1);
  if(board[row + 2][column + 1] == BLANK || sign_of_piece(board[row + 2][column + 1]) != sign_of_knight)
    push_move(sign_of_knight * KNIGHT, row, column, row + 2, column + 1);
  if(WISE_KNIGHT)
    remove_unsafe_moves();
}

void moves_of_rook(int row, int column) {
  // initialises linked list pointer 'moves' with list of moves that the rook at (row,column) can make.
  clear_moves();
  move_straight(row, column);
  if(WISE_ROOK)
    remove_unsafe_moves();
}

void moves_of_queen(int row, int column) {
  // initialises linked list pointer 'moves' with list of moves that the queen at (row,column) can make.
  clear_moves();
  move_diagonally(row, column);
  move_straight(row, column);  
  if(WISE_QUEEN)
    remove_unsafe_moves();
}

void moves_of_king(int row, int column) {
  // initialises linked list pointer 'moves' with list of moves that the king at (row,column) can make.
  clear_moves();
  int sign_of_king = sign_of_piece(board[row][column]);
  if(board[row - 1][column] == BLANK || sign_of_piece(board[row - 1][column]) != sign_of_king)
    push_move(sign_of_king * KING, row, column, row - 1, column);
  if(board[row - 1][column - 1] == sign_of_piece(BLANK || board[row - 1][column - 1]) != sign_of_king)
    push_move(sign_of_king * KING, row, column, row - 1, column - 1);
  if(board[row - 1][column + 1] == sign_of_piece(BLANK || board[row - 1][column + 1]) != sign_of_king)
    push_move(sign_of_king * KING, row, column, row - 1, column + 1);
  if(board[row][column - 1] == BLANK || sign_of_piece(board[row][column - 1]) != sign_of_king)
    push_move(sign_of_king * KING, row, column, row, column - 1);
  if(board[row][column + 1] == BLANK || sign_of_piece(board[row][column + 1]) != sign_of_king)
    push_move(sign_of_king * KING, row, column, row, column + 1);
  if(board[row + 1][column] == BLANK || sign_of_piece(board[row + 1][column]) != sign_of_king)
    push_move(sign_of_king * KING, row, column, row + 1, column);
  if(board[row + 1][column - 1] == sign_of_piece(BLANK || board[row + 1][column - 1]) != sign_of_king)
    push_move(sign_of_king * KING, row, column, row + 1, column - 1);
  if(board[row + 1][column + 1] == sign_of_piece(BLANK || board[row + 1][column + 1]) != sign_of_king)
    push_move(sign_of_king * KING, row, column, row + 1, column + 1);
  if(WISE_KING)			/* this must be on for normal gameplay */
    remove_unsafe_moves();
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

void set_row_column_by_notation(char *str, int *row, int *column) {
  switch(str[0]) {
  case 'a':
  case 'A':
    *column = 2;
    break;
  case 'b':
  case 'B':
    *column = 3;
    break;
  case 'c':
  case 'C':
    *column = 4;
    break;
  case 'd':
  case 'D':
    *column = 5;
    break;
  case 'e':
  case 'E':
    *column = 6;
    break;
  case 'f':
  case 'F':
    *column = 7;
    break;
  case 'g':
  case 'G':
    *column = 8;
    break;
  case 'h':
  case 'H':
    *column = 9;
    break;
  }
  if(str[1] > 48 && str[1] < 57) {
    *row = str[1] - '0';
    *row = (BOARD_SIZE - 2) - *row;
  }
}

void move_by_notation(char *str) {
  int row = 0, column = 0;
  set_row_column_by_notation(str, &row, &column);
  if(row != 0 && column != 0)
    move_by_row_column(row, column);
}

void danger_by_notation(int piece, char *str) {
  int row = 0, column = 0;
  set_row_column_by_notation(str, &row, &column);
  if(row != 0 && column != 0)
    danger_by_row_column(piece, row, column);
}

void danger_on_oneself(int row, int column) {
  danger_by_row_column(board[row][column], row, column);
}

void danger_on_oneself_by_notation(char *str) {
  int row = 0, column = 0;
  set_row_column_by_notation(str, &row, &column);
  if(row != 0 && column != 0)
    danger_on_oneself(row, column);
}

int main() {
  freopen("input.txt", "r", stdin);
  initialise_board();
  read_fen();

  char str[] = "e5";
  move_by_notation(str);
  show_moves();

  danger_on_oneself_by_notation(str);
  show_dangers();
  
  show_board();
  return 1;
}

