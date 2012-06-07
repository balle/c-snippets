/*
  Rechne einen ASCII String nach Binaer um
  Programmed by Bastian Ballmann
  28.05.2004
*/

#include <stdio.h>

struct code
{
  char ascii;
  char binary[8];
};

int main(int argc, char *argv[])
{
  unsigned char *input = argv[1];
  struct code *output, **gesamt;
  unsigned char i; 
  int strsize, x, a  = 0;

  if(argc < 2)
    {
      printf("%s <string>\n", argv[0]);
      exit(0);
    }

  strsize = strlen(input);

  if( (gesamt = (struct code **)malloc(strsize + 1 * sizeof(struct code *))) == NULL)
    {
      printf("Damn! We ran out of memory... :(\n");
      exit(1);
    }

  /* Zeichenweise durch den Input String laufen */
  while(*input != '\0')
    {
      /* Output Buffer initialisieren */
      if((output = (struct code *)malloc(sizeof(struct code))) == NULL)
	{
	  printf("Damn! We ran out of memory! :(\n");
	  exit(1);
	}

      memset(output->binary,'0',8);

      /* Nacheinander die 2er Potenzen durchlaufen und bitweise mit dem
	 Zeichen AND verknuepfen (kucken ob das Bit gesetzt ist) */
      for(i = 1, x = 7; x >= 0; i *= 2, x--)
	{
	  if(*input & i)
	    output->binary[x] = '1';
	  else
	    output->binary[x] = '0';
	}

      /* Output String terminieren und in gesamt speichern */
      output->ascii = *input;
      output->binary[8] = '\0';
      gesamt[a++] = output;

      /* Das naechste Zeichen bitte */
      input++;
    }

  /* Ausgabe */
  for(a = 0; a < strsize; a++)
	printf("%c %s\n", gesamt[a]->ascii, gesamt[a]->binary);

  return 0;
}
