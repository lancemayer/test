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

#define SEND_PORT 61234
#define RECV_PORT 61235
#define LINE_LENGTH 50
#define MAX_BUFF 32

////////////////
//  Globals   //
////////////////

int recv_sock, send_sock = 0;
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
  int y = 1;
  int x = 5;
  char c;
  move(y, x);
  while ((c = getch()) != 27) {
    if ((c == KEY_UP) || (c == KEY_DOWN) || (c == KEY_LEFT) || (c == KEY_RIGHT)) {
      arrowKey(c, &y, &x); // processes arrow key input      
    }
    if (c == 10) {
      y = 1;
      x = 5;
      move(y, x);


      //package up current message and send to other user
    }
    //cancel threads and exit cleanly 
  }

  //create buffer for thread
}

void *recv_work() {
  char recv_buff[10][25] = {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0};

  while (1) {
    recvfrom(send_sock, recv_buff, 25, 0, NULL, NULL);
    
    int msg_count = 9;
    while (msg_count > 0) {
      // move messages down the line
    }

    // Insert new message into first slot of array

    msg_count = 0;
    while ((msg_count < 10) && (recv_buff[msg_count][0] != '\0')) {
      //print messages every other line
    }
  }

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
  
  //////////////////////////////////////////////////////
  //         Establish udp SERVER connection          //
  //////////////////////////////////////////////////////

  struct sockaddr_in recv_addr, send_addr;
  socklen_t sin_size = sizeof(send_addr);
  int recvlen;
  int ret = 0;

  memset(&my_addr, 0, sizeof(recv_addr));
  recv_addr.sin_family = AF_INET;
  recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  recv_addr.sin_port = htons(RECV_PORT);

  recv_sock = socket(AF_INET, SOCK_DGRAM, 0);

  ret = bind(sockfd, (const struct sockaddr *)&recv_addr, sizeof(recv_addr));
  if (ret != 0) {
    perror("Bind Failed");
    exit(EXIT_FAILURE);
  }

  //////////////////////////////////////////////////////
  //         Establish udp CLIENT connection          //
  //////////////////////////////////////////////////////

  memset(&recv_addr, 0, sizeof(recv_addr));
  rem_addr.sin_family = AF_INET;
  rem_addr.sin_port = htons(PORT);

  if (inet_aton(SRV_IP, &rem_addr.sin_addr) == 0) {
    printw("inet_aton() failed");
    exit(1);
  }

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);


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

  pthread_create(&send_t, NULL, send_work, NULL);
  pthread_create(&recv_t, NULL, recv_work, NULL);

  pthread_join(sent_t, NULL);
  pthread_join(recv_t, NULL);

  close(recv_sock);
  close(send_sock);
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
