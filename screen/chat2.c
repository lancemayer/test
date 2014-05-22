#include <ncurses.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

#define SRV_IP "127.0.0.1"

#define OTHER_PORT 61234
#define MY_PORT 61234
#define LINE_LENGTH 50
#define MAX_BUFF 32

////////////////
//  Globals   //
////////////////

pthread_t send_t;
pthread_t recv_t;


//////////////////////
//  Signal Handler  //
//////////////////////

void user() {
  pthread_cancel(send_t);
  pthread_cancel(recv_t);
}


//////////////////////////////////////
//  Sending and receiving threads   //
//////////////////////////////////////

void *send_work() {
  //////////////////////////////////////////////////////
  //         Establish udp CLIENT connection          //
  //////////////////////////////////////////////////////

  int other_sock = 0;
  struct sockaddr_in other_addr;
  int slen = sizeof(other_addr);

  memset((char *)&other_addr, 0, sizeof(other_addr));
  other_addr.sin_family = AF_INET;
  other_addr.sin_port = htons(OTHER_PORT);

  if (inet_aton(SRV_IP, &other_addr.sin_addr) == 0) {
    printw("inet_aton() failed");
    exit(1);
  }

  other_sock = socket(AF_INET, SOCK_DGRAM, 0);

  int y = 1;
  int x = 5;
  char c;
  char temp;
  char send_buff[25] = {0};
  int buff_len = 0;
  move(y, x);
  while ((c = getch()) != 27) {
    if ((c == KEY_UP) || (c == KEY_DOWN) || (c == KEY_LEFT) || (c == KEY_RIGHT)) {
      arrowKey(c, &y, &x); // processes arrow key input      
    }
    else if (c == 10) {
      bzero(send_buff, 25);
      y = 1;
      x = 5;
      while (x < 29) {
	move(y, x);
	temp = inch();
	strcat(send_buff, &temp);
	x = x + 1;
      }
      buff_len = strlen(send_buff);
      sendto(other_sock, send_buff, MAX_BUFF, 0, &other_addr, slen);
    }
    //cancel threads and exit cleanly 
  }
  close(other_sock);
}

void *recv_work() {
  //////////////////////////////////////////////////////
  //         Establish udp SERVER connection          //
  //////////////////////////////////////////////////////
  int y, x;
  int my_sock = 0;
  struct sockaddr_in my_addr;
  socklen_t sin_size = sizeof(my_addr);
  int recvlen;
  int ret = 0;

  memset(&my_addr, 0, sizeof(my_addr));
  my_addr.sin_family = AF_INET;
  my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  my_addr.sin_port = htons(MY_PORT);

  my_sock = socket(AF_INET, SOCK_DGRAM, 0);

  ret = bind(my_sock, (const struct sockaddr *)&my_addr, sizeof(my_addr));
  if (ret != 0) {
    perror("Bind Failed");
    exit(EXIT_FAILURE);
  }

  char recv_buff[25];
  //char recv_buff[10][25] = {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0};

  while (1) {
    bzero(recv_buff, 25);
    recvfrom(my_sock, recv_buff, 25, 0, NULL, NULL);

    y = 5;
    x = 5;
    move(y, x);

    printw(recv_buff);
    
    /*
    int msg_count = 9;
    while (msg_count > 0) {
      // move messages down the line
    }

    // Insert new message into first slot of array

    msg_count = 0;
    while ((msg_count < 10) && (recv_buff[msg_count][0] != '\0')) {
      //print messages every other line
      }*/
  }
  close(my_sock);
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
  

  /////////////////////////////////
  //       Signal Handling       //
  /////////////////////////////////

  struct sigaction mysig;

  sigset_t proccess_mask;

  mysig.sa_handler = &user;

  sigfillset( &mysig.sa_mask );

  sigaction( SIGTERM, &mysig, NULL);

  sigfillset( &proccess_mask );
  sigdelset( &proccess_mask, SIGTERM);

  pthread_sigmask(SIG_SETMASK, &proccess_mask, NULL);


  ///////////////////////////////////////////////
  //          Screen Interaction               //
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

  pthread_t send_t;
  pthread_t recv_t;

  pthread_create(&send_t, NULL, send_work, NULL);
  pthread_create(&recv_t, NULL, recv_work, NULL);

  pthread_join(send_t, NULL);
  pthread_join(recv_t, NULL);

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
