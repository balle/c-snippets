/*
Eine Reimplementierung von tail -f

By Bastian Ballmann
23.07.2003
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[])
{
  FILE *fh;
  int old, new;
  char c;

  if(argc < 2)
    {
      printf("Usage: %s <file>\n",argv[0]);
      exit(0);
    }

  // Datei oeffnen
  fh = fopen(argv[1],"r");
  if(fh == NULL) { perror("tail"); exit(1); }

  // Springe ans Ende der Datei und merke Dir die Position
  fseek(fh,0,SEEK_END);
  old = ftell(fh);

  while(1)
    {
      // Springe an Ende der Datei
      fseek(fh,0,SEEK_END);
      new = ftell(fh);

      // Ist das alte Ende gleich dem aktuellen Ende?
      if(old != new)
	{
	  // Falls Daten hinzu gekommen sind, spring an das
	  // alte Ende und gib alles bis EOF aus.
	  fseek(fh,old,SEEK_SET);

	  while( (c = getc(fh)) != EOF)
	    {
	      printf("%c",c);
	    }

	  old = ftell(fh);
	}

      sleep(1);
    }

  fclose(fh);
  return 0;
}
