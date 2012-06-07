#include <stdio.h>

int main(int argc, char *argv[])
{
  unsigned char *input = argv[1];
  char *output;
  unsigned char i; 
  int strsize, a, x;

  if(argc < 2)
    {
      printf("%s <string>\n", argv[0]);
      exit(0);
    }

  strsize = strlen(input);
  x = strsize * 8 - 1;
  
  if( (output = (char *)malloc(8 * strsize * sizeof(char))) == NULL)
    {
      printf("Damn! We ran out of memory! :(\n");
      exit(1);
    }

  /* Output Buffer initialisieren */
  memset(output, '\0', 8 * strsize);

  /* Zeichenweise durch den Input String laufen */
  while(*input != '\0')
    {
      /* Nacheinander die 2er Potenzen durchlaufen und bitweise mit dem
	 Zeichen AND verknuepfen (kucken ob das Bit gesetzt ist) */
      for(i = 1, a = 7; a >= 0; i *= 2, a--)
	{
	  if(*input & i)
	      output[x--] = '1';
	  else
	      output[x--] = '0';
	}

      /* Das naechste Zeichen bitte */
      input++;
    }

  printf ("%s\n\n", output);

  for(x = 0, a = 1; x < strsize * 8; x++, a++)
    {
      printf("%c", output[x]);

      if(a == 6)
	{
	  a = 0;
	  printf(" ");
	}
    }

  while(a <= 6)
    {
      printf("0");
      a++;
    }

  printf("\n");
  return 0;
}
