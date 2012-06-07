/*
Ein Beispiel fuer die Verwendung von fork() und pipe().
Oder wie man in C Kindprozesse erstellt, die ueber Pipes
mit einem Vaterprozess labern.

By Bastian Ballmann
25.07.2003
*/

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


// Funktionsdefinitionen
int *mkchild(void);


// Globale Variablen
int pipes[2];


// Main part
int main(void)
{
  int check;
  char response[50];
  char *msg = "Eat my shorts!";
  int len = strlen(msg);
  memset(response,'\0',50);

  // Erzeuge einen Kindprozess
  mkchild();

  // Sende dem Kindprozess eine Nachricht
  check = write(pipes[1],msg,len);
  if(check == -1) { perror("write"); exit(1); }

  // Lese die Antwort des Kindprozesses
  check = read(pipes[0],response,50);
  if(check == -1) { perror("read"); exit(1); }
  printf("Child said: '%s'\n",response);
  return 0;
}


// Erzeuge einen Kindprozess
int *mkchild(void)
{
  int pid, check;
  int pipes_father[2], pipes_child[2];
  char msg[50];
  char *response = "You think you are funny, eh?";
  int len = strlen(response);
  memset(msg,0,50);

  // Erzeuge ein Set Pipes fuer den Vater- und den Kindprozess
  check = pipe(pipes_father);
  if(check == -1) { perror("pipe"); exit(1); }
  check = pipe(pipes_child);
  if(check == -1) { perror("pipe"); exit(1); }

  // Forke den Kindprozess
  pid = fork();
  if(pid == -1) { perror("fork"); exit(1); }

  // Vaterprozess
  if(pid != 0)
    {
      // Schreibe die Pipes in die gobale Variable pipes
      pipes[0] = pipes_child[0];
      pipes[1] = pipes_father[1];
      return 0;
    }

  // Kindprozess
  else
    {
      // Lese die Nachricht des Vaterprozesses
      check = read(pipes_father[0],msg,50);
      if(check == -1) { perror("read"); exit(1); }
      printf("Father said: '%s'\n",msg);
  
      // Sende dem Vaterprozess eine Anwort
      check = write(pipes_child[1],response,len);
      if(check == -1) { perror("write"); exit(1); }

      // Beende den Kindprozess
      exit(0);
    }
}
