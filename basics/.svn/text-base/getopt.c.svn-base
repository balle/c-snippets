/*
Ein Beispiel fuer die Verwendung von getopt

By Bastian Ballmann
25.07.2003
*/

#include <stdio.h>
#include <getopt.h>

int main(int argc, char *argv[])
{
  char c;
  char *opts = "abc:";

  while( (c = getopt(argc,argv,opts)) != -1)
    {
      switch(c)
	{
	case 'a':
	  printf("Found parameter -a.\n");
	  break;
	  
	case 'b':
	  printf("Found parameter -b.\n");
	  break;
	  
	case 'c':
	  printf("Found parameter -c with value %s.\n",optarg);
	  break;
	  
	defaults:
	  printf("Unrecognized parameter %c\n",c);
	}
    }

  return 0;
}
