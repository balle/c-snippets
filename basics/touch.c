/*
Just a simple reimplementation of /bin/touch

By Bastian Ballmann
23.07.2003
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
  FILE *fh;

  if(argc < 2)
    {
      printf("Usage: %s <file>\n",argv[0]);
      exit(0);
    }

  fh = fopen(argv[1],"w");
  if(fh == NULL) { perror("touch"); exit(1); }
  fclose(fh);

  return 0;
}
