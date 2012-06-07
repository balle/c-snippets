/*
  Playing around with pointers
  Programmed by Bastian Ballmann
  http://www.geektown.de

  NOTE: 

  - & ist der Adressoperator, mit ihm 
  bekommt man die Adresse einer Variablen

  - Ein Array liefert die Anfangsadresse
  zurueck, wenn es ohne Index aufgerufen 
  wird

  - * dereferenziert einen Pointer, d.h.
  man greift auf den Wert zu, auf den der
  Pointer zeigt

  - Adressen auf die ein Pointer zeigt, 
  kann man in- bzw. dekrementieren, um
  den naechsten Wert, auf den der Pointer
  zeigt, zu bekommen.
*/

int main(void)
{
  char text[]="Hallo Welt\0";
  char *zeiger = text;
  char *hallo, *welt;

  // Gib den String aus und vermindere ihn
  // immer um einen Character
  // [H|a|l|l|o| |W|e|l|t|\0]
  //  ^
  //    ^ 
  //      ^ [...]
  while(*zeiger)
    {
      printf("%s\n",zeiger);

      // Ein Character ist ein Byte, sonst muesste
      // man schreiben zeiger += sizeof(foobar);
      zeiger++;
    }

  // Gib den String jetzt rueckwerts aus
  while(zeiger != text)
    {
      printf("%s\n",zeiger);
      zeiger--;
    }
  printf("%s\n\n",zeiger);

  // Zerlege den String am Leerzeichen
  while(*zeiger)
    {
      // Ist der aktuelle Character ein Leerzeichen?
      if(*zeiger == ' ')
	{
	  // Setze hinter das Leerzeichen die String-Ende-Kennung
	  *zeiger = '\0';

	  // Speicher in hallo die Startadresse des Strings
	  // Durch die String-Ende-Kennung wird alles von
	  // Anfang bis zum Leerzeichen ausgegeben
	  hallo = text;

	  // Speicher in Welt alles nach der String-Ende-Kennung
	  zeiger++;
	  welt = zeiger;
	  printf("%s und %s\n",hallo,welt);
	  break;
	}

      zeiger++;
    }

  return 0;
}
