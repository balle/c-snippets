/* Header File von config.c
   Programmed by Bastian Ballmann [ ballmann@co-de.de ]
   @ Computational Design GmbH [ http://www.co-de.de ]
*/

typedef struct config {
  char key[256];
  char value[256];
  struct config *next;
} config;




// ---:[ Globale Variablen

  config *confanfang = NULL;
  config *confende = NULL;



// ---:[ Funktionsdefinitionen

// Einlesen der Config Datei
int read_config(char* file);

// Suche Configurations Parameter in der verketteten Liste und gebe
// den Pointer auf den gefundenen Eintrag zurück. Bei Fehler NULL.
config* search_config(char search[]);

// Speicherplatz für eine Config Struktur anlegen
config* makeNewConfig(void);

// Erstelle aus der Configdatei eine verkettete Liste  
int insertConfig(config* new);


