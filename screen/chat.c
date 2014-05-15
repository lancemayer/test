#include <ncurses.h>

#define LINE_LENGTH 80

void arrowKey(int, int*, int*);

int main() {
  initscr();
  noecho();
  int y = 0;
  int x = 0;
  int c;
  keypad(stdscr, TRUE);
  move(y, x);
  while ((c = getch()) != 27) {
    if ((c == KEY_UP) || (c == KEY_DOWN) || (c == KEY_LEFT) || (c == KEY_RIGHT)) {
      arrowKey(c, &y, &x); // processes arrow key input
    }
    else if (c == 10) {
      y = y + 1;
      x = 0;
      move(y, x);
    }
    else {
      printw("%c", c); // prints characters from input
      x = x + 1;
      if (x > 80) {
	x = 0;
	y = y + 1;
      }
      move(y, x);
    }
  }

  endwin();
  return 0;
}

void arrowKey(int c, int* y, int* x) {
  switch (c) {
  case KEY_UP:
    if (*y != 0) 	
      *y = *y - 1;
    move(*y, *x);
    break;
  case KEY_DOWN:
    *y = *y + 1;
    move(*y, *x);
    break;
  case KEY_LEFT:
    if ((*x == 0) && (*y != 0)) {
      *x = 80;
      *y = *y - 1; // wraps cursor to previous line
    }
    else if (*x != 0) {	
      *x = *x - 1;
    }
    move(*y, *x);
    break;
  case KEY_RIGHT:
    *x = *x + 1;
    if (*x > 80) { // wraps cursor to next line
      *x = 0;
      *y = *y + 1;
    }
    move(*y, *x);
    break;
  }
}
