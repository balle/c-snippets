/*
Just a simple reimplementation of /bin/cat

By Bastian Ballmann
23.07.2003
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
  FILE *fh;
  char c;

  if(argc < 2)
    {
      printf("Usage: %s <file>\n",argv[0]);
      exit(0);
    }

  fh = fopen(argv[1],"r");
  if(fh == NULL) { perror("cat"); exit(1); }
  while( (c = getc(fh)) != EOF) { printf("%c",c); }
  fclose(fh);

  return 0;
}
