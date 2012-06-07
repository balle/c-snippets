/*
Just a simple time client

Programmed by Bastian Ballmann
22.07.2003
*/

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#define PORT 37

int main(int argc, char *argv[])
{
  int sock,check;
  struct sockaddr_in server;
  struct hostent *host;
  char zeit[7];

  // No arguments?
  if(argc < 2){ printf("Usage: %s <timeserver>\n",argv[0]); exit(1); }
  
  // TCP socket descriptor
  sock = socket(AF_INET,SOCK_STREAM,6);
  if(sock == -1) { perror("socket "); exit(1); }
  
  // Socket address
  host = gethostbyname(argv[1]);
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = inet_addr(host->h_name);

  // Connect to the server
  check = connect(sock, (struct sockaddr *)&server, sizeof(server));
  if(check == -1) { perror("Cannot connect "); exit(1); }

  read(sock,zeit,8);
  printf("Time: %s\n",zeit);
  return 0;
}
