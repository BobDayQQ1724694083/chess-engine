#include <bits/stdc++.h>
using namespace std;

#define BORDER	42
#define KING	10
#define QUEEN	9
#define ROOK	5
#define KNIGHT	4
#define BISHOP	3
#define PAWN	1
#define BLANK	0
#define FULL    1

int a[12][12];			/* Board representation, white as positive, black as negative, blanks with zeroes */

typedef struct move {
  int row;
  int column;
  struct move * next;
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
  if (FULL) {
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

void moves_of_pawn(int row, int column) {
  // intiliases linked list pointer *moves with list of moves that the pawn at (row,column) can make.
  int sign = a[row][column] / (abs(a[row][column]));
  switch(sign) {
  case 1:
    if(a[row - 1][column] == BLANK) {
      move *p = (move *)malloc(size_of(move));
      p->row = row - 1;
      p->column = column;
      p->next = moves;
      moves = p;
    }
    if(sign != ((a[row - 1][column - 1] / (abs(a[row - 1][column - 1])))) && a[row - 1][column - 1] != BLANK) {
      move *p = (move *)malloc(size_of(move));
      p->row = row-1;
      p->column = column-1;
      p->next = moves;
      moves = p;
    }
    if(sign != ((a[row - 1][column + 1] / (abs(a[row - 1][column + 1])))) && a[row - 1][column + 1] != BLANK) {
      move *p = (move *)malloc(size_of(move));
      p->row = row - 1;
      p->column = column + 1;
      p->next = moves;
      moves = p;
    }
    break;
  case -1:
    if(a[row + 1][column] == BLANK) {
      move *p = (move *)malloc(size_of(move));
      p->row = row + 1;
      p->column = column;
      p->next = moves;
      moves = p;
    }
    if(sign != ((a[row + 1][column - 1] / (abs(a[row + 1][column - 1])))) && a[row + 1][column - 1] != BLANK) {
      move *p = (move *)malloc(size_of(move));
      p->row = row-1;
      p->column = column-1;
      p->next = moves;
      moves = p;
    }
    if(sign != ((a[row + 1][column + 1] / (abs(a[row + 1][column + 1])))) && a[row + 1][column + 1] != BLANK) {
      move *p = (move *)malloc(size_of(move));
      p->row = row + 1;
      p->column = column + 1;
      p->next = moves;
      moves = p;
    }
    break;
  }
}

void moves_of_bishop(int row, int column) {
  // intiliases linked list pointer *moves with list of moves that the bishop at (row,column) can make.
  int sign = a[row][column] / (abs(a[row][column]));
}

void moves_of_knight(int row, int column) {
  // intiliases linked list pointer *moves with list of moves that the knight at (row,column) can make.
  int sign = a[row][column] / (abs(a[row][column]));
}

void moves_of_rook(int row, int column) {
  // intiliases linked list pointer *moves with list of moves that the rook at (row,column) can make.
  int sign = a[row][column] / (abs(a[row][column]));
}

void moves_of_queen(int row, int column) {
  // intiliases linked list pointer *moves with list of moves that the queen at (row,column) can make.
  int sign = a[row][column] / (abs(a[row][column]));
}

void moves_of_king(int row, int column) {
  // intiliases linked list pointer *moves with list of moves that the king at (row,column) can make.
  int sign = a[row][column] / (abs(a[row][column]));
}


int main() {
  freopen("input.txt", "r", stdin);
  initialise_board();
  read_fen();

  show_board();
  return 1;
}

