#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
  FILE* fh;
  char c;

  if(argc < 2)
    {
      printf("I need a file...\n");
      exit(0);
    }

  fh = fopen(argv[1],"r");

  if(fh != NULL)
    {
      while( (c=getc(fh)) != EOF)
	{
	  printf("%c",c);
	}
    }
  else
    {
      printf("Cannot read file %s",argv[1]);
      exit(0);
    }
  
  return 1;
}
