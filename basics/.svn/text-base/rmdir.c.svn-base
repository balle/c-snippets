/*
Eine Reimplementierung von rmdir

By Bastian Ballmann
23.07.2003
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  if(argc < 2) { printf("Usage: %s <dir>\n",argv[0]); exit(0); }
  if( rmdir(argv[1]) == -1 ){ perror("rmdir"); exit(1); }
  return 0;
}
