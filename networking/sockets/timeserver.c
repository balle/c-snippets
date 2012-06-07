/*
Just a simple internet time server

Programmed by Bastian Ballmann
22.12.2003
*/

#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define SPORT 37    // Server Port
#define QUEUE 5     // Listen queue


int main(void)
{
  int lsock,csock,socksize;
  struct sockaddr_in server, client;
  int t;

  if( (lsock = socket(AF_INET,SOCK_STREAM,0)) < 0 ) { perror("socket "); exit(1); }
  server.sin_family = AF_INET;
  server.sin_port = htons(SPORT);
  server.sin_addr.s_addr = INADDR_ANY;
  if( (bind(lsock,(struct sockaddr*)&server,sizeof(server))) < 0) { perror("bind "); exit(1); }
  listen(lsock,QUEUE);
  socksize = sizeof(struct sockaddr_in);

  while(csock = accept(lsock,(struct sockaddr*)&client,&socksize))
    {
      t = (int)time(0);
      write(csock,(char *)&t,sizeof(int));
      close(csock);
    }

  close(lsock);
  return 0;
}
