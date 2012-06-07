/*
  Beispielcode zum parsen von Textdateien
  Das Ergebnis wird in einem zweidimensionalen Array gespeichert

  Programmed by Bastian Ballmann [ Crazydj@chaostal.de ]
  28.01.2004
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024
#define DELIMITER ' '

int main(int argc, char *argv[])
{
  FILE *fp;
  char buf[BUFSIZE];
  register char *curr = buf;
  register char *first = curr;
  int index = 0;
  int num_rows = 2;
  char **output = (char **)malloc(num_rows * sizeof(char *));

  // Wurde keine Datei als Parameter angegeben?
  if(argc < 2)
    {
      printf("%s <file>\n",argv[0]);
      exit(0);
    }

  // Versuche die Datei zu oeffnen
  if( (fp = fopen(argv[1],"r")) == NULL)
    {
      perror("open");
      exit(0);
    }

  // Lese die Datei zeilenweise ein
  while(fgets(buf,BUFSIZE,fp) != NULL)
    {
      // curr zeigt auf den aktuellen Charcater
      // first auf ersten Character der kopiert werden soll
      curr = buf;
      first = curr;
      
      // Laufe zeichenweise durch den String
      // Splitten des String anhand eines Trennzeichens kann 
      // man auch mit strtok() loesen
      while(*curr != '\0')
	{
	  // Wurde das Trennzeichen oder Newline gefunden?
	  if( (*curr == DELIMITER) || (*curr == '\n') )
	    {
	      // Ersetze das aktuelle Zeichen mit der Stringende-Kennung
	      *curr = '\0';
	      
	      // String speichern
	      output[index++] = strdup(first);

	      // Die erste Position ist jetzt die aktuelle nach der
	      // Stringende-Kennung
	      first = ++curr;
	      
	      // Falls noetig neuen Speicher allokieren
	      if(index > num_rows)
		{
		  num_rows *= 2;

		  if( (output = (char **)realloc(output,num_rows * sizeof(char *))) == NULL)
		    {
		      perror("realloc");
		      exit(1);
		    }
		}
	    }
	  
	  curr++;
	}
    }

  // Gib den Inhalt des Arrays aus
  for(index = 0; index <= num_rows; index++ )
    {
      if(output[index] != NULL)
	printf("%s\n",output[index]);
      
      free(output[index]);
    }

  free(output);
  return 0;
}
