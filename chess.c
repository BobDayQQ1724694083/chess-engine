#include <stdio.h>

#define BORDER	42
#define KING	10
#define QUEEN	9
#define ROOK	5
#define BISHOP	4
#define KNIGHT	3
#define PAWN	1

int a[10][10];			/* Board representation, white as positive, black as negative, blanks with zeroes */
int moves[8][8];		/* List will represent possible calculated moves for a piece, probably should be a struct and should be local to take advantage of recursion at some point in future */

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
  for(int i = 0; i < 10; i++) {
    for(int j = 0; j < 10; j++) {
      if(i == 0 || j ==0 || i == 9 || j == 9)
	a[i][j] = BORDER;
    }
  }
}

void show_board() {
  for(int i = 1; i < 9; i++) {
    for(int j = 1; j < 9; j++) 
      printf("%d\t",a[i][j]);
    printf("\n");
  }
}

int main() {
  initialise_board();
  show_board();
  
  return 1;
}

