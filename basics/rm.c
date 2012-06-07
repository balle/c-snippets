/*
Eine simple Reimplementation von /bin/rm

By Bastian Ballmann
23.07.2003
*/

#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
      printf("Usage: %s <file>\n",argv[0]);
      exit(0);
    }
    
    if( unlink(argv[1]) == -1 ) { perror("rm"); exit(1); }
    return 0;
}
