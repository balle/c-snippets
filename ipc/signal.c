/*
Beispiel Code zum abfangen von Signalen

By Bastian Ballmann
25.07.2003
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sig_int(int code);
void sig_term(int code);

int main(void)
{
  signal(SIGINT,sig_int);
  signal(SIGTERM,sig_term);
  while(1){}
  return 0;
}

void sig_int(int code)
{
  printf("Got signal INT.\n");
}

void sig_term(int code)
{
  printf("Got signal TERM.\n");
}

