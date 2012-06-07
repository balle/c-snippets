/*
  Eine billige Variante vom ls
  Eigentlich nur zum spielen mit system calls

  C0D3D by Bastian Ballmann [ ballmann@co-de.de ]

  Last Update: 17.04.2002
*/


// Standard I/O like printf
#include <stdio.h>

// String commands like strcpy and strlen
#include <string.h>

// Linux / Unix systemcalls
#include <syscall.h>

// Unix file descriptor handling
#include <fcntl.h>

// Dateitypen
#include <sys/types.h>

// Inode Infos
#include <sys/stat.h>

// Directory commands like readdir
#include <dirent.h>

// Zeitfunktionen wie localtime
#include <time.h>



void list(char *name);


int main(int argc, char *argv[])
{
  char current[2] = ".";

  // Wurde kein Directory angegeben?
  if(argc == 1)
    {
      list(current);
    }
  else
    {
      list(argv[1]);
    }

  return 0;
}




// List directory content
void list(char *name)
{
  struct dirent *dp;
  struct DIR *dir;
  struct stat stbuf;
  struct tm *zeit;

  printf("Reading dir %s\n",name);

  if(stat(name,&stbuf) == -1)
    {
      printf("Shit there was an error while trying to stat %s\n",name);
      return;
    }

  // Versuche das Verzeichnis zu oeffnen
  if( (dir = (struct DIR*) opendir(name)) == NULL)
    {
      printf("Shit there was an error while trying to open dir %s\n\n", name);
      exit(0);
    }

  // Lese das Verzeichnis ein
  while( (dp = readdir((DIR*) dir)) != NULL)
    {
      // . und .. auslassen
      if(!strcmp(dp->d_name,".") || !strcmp(dp->d_name,".."))
	{
	  continue;
	}
      else
	{
	  // Eigenschaften der aktuellen Datei einlesen
	  if(stat(dp->d_name,&stbuf) == -1)
	    {
	      printf("Shit there was an error while trying to stat %s\n",dp->d_name);
	      return;
	    }

	  // Unix Zeit in "normale" Zeit umrechnen
	  zeit = localtime(&stbuf.st_ctime);

	  // Zeitausgabe konvertieren
	  zeit->tm_year += 1900;


	  // Datei und Directory Namen ausgeben
	  if(strcmp(name,"."))
	    {
	      printf("file %s%-50s UID %-5d GID %-5d   size %10d byte   ctime %02d.%02d.%d %02d:%02d:%02d\n",name,dp->d_name,(int) stbuf.st_uid,(int) stbuf.st_gid,(int) stbuf.st_size,zeit->tm_mday,zeit->tm_mon,zeit->tm_year,zeit->tm_hour,zeit->tm_min,zeit->tm_sec);
	    }
	  else
	    {
	      printf("file %-50s UID %-5d GID %-5d   size %10d byte   ctime %02d.%02d.%d %02d:%02d:%02d\n",dp->d_name,(int) stbuf.st_uid,(int) stbuf.st_gid,(int) stbuf.st_size,zeit->tm_mday,zeit->tm_mon,zeit->tm_year,zeit->tm_hour,zeit->tm_min,zeit->tm_sec);
	    }
	}
    }

  // Handle schlieﬂen und Speicher frei geben
  closedir((DIR*) dir);
}
      

// ---:[ Thats the end folks
