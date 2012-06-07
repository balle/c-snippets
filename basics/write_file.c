#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
  FILE* fh;

  fh = fopen("test.txt","w");

  if(fh != NULL)
    {
      fputs("you have been hacked.\n",fh);
      fclose(fh);
    }
  
  return 0;
}
