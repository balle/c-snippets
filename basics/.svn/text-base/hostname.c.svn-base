/*
Eine Reimplementierung von hostname

By Bastian Ballmann
23.07.2003
*/

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  FILE *fh;
  char c;

  fh = fopen("/proc/sys/kernel/hostname","r");
  if(fh == NULL){ exit(1); }
  while( (c = getc(fh)) != EOF) { printf("%c",c); }
  return 0;
}
