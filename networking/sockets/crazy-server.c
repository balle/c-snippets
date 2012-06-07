/* Playing around with sockets in C 
   Programmed by Bastian Ballmann
   bytebeater@crazydj.de
   http://www.crazydj.de */

// ---:[ INCLUDES ]:---

// Standard IO functions
#include <stdio.h>

// Standard library
#include <stdlib.h>

// Error handling
#include <errno.h>

// Systems data types
#include <sys/types.h>

// Socket systems calls
#include <sys/socket.h>

// Functions like gethostbyname
#include <netdb.h>

// Functions like inet_addr
#include <arpa/inet.h>

// Protocoll types
#include <netinet/in.h>

// Systems calls like fork()
#include <unistd.h>

// File descriptor handling
#include <fcntl.h>

// time functions
#include <sys/time.h>


// ---:[ DEFINE FUNTION HEADERS ]:---
int fork_child(int *clientfd);


// ---:[ CONFIGURATION ]:--

// Server source port
#define SOURCEPORT 7777

// Queued connections
#define QUEUE 5

// Should the Client server be in BLOCKING mode?
// 0 == no / 1 == yes
#define FDOPT 0


// ---:[ MAIN PART ]:---

int main(void)
{
  // Declarations
  // Socket descriptors
  int sockfd, clientfd;

  // Check return values
  int check;

  // Client socket size
  int socksize;

  // Protocoll struct
  struct protoent *protocol;

  // Protocoll ID
  int proto;

  // Host struct
  struct hostent *host;

  // Server socket address information
  struct sockaddr_in server_socket;

  // Connected client socket address information
  struct sockaddr_in client_socket;


  // Create the server socket descriptor
  protocol = getprotobyname("tcp");
  proto = protocol->p_proto;
  sockfd = socket(AF_INET,SOCK_STREAM,proto);

  // Failed?
  if(sockfd == -1)
    {
      perror("socket ");
      exit(1);
    }

  printf("Created socket descriptor.\n");

  // Define server socket information
  host = gethostbyname("localhost");
  server_socket.sin_family = AF_INET;
  server_socket.sin_port = htons(SOURCEPORT);
  server_socket.sin_addr.s_addr = inet_addr("127.0.0.1");

  // Bind the socket on the specified port
  check = bind(sockfd,(struct sockaddr*)&server_socket,sizeof(server_socket));

  // Failed?
  if(check == -1)
    {
      perror("Bind ");
    }

  printf("Socket was bind on %s:%d\n",host->h_name,SOURCEPORT);

  // The server should listen for incoming connections
  check = listen(sockfd,QUEUE);

    // Failed?
  if(check == -1)
    {
      perror("Listen ");
      exit(1);
    }

  printf("Listening for incoming connections...\n");

  // OK. The server socket is now listening for incoming connections
  // Endless loop...
  while(1)
    {
      // Got a connection request!
      socksize = sizeof(struct sockaddr_in);
      clientfd = accept(sockfd,(struct sockaddr*)&client_socket,&socksize);

      // Failed?
      if(clientfd == -1)
	{
	  perror("Accept ");
	}

      printf("Got a connections request from client %s:%d.\n",inet_ntoa(client_socket.sin_addr),client_socket.sin_port);

      // Set the client socket to non-blocking mode?
      if(FDOPT == 0)
	{
	  check = fcntl(clientfd,F_SETFL,O_NONBLOCK);

	  if(check == -1)
	    {
	      perror("Fcntl");
	    }

	  printf("Client socket was set to non-blocking mode!\n");
	}

      fork_child(&clientfd);
      close(clientfd);
    }

  return 0;
}


// Create a child to talk to the connected client
int fork_child(int *clientfd)
{
  // Childs PID
  int pid;

  // Writable file descriptor
  int wfd;

  // File descriptors ready to write to
  fd_set writefds;

  // Set a socket timeout
  struct timeval timeout;
  timeout.tv_sec = 10;
  timeout.tv_usec = 0;

  printf("Forking child process...\n");

      // Fork the child process
      pid = fork();

      if(pid == 0)
	{
	  return pid;
	}
      else
	{
      	  perror("Fork");
	}

      // child process goes here

      // Clear the file descriptor set
      FD_ZERO(&writefds);

      // Push the socket descriptor on the monitor list
      FD_SET(*clientfd,&writefds);
      
      // monitor if the client socket is ready for writing
      wfd = select(*clientfd+1,NULL,&writefds,NULL,&timeout);

      // Check if the socket is ready for writing
      if(wfd != -1)
	{
	  printf("Talking to client.\n");
	  send(*clientfd,"You suck! =)\n",13,0);
	  send(*clientfd,"Killing ya connection.\n",23,0);
	}
      else
	{
	  printf("Connection of client timed out...\n");
	}

      printf("Killing child process %d.\n",pid);

      // Close the socket
      close(*clientfd);

      // Remove the socket from the monitor list
      FD_CLR(*clientfd,&writefds);
      
      // Kill child process
      exit(0);
}
