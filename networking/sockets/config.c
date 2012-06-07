/* CVS History 
   Programmed by Bastian Ballmann [ ballmann@co-de.de ]
   @ Computational Design GmbH [ http://www.co-de.de ]
*/

#include "config.h"




// Reserviere Speicher und lege einen neuen Pointer auf eine
// config Struktur an
config* makeNewConfig(void)
{
  config *newPtr = malloc ( sizeof(struct config) );

  if ( newPtr != NULL ) {
    newPtr->key[0] = '\0';
    newPtr->value[0] = '\0';
  } else {
    printf ("\n>>> FATAL: Out of memory in makeNewConfig()\n");
    exit(1);
  }
  
  return newPtr;
}




// Erstelle aus der Configdatei eine verkettete Liste  
int insertConfig(config* new)
{
  if(confanfang == NULL)
    {
      confanfang = new;
      confanfang->next = NULL;
      confende = new;
      confende->next = NULL;
    }
  else
    {
      confende->next = new;
      confende = new;
      confende->next = NULL;
    }
    
  return 1;
}





// Einlesen der Config Datei
int read_config(char* file)
{
  FILE* fh;
  char c;
  char string[256];
  config *confentry;
  int div = 0;
  int count = 0;
  int comment = 0;


  // Lese die Config Datei ein
  fh = fopen(file,"r");

  if(fh != NULL)
    {
      // Erstelle Root Element der verketteten Liste
      confentry = makeNewConfig();

      // Lese die Datei zeichenweise ein
      while( (c=getc(fh)) != EOF)
	{
	  // Wenn wir am Zeilenende sind, hänge die geparsten Daten in
	  // die verkettete Liste
	  if(c == '\n')
	    {
	      if(comment == 0)
		{
		  string[count++] = '\0';
		  strcpy(confentry->value,string);
		  insertConfig(confentry);
		  confentry = makeNewConfig();
		}
	      else
		{
		  comment = 0;
		}

	      count = 0;
	    }


	  // Trennzeichen gefunden?
	  else if(c == '=')
	    {
	      string[count++] = '\0';
	      strcpy(confentry->key,string);
	      bzero(string,256);
	      div = 1;
	      count = 0;
	      continue;
	    }


	  // Leerzeichen auslassen
	  else if(c == ' ')
	    {
	      continue;
	    }


	  // Kommentarzeilen (#) auslassen
	  else if(c == '#')
	    {
	      comment = 1;
	      continue;
	    }

	  // Ansonsten Zeichen einlesen
	  else
	    {
	      if( (div == 1) && (comment == 0) )
		{
		  string[count] = c;
		}
	      else
		{
		  if(comment == 0)
		    {
		      string[count] = c;
		    }
		}

	      count++;
	    }
	}
    }

  return 1;
}



// Suche Configurations Parameter in der verketteten Liste und gebe
// den Pointer auf den gefundenen Eintrag zurück. Bei Fehler NULL.
config* search_config(char search[])
{
  config *current;
  config *found = NULL;

  // Gibt es überhaupt schon ein Root Element der verketteten Liste?
  if(confanfang != NULL)
    {
      current = confanfang;

      // Wühle Dich durch die Liste
      while(current != NULL)
	{
	  // Wurde der Parameter gefunden?
	  if(!strcmp(current->key,search))
	    {
	      found = current;
	      break;
	    }
	  else
	    {
	      current = current->next;
	    }
	}
    }

  return found;
}
      
