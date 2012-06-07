/*
  An example implementation of inetd
  the internet super server

  Programmed by Bastian Ballmann
  http://www.geektown.de
  Last update: 22.12.2003
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pwd.h>
#include <signal.h>
#include <sys/wait.h>
#include <syslog.h>

#define CONF "inetd.conf"
#define QUEUE 10

typedef struct inetd
{
  char name[10];
  char type[10];
  char prot[10];
  char wait[8];
  char user[20];
  char path[256];
  char prog[256];
  char args[256];
  int socket;
  int childpid;
  struct inetd *next;
} inetd;

inetd *anfang = NULL;
fd_set readfds;
int nfds;
char msg[256];

inetd *get_new_inetd(void);
inetd *search_socket(fd_set readfds);
inetd *search_client(int pid);
void accept_connections(void);
void sig_child(int code);
void sig_int(int code);
void log(char *msg);

int main(void)
{
  FILE *fh;
  char line[1024];
  struct sockaddr_in server;
  struct protoent *proto;
  struct servent *service;
  inetd *last = NULL;
  register inetd *inetd;

  // Biste root?
  if(getuid() != 0)
    {
      printf("You must be root!\n");
      exit(1);
    }

  // Initialisiere readfds Array fuer select()
  FD_ZERO(&readfds);

  // Oeffne die Config Datei zum lesen
  if( (fh = fopen(CONF,"r")) == NULL)
    {
      perror("open");
      exit(1);
    }

  // Verarbeite die Eingabe zeilenweise
  while(fgets(line,1024,fh) != NULL)
    {
      // Leerzeichen am Anfang der Zeile auslassen
      while(*zeiger == ' '){ zeiger++; }

      // Auskommentierte Zeilen auslassen
      if(*line != '#')
	{
	  // Reserviere Speicher fuer eine neue inetd Struktur
	  inetd = get_new_inetd();

	  // Ist das die erste inetd Struktur?
	  // Dann merke sie Dir als den Anfang
	  if(last == NULL)
	    {
	      anfang = inetd;
	    }
	  
	  // Ansonsten zeigt der letzte Pointer auf den aktuellen
	  else
	    {
	      last->next = inetd;
	    }
	  
	  last = inetd;
	  
	  // Zerlege jede Zeile in ihre Bestandteile
	  sscanf(line,"%10s %10s %10s %8s %20s %256s %256s %256s",
		 inetd->name, 
		 inetd->type, 
		 inetd->prot, 
		 inetd->wait, 
		 inetd->user, 
		 inetd->path, 
		 inetd->prog, 
		 inetd->args);
	  
	  // Finde das Transport Protokoll heraus
	  if( (proto = getprotobyname(inetd->prot)) == NULL)
	    {
	      snprintf(msg,256,"Unkown protocol %s!\n",inetd->prot);
	      log(msg);
	      exit(1);
	    }
	  
	  // Finde das Application Layer Protokoll heraus
	  if( (service = getservbyname(inetd->name,proto->p_name)) == NULL)
	    {
	      snprintf(msg,256,"Unkown protocol %s!\n",inetd->name);
	      log(msg);
	      exit(1);
	    }
	  
	  // Adresse des Sockets
	  server.sin_family = AF_INET;
	  server.sin_port = service->s_port;
	  server.sin_addr.s_addr = INADDR_ANY;
	  
	  // Oeffne einen Socket
	  // TCP
	  if(!strcmp(inetd->type,"stream"))
	    {
	      if( (inetd->socket = socket(AF_INET,SOCK_STREAM,proto->p_proto)) < 0)
		{
		  perror("socket");
		  exit(1);
		}
	    }

	  // UDP
	  else
	    {
	      if( (inetd->socket = socket(AF_INET,SOCK_DGRAM,proto->p_proto)) < 0)
		{
		  perror("socket");
		  exit(1);
		}
	    }
	  
	  // Binde den Socket an die Adresse
	  if( (bind(inetd->socket, (struct sockaddr *)&server, sizeof(server))) < 0)
	    {
	      perror("bind");
	      exit(1);
	    }
	  
	  // Setze die Listen Queue bei TCP Sockets
	  if(!strcmp(inetd->type,"stream"))
	    {
	      listen(inetd->socket,QUEUE);
	    }

	  // Setze den Socket auf die select() Liste
	  FD_SET(inetd->socket,&readfds);
	  nfds = inetd->socket;
	}
    }

  fclose(fh);

  // Warte auf Verbindungsanfragen
  accept_connections();
  return 0;
}


// Warte und bearbeite Verbindungsanfragen
void accept_connections(void)
{  
  int client_sock = 0, socksize = sizeof( struct sockaddr_in );
  register inetd *inetd;
  struct sockaddr_in client;
  struct passwd *pass;

  // Nachdem alle Sockets erstellt wurden, ueberwache sie auf
  // Verbindungen mittels select()
  log("Waiting for connections...\n");
  while(select(nfds+1, &readfds, NULL, NULL, NULL)) 
    {
      // Suche den inetd Eintrag zum Socket
      if( (inetd = search_socket(readfds)) == NULL)
	{
	  snprintf(msg,256,"Error while searching for inetd entry!\n");
	  log(msg);
	  exit(1);
	}
      
      // Akzeptiere die Verbindung bei TCP Sockets
      if(!strcmp(inetd->type,"stream"))
	{
	  if( (client_sock = accept(inetd->socket, (struct sockaddr *)&client, &socksize)) < 0 )
	    {
	      perror("accept");
	    }
	  
	  snprintf(msg,256,"Client %s:%d connected to %s server\n",
		   inet_ntoa(client.sin_addr), 
		   client.sin_port, 
		   inetd->name);
	  log(msg);
	}

      // Forke einen Kindprozess
      if( (inetd->childpid = fork()) < 0)
	{
	  perror("fork");
	  exit(1);
	}
      
      // Kindprozess
      if(inetd->childpid == 0)
	{
	  if(!strcmp(inetd->type,"stream"))
	    {
	      // Schliesse STDIN, STDOUT und STDERR
	      close(0);
	      close(1);
	      close(2);
	      
	      // Biege STDIN, STDOUT, STDERR zum Client Socket um
	      dup2(client_sock,0);
	      dup2(client_sock,1);
	      dup2(client_sock,2);
	  
	      // Schliesse den Client Socket
	      close(client_sock);
	    }

	  // Setze UID und GID wenn der Prozess nicht als root laufen soll
	  if(strcmp(inetd->user,"root"))
	    {
	      if( (pass = getpwnam(inetd->user)) == NULL)
		{
		  snprintf(msg,256,"Unkown user %s!\n",inetd->user);
		  log(msg);
		}
	      else
		{
		  setgid(pass->pw_gid);
		  setuid(pass->pw_uid);
		}
	    }
	  
	  // Starte den Server Prozess
	  execl(inetd->path, inetd->args, NULL);
	}
      
      // Vaterprozess
      else
	{
	  // Nimm den Socket von der select() Liste,
	  // wenn wir auf den Client warten sollen
	  if(!strcmp(inetd->wait,"wait"))
	    {
	      FD_CLR(inetd->socket,&readfds);	      
	    }

	  // Schliesse den Client Socket
	  if(!strcmp(inetd->type,"stream"))
	    {
	      close(client_sock);
	    }
	}
    }
}


// Initialisiere eine neue Inetd Struktur
inetd *get_new_inetd(void)
{
  register inetd *newPtr = malloc(sizeof(struct inetd));

  if(newPtr == NULL)
    {
      snprintf(msg,256,"FATAL ERROR: Ran out of memory in get_new_inetd!\n");
      log(msg);
      exit(1);
    }
  else
    {
      newPtr->name[0] = '\0';
      newPtr->type[0] = '\0';
      newPtr->prot[0] = '\0';
      newPtr->wait[0] = '\0';
      newPtr->user[0] = '\0';
      newPtr->path[0] = '\0';
      newPtr->prog[0] = '\0';
      newPtr->args[0] = '\0';
      newPtr->socket = 0;
      newPtr->childpid = 0;
      newPtr->next = NULL;
    }

  return newPtr;
}


// Suche nach der Inetd Struktur mit dem
// passenden Socket Deskriptor
inetd *search_socket(fd_set readfds)
{
  register inetd *current = anfang;

  while(current != NULL)
    {
      if(FD_SET(current->socket, &readfds))
	{
	  return current;
	}
      
      current = current->next;
    }

  return current;
}


// Suche nach der Inetd Struktur mit der
// passenden Child PID
inetd *search_child(int pid)
{
  register inetd *current = anfang;

  while(current != NULL)
    {
      if(current->childpid == pid)
	{
	  return current;
	}
      
      current = current->next;
    }

  return current;
}


// Wenn ein Kindprozess beendet wird, der einen
// wartenden Server Prozess bedient hat, dann
// schmeiss den Socket wieder auf die select()
// Liste
void sig_child(int code)
{
  inetd *inetd;
  int status = -1;
  pid_t pid = wait(&status);

  if( ((inetd = search_child(pid)) != NULL) && (!strcmp(inetd->type,"wait")) )
    {
      FD_SET(inetd->socket, &readfds);
      accept_connections();
    }
}


// Schliesse alle Sockets bei einem Interrupt Signal
void sig_int(int code)
{
  inetd *old;
  register inetd *current;

  while(current != NULL)
    {
      close(current->socket);
      old = current;
      current = current->next;
      free(old);
    }
}


// Sende eine Nachricht an den Syslog Daemon
void log(char *msg)
{
  openlog("inetd",LOG_PID,LOG_DAEMON);
  syslog(LOG_INFO,msg);
  closelog();
}

/* EOF */
