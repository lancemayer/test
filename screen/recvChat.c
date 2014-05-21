#include <ncurses.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>


#define LINE_LENGTH 50
#define MAX_BUFF 25

void *send_thread() {
  //create buffer for thread
}

void *recv_thread() {
  //create buffer for thread
  //set local nbytes
  //while loop for recv thread
}

void arrowKey(int, int*, int*);

int main() {

  ///////////////////////////////////////////////
  //        Draw screen for chat layout        //
  ///////////////////////////////////////////////

  initscr();
  int y = 3;
  int x = 0;

  move(y, x);
  while (x < (LINE_LENGTH + 1)) {
    printw("-");
    x = x + 1;
  }
  
  ///////////////////////////////////////////////
  //         Establish udp connection          //
  ///////////////////////////////////////////////

  int port = 61111;
  int sockfd, connfd = 0;
  struct sockaddr_in my_addr;
  socklen_t sin_size;

  char recv_buff[MAX_BUFF] = {0};
  int nbytes = 0;
  int ret = 0;

  memset(&my_addr, 0, sizeof(struct sockaddr_in));
  my_addr.sin_family = AF_INET;
  my_addr.sin_addr.s_addr = INADDR_ANY;
  my_addr.sin_port = htons(port);

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  ret = bind(sockfd, (const struct sockaddr *)&my_addr, sizeof(my_addr));
  if (ret != 0) {
    perror("Bind Failed");
    exit(EXIT_FAILURE);
  }

  ///////////////////////////////////////////////

  noecho();
  y = 1;
  x = 0;
  int c;
  char temp;
  char send_buff[MAX_BUFF] = {0};
  keypad(stdscr, TRUE);
  move(y, x);
  printw("Me: ");
  x = 5;
  move(y, x);
  while ((c = getch()) != 27) {
    if ((c == KEY_UP) || (c == KEY_DOWN) || (c == KEY_LEFT) || (c == KEY_RIGHT)) {
      arrowKey(c, &y, &x); // processes arrow key input
    }
    else if (c == 10) {
      /*
       * Insert code that packages up line and sends to other user
       * Testing just packages up code and places it below separator
       */
      /////////////////////////////
      // Get message from sender
      x = 5;
      while (x != LINE_LENGTH) {
	move(y, x);
	temp = inch();
	strcat(send_buff, &temp);
	x = x + 1;
      }

      // send buffer to other user

      ///////////////////////////////////
      // Print message below separator
      //addstr(send_buff);
      //send_buff[0] = '\0';

      while (1) {
	mvaddch(5, 3, '>');
	x = 5;
	y = 5;
	move(y, x);
	nbytes =recvfrom(sockfd, recv_buff, MAX_BUFF, 0, NULL, NULL);
	
	addstr(recv_buff);
	refresh();
      }

      /////////////////////////////////////////
      // Return cursor to begin new message
      y = 1;
      x = 5;
      move(y, x);

      int count = LINE_LENGTH - 5;
      while (count > 0) {
	delch();
	count = count - 1;
      }
    }
    else if (c == KEY_BACKSPACE) {
      if (x != 5) {
	x = x - 1;
	move(y, x);
	printw(" ");
	move(y, x);
      }
    }
    else if (c == KEY_DC) {
      delch();
    }
    else {
      if (x != (LINE_LENGTH)) {
	addch(c);
	x = x + 1;
	move(y, x);
      }
    }
  }

  endwin();
  return 0;
}

void arrowKey(int c, int* y, int* x) {
  switch (c) {
  case KEY_UP:
    //if (*y != 0) 	
    //*y = *y - 1;
    //move(*y, *x);
    break;
  case KEY_DOWN:
    //*y = *y + 1;
    //move(*y, *x);
    break;
  case KEY_LEFT:
    if (*x != 5) {
      *x = *x - 1;
      move(*y, *x);
    }
    break;
  case KEY_RIGHT:
    if (*x < (LINE_LENGTH)) {
      *x = *x + 1;
      move(*y, *x);
    }
    break;
  }
}
