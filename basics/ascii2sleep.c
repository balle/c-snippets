/*
  Rechne einen ASCII String nach Binaer um
  Programmed by Bastian Ballmann
  27.05.2004
*/

int main(int argc, char *argv[])
{
  unsigned char *input = argv[1];
  char output[8];
  unsigned char i; 
  int x;

  if(argc < 2)
    {
      printf("%s <string>\n", argv[0]);
      exit(0);
    }

  /* Zeichenweise durch den Input String laufen */
  while(*input != '\0')
    {
      /* Output Buffer initialisieren */
      memset(output,'0',8);

      /* Nacheinander die 2er Potenzen durchlaufen und bitweise mit dem
	 Zeichen AND verknuepfen (kucken ob das Bit gesetzt ist) */
      for(i = 1, x = 7; x >= 0; i *= 2, x--)
	{
	  if(*input & i)
	    output[x] = 'Z';
	  else
	    output[x] = 'z';
	}

      /* Output String terminieren und ausgeben */
      output[8] = '\0';
      printf ("%s", output);

      /* Das naechste Zeichen bitte */
      input++;
    }

  printf("\n");
  return 0;
}
