/*
Just a simple daytime server

Programmed by Bastian Ballmann
22.07.2003
*/

// Includes
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>


// Configuration
#define SPORT 13    // Server Port
#define QUEUE 5     // Listen queue

int lsock;
void sig_int(int code);

// Main part
int main(void)
{
  int csock,socksize;
  struct sockaddr_in server_socket, client_socket;
  struct tm *zeit;
  char zeitstring[21];
  int jahr;

  signal(SIGINT,sig_int);

  // TCP socket descriptor
  if( (lsock = socket(AF_INET,SOCK_STREAM,0)) < 0 ) { perror("socket "); exit(1); }

  // Socket address
  server_socket.sin_family = AF_INET;
  server_socket.sin_port = htons(SPORT);
  server_socket.sin_addr.s_addr = INADDR_ANY;

  // Bind the socket to the specifed address
  if( (bind(lsock,(struct sockaddr*)&server_socket,sizeof(server_socket))) < 0) { perror("bind "); exit(1); }

  // Listen on the socket
  listen(lsock,QUEUE);
  socksize = sizeof(struct sockaddr_in);

  // Waiting for connections
  while(csock = accept(lsock,(struct sockaddr*)&client_socket,&socksize))
    {
      // Write the current time into the socket connection
      time_t t = time(0);
      zeit = localtime(&t);
      jahr = zeit->tm_year + 1900;
      snprintf(zeitstring,21,"%02d.%02d.%04d %02d:%02d:%02d\n",
	       zeit->tm_mday,
	       zeit->tm_mon,
	       jahr,
	       zeit->tm_hour,
	       zeit->tm_min,
	       zeit->tm_sec);
      write(csock,zeitstring,21);

      // Kick the client
      close(csock);
    }

  // Close server socket
  close(lsock);
  return 0;
}

void sig_int(int code)
{
  close(lsock);
  exit(0);
}
