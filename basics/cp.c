/*
Eine simple Reimplementierung von cp

By Bastian Ballmann
23.07.2003
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
  FILE *src, *dst;
  char c;

  if(argc < 3)
    {
      printf("Usage: %s <src> <dst>\n",argv[0]);
      exit(0);
    }

  src = fopen(argv[1],"r");
  if(src == NULL) { perror("cp"); exit(1); }

  dst = fopen(argv[2],"w");
  if(dst == NULL) { perror("cp"); exit(1); }

  while( (c = getc(src)) != EOF) { putc(c,dst); }
  fclose(src);
  fclose(dst);

  return 0;
}
