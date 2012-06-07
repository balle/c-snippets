/* 
   Beispiel Code zum erstellen eines Daemons 
   By Bastian Ballmann
   09.08.2003
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(void)
{
  printf("Becoming a daemon...\n");
  
  // Dateihandles schliessen
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  // INT Signal ignorieren
  signal(SIGINT,SIG_IGN);

  // Fork in den Background und exit() Vaterprozess
  if(fork() != 0)
    {
      exit(0);
    }

  // Change working directory
  chdir("/");

  // Vom tty los loesen
  setpgrp();

  // Mach was oder auch nichts...
  while(1){}

  return 1;
}
