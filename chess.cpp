#include <bits/stdc++.h>
using namespace std;

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

// macros for board configurations
#define BORDER		42
#define BLANK		0
#define SHOW_BORDER	1


int a[12][12];			/* Board representation, white as positive, black as negative, blanks with zeroes */

typedef struct move {
  int row;
  int column;
  struct move *next;
}move;

struct move *moves = NULL;

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
  for(int i = 0; i < 12; i++) {
    for(int j = 0; j < 12; j++) {
      if(i == 0 || i == 1 || j ==0 || j == 1 || i == 10 || j == 10 || i == 11 || j == 11)
	a[i][j] = BORDER;
      else
	a[i][j] = BLANK;
    }
  }
}

void show_board() {
  if (SHOW_BORDER) {
    for(int i = 0; i < 12; i++) {
      for(int j = 0; j < 12; j++) 
	printf("%d\t",a[i][j]);
      printf("\n");
    }
  }
  else {
    for(int i = 2; i < 10; i++) {
      for(int j = 2; j < 10; j++) 
	printf("%d\t",a[i][j]);
      printf("\n");
    }
  }
}

void read_fen() {
  string str;
  // scanf("%s\n",str);
  cin >> str;
  // cout << endl << str << endl;
  int counter = 1;
  for (int i = 0, row = 2, column = 2; i < str.length(); i++ ) {
    char ch = str[i];
    if(ch == '/') {
      row++;
      column = 2;
    }
    else if(ch >= 65 && ch <= 90) {
      switch(ch) {
      case 'P': a[row][column++] = (PAWN);
	break;
      case 'B': a[row][column++] = (BISHOP);
	break;
      case 'N': a[row][column++] = (KNIGHT);
	break;
      case 'R': a[row][column++] = (ROOK);
	break;
      case 'Q': a[row][column++] = (QUEEN);
	break;
      case 'K': a[row][column++] = (KING);
	break;
      }
    }
    else if(ch >= 97 && ch <= 122) {
      switch(ch) {
      case 'p': a[row][column++] = -(PAWN);
	break;
      case 'b': a[row][column++] = -(BISHOP);
	break;
      case 'n': a[row][column++] = -(KNIGHT);
	break;
      case 'r': a[row][column++] = -(ROOK);
	break;
      case 'q': a[row][column++] = -(QUEEN);
	break;
      case 'k': a[row][column++] = -(KING);
	break;
      }
    }
    else {
      int n = ch - 48;
      while(n != 0) {
	a[row][column++] = BLANK;
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

void show_moves() {
  move *temp = moves;
  while(temp != NULL) {
    printf("\nrow=%d, col=%d", temp -> row, temp -> column);
    temp = temp -> next;
  }
}

void push_move(int rownum, int colnum) {
  if (a[rownum][colnum] == BORDER) // discard invalid moves
    return;
  move *temp = (move *)malloc(size_of(move));
  temp -> row = rownum - 1;
  temp -> column = colnum;
  temp -> next = moves;
  moves = p;
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
  if(a[row][column] > 0) {	// if piece is white
    while(a[row][column] != BORDER) {
      if(a[row][column] > 0)	// break if piece was of same color
	break;
      push_move(row, column);
      if(a[row][column] < 0)	// break if piece was of opposite color
	break;
      change_row_column(&row, &column, &direction); // move to next position
    }
  }
  else {			// else piece is black
    while(a[row][column] != BORDER) {
      if(a[row][column] < 0)	// break if piece was of same color
	break;
      push_move(row, column);
      if(a[row][column] > 0)	// break if piece was of opposite color
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
  // make an improvement to make pawns run fast from their initial positions
  if(a[row][column] != PAWN)
    return;
  clear_moves();
  if(a[row][column] > 0) {	// if piece is white
    if(a[row - 1][column] == BLANK) 
      push_move(row - 1, column);
    if(a[row - 1][column - 1] < 0 && a[row - 1][column - 1] != BLANK)
      push_move(row - 1, column - 1);
    if(a[row - 1][column + 1] < 0  && a[row - 1][column + 1] != BLANK)
      push_move(row - 1, colummn + 1);
  }
  else {			// else piece is black
    if(a[row + 1][column] == BLANK)
      push_move(row + 1, colummn);
    if(a[row + 1][column - 1] > 0 && a[row + 1][column - 1] != BLANK)
      push_move(row + 1, colummn - 1);
    if(a[row + 1][column + 1] > 0 && a[row + 1][column + 1] != BLANK)
      push_move(row + 1, colummn + 1);
  }
}

void moves_of_bishop(int row, int column) {
  // initialises linked list pointer 'moves' with list of moves that the bishop at (row,column) can make.
  if(a[row][column] != BISHOP)
    return;
  clear_moves();
  move_diagonally(row, column);
}

void moves_of_knight(int row, int column) {
  // initialises linked list pointer 'moves' with list of moves that the knight at (row,column) can make.
  if(a[row][column] != KNIGHT)
    return;
  clear_moves();
  if(a[row][column] > 0) {	// if piece is white
    if(a[row - 1][column - 2] == BLANK || a[row - 1][column - 2] < 0)
      push_move(row - 1, column - 2);
    if(a[row - 1][column + 2] == BLANK || a[row - 1][column + 2] < 0)
      push_move(row - 1, column + 2);
    if(a[row - 2][column - 1] == BLANK || a[row - 2][column - 1] < 0)
      push_move(row - 2, column - 1);
    if(a[row - 2][column + 1] == BLANK || a[row - 2][column + 1] < 0)
      push_move(row - 2, column + 1);
    if(a[row + 1][column - 2] == BLANK || a[row + 1][column - 2] < 0)
      push_move(row + 1, column - 2);
    if(a[row + 1][column + 2] == BLANK || a[row + 1][column + 2] < 0)
      push_move(row + 1, column + 2);
    if(a[row + 2][column - 1] == BLANK || a[row + 2][column - 1] < 0)
      push_move(row + 2, column - 1);
    if(a[row + 2][column + 1] == BLANK || a[row + 2][column + 1] < 0)
      push_move(row + 2, column + 1);
  }
  else {			// else piece is black
    if(a[row - 1][column - 2] == BLANK || a[row - 1][column - 2] > 0)
      push_move(row - 1, column - 2);
    if(a[row - 1][column + 2] == BLANK || a[row - 1][column + 2] > 0)
      push_move(row - 1, column + 2);
    if(a[row - 2][column - 1] == BLANK || a[row - 2][column - 1] > 0)
      push_move(row - 2, column - 1);
    if(a[row - 2][column + 1] == BLANK || a[row - 2][column + 1] > 0)
      push_move(row - 2, column + 1);
    if(a[row + 1][column - 2] == BLANK || a[row + 1][column - 2] > 0)
      push_move(row + 1, column - 2);
    if(a[row + 1][column + 2] == BLANK || a[row + 1][column + 2] > 0)
      push_move(row + 1, column + 2);
    if(a[row + 2][column - 1] == BLANK || a[row + 2][column - 1] > 0)
      push_move(row + 2, column - 1);
    if(a[row + 2][column + 1] == BLANK || a[row + 2][column + 1] > 0)
      push_move(row + 2, column + 1);
  }
}

void moves_of_rook(int row, int column) {
  // initialises linked list pointer 'moves' with list of moves that the rook at (row,column) can make.
  if(a[row][column] != ROOK)
    return;
  clear_moves();
  move_straight(row, column);
}

void moves_of_queen(int row, int column) {
  // initialises linked list pointer 'moves' with list of moves that the queen at (row,column) can make.
  if(a[row][column] != QUEEN)
    return;
  clear_moves();
  move_diagonally(row, column);
  move_straight(row, column);  
}

void moves_of_king(int row, int column) {
  // initialises linked list pointer 'moves' with list of moves that the king at (row,column) can make.
  // this function needs some work afterwards, like king can't take a piece which will put it in a check.
  if(a[row][column] != KING)
    return;
  clear_moves();
  if(a[row][column] > 0) {	// if piece is white
    if(a[row - 1][column] == BLANK || a[row - 1][column] < 0)
      push_move(row - 1, column);
    if(a[row - 1][column - 1] == BLANK || a[row - 1][column - 1] < 0)
      push_move(row - 1, column - 1);
    if(a[row - 1][column + 1] == BLANK || a[row - 1][column + 1] < 0)
      push_move(row - 1, column + 1);
    if(a[row][column - 1] == BLANK || a[row][column - 1] < 0)
      push_move(row, column - 1);
    if(a[row][column + 1] == BLANK || a[row][column + 1] < 0)
      push_move(row, column + 1);
    if(a[row + 1][column] == BLANK || a[row + 1][column] < 0)
      push_move(row + 1, column);
    if(a[row + 1][column - 1] == BLANK || a[row + 1][column - 1] < 0)
      push_move(row + 1, column - 1);
    if(a[row + 1][column + 1] == BLANK || a[row + 1][column + 1] < 0)
      push_move(row + 1, column + 1);
  }
  else {			// else piece is black
    if(a[row - 1][column] == BLANK || a[row - 1][column] > 0)
      push_move(row - 1, column);
    if(a[row - 1][column - 1] == BLANK || a[row - 1][column - 1] > 0)
      push_move(row - 1, column - 1);
    if(a[row - 1][column + 1] == BLANK || a[row - 1][column + 1] > 0)
      push_move(row - 1, column + 1);
    if(a[row][column - 1] == BLANK || a[row][column - 1] > 0)
      push_move(row, column - 1);
    if(a[row][column + 1] == BLANK || a[row][column + 1] > 0)
      push_move(row, column + 1);
    if(a[row + 1][column] == BLANK || a[row + 1][column] > 0)
      push_move(row + 1, column);
    if(a[row + 1][column - 1] == BLANK || a[row + 1][column - 1] > 0)
      push_move(row + 1, column - 1);
    if(a[row + 1][column + 1] == BLANK || a[row + 1][column + 1] > 0)
      push_move(row + 1, column + 1);
  }
}

int main() {
  freopen("input.txt", "r", stdin);
  initialise_board();
  read_fen();

  show_board();
  return 1;
}

