#include <stdio.h>
#include <stdlib.h>

// Socket systemcalls
#include <sys/socket.h>

// Protocoll types
#include <netinet/in.h>

// Error handling
#include <errno.h>

// functions like gethostbyname
#include <netdb.h>


// Config
#define SERVER "213.240.167.89"
#define SOURCE 7777
#define DEST 25


int main(void)
{
  int sock, check;
  char command[2000];
  char result[2000];
  struct sockaddr_in address;
  struct hostent *host;

  // Domain, Type, Protocol returns descriptor (int)
  // This socket should be an Internet socket which handles byte streams
  // Protocol 0 == auto detection
  // Could also be eg 6 for TCP like defined in netinet/in.h
  sock = socket(AF_INET,SOCK_STREAM,0);

  // Fehler?
  if(sock < 0)
    {
      printf("Cannot create the socket...\n\n");
      perror("");
      exit(0);
    }


  // Socket addressieren
  host = gethostbyname(SERVER);

  // Socket type
  address.sin_family = AF_INET;
  // Port
  address.sin_port = htons(DEST);
  // Destination IP
  address.sin_addr.s_addr = inet_addr(host->h_name);
  printf("Connecting to %s:%d\n",host->h_name,DEST);

  // Connect the socket (descriptor, struct sockaddr, sockaddr length)
  check = connect(sock, (struct sockaddr *)&address, sizeof(address));

  if(check < 0)
    {
      printf("Cannot connect...\n");
      perror("");
    }
  else
    {
      printf("Connected.\n");
      read(sock,result,1024);
      printf("Got result: %s\n",result);
      
      strcpy(command,"HELO\n");
      write(sock,command,strlen(command));
      printf("Wrote %s\n",command);

      read(sock,result,1024);
      printf("Got result: %s\n",result);

      strcpy(command,"MAIL FROM: krasser@socket.bla\n");
      write(sock,command,strlen(command));
      printf("Wrote: %s\n",command);

      read(sock,result,1024);
      printf("Got result: %s\n",result);

      strcpy(command,"RCPT TO: jaeger@co-de.de\n");
      write(sock,command,strlen(command));
      printf("Wrote: %s\n",command);

      read(sock,result,1024);
      printf("Got result: %s\n",result);
      
      strcpy(command,"DATA\n");
      write(sock,command,strlen(command));
      printf("Wrote: %s\n",command);

      read(sock,result,1024);
      printf("Got result: %s\n",result);

      strcpy(command,"Subject: Programming socketz in C\n");
      write(sock,command,strlen(command));
      printf("Wrote: %s\n",command);

      strcpy(command,"Karl ist ein  Arsch! =)\n");
      write(sock,command,strlen(command));
      printf("Wrote: %s\n",command);

      strcpy(command,".\n");
      write(sock,command,strlen(command));
      printf("Wrote: %s\n",command);

      strcpy(command,"QUIT\n");
      write(sock,command,strlen(command));
      printf("Wrote %s\n",command);

      read(sock,result,1024);
      printf("Got result: %s\n",result);
    }

  return 1;
}

