#include <ncurses.h>

int main() {
  initscr();
  keypad(stdscr, TRUE);
  noecho();
  int c = 0;
  
  do {
    move (0, 0);
    printw("Write something (CTRL-C to escape): ");
    move(2, 0);
    printw("Keycode: %d    \nCharacter: %c    ", c, c);
    refresh();
  } while (c = getch());

  endwin();
  return 0;
}
