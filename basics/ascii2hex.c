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
      printf ("\%\%%x", *input);

      /* Das naechste Zeichen bitte */
      input++;
    }

  return 0;
}
