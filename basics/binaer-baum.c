/*
Ein Beispiel fuer einen Binaer Baum

By Bastian Ballmann
24.07.2003

Das loeschen von Eintraegen funktioniert noch nicht!
*/

// Includes 
#include <stdio.h>  // I/O Funktionen wie printf() und fopen()
#include <stdlib.h> // Standardfunktionen wie exit() und malloc()


// Einfacher eigener Datentyp node
typedef struct node {
  char *name;
  struct node *parent;
  struct node *left;
  struct node *right;
} node;


// Funktionsdefinitionen
node *create_node(void);
int insert_node(char *s);
int delete_node(char *s);
int show_tree(node *knoten);


// Globale Variablen
node *anfang;


// Main Part
int main(void)
{
  // Fuege ein paar Knoten hinzu
  insert_node("bar");
  insert_node("bla");
  insert_node("fnord");
  insert_node("blubb");
  insert_node("foo");
  insert_node("ccc");
  insert_node("thc");

  // Loesche einen Knoten
  delete_node("bla");

  // Gib den Baum aus
  show_tree(anfang);
  return 0;
}


// Erstelle einen neuen Knoten (Speicher reservieren + Anfangswerte setzen)
node *create_node(void)
{
  // Reserviere Speicher fuer einen neuen Knoten
  node *knoten = malloc(sizeof(struct node));

  // Startwerte setzen
  knoten->name = NULL;
  knoten->parent = NULL;
  knoten->left = NULL;
  knoten->right = NULL;
  return knoten;
}


// Fuege einen neuen Knoten in den Baum ein
int insert_node(char *s)
{
  node *knoten, *current, *last;
  int richtung = 0;
  int vergleich = 0;

  // Neuen Knoten erstellen
  if( (knoten = create_node()) == NULL) 
    { 
      printf("Duh! We ran out of memory! :-(\n");
      exit(1);
    }

  // Speicher fuer den String allozieren
  knoten->name = malloc(sizeof(*s));
  strcpy(knoten->name,s);

  // Hat der Baum schon eine Wurzel?
  if(anfang == NULL)
    {
      anfang = knoten;
    }
  else
    {
      // Hangel Dich vom Anfang durch den Baum...
      current = anfang;

      // ...und suche die passende Speicherstelle
      while(current != NULL)
	{
	  // Merke Dir den letzten Ast
	  last = current;

	  // Vergleiche die Strings
	  vergleich = strcmp(knoten->name,current->name);

	  // Wenn der einzufuegende String "kleiner" ist,
	  // dann gehts links lang
	  if(vergleich < 0)
	    {
	      richtung = 0;
	      current = current->left;
	    }

	  // Wenn er groesser ist, gehts rechts lang
	  else if(vergleich > 0)
	    {
	      richtung = 1;
	      current = current->right;
	    }

	  // Diesen String gibt es schon im Baum
	  else
	    {
	      printf("insert_node(): String %s exists!\n",s);
	      return 1;
	    }
	}

      // Vater des einzufuegenden Knotens setzen
      knoten->parent = last;

      // Ist der neue Knoten links oder rechts vom Vater?
      if(richtung == 0)
	{
	  last->left = knoten;
	}
      else
	{
	  last->right = knoten;
	}
    }

  return 0;
}


// Loesche einen Knoten aus dem Baum
int delete_node(char *s)
{
  node *current, *parent, *new;
  int vergleich = 0;
  int richtung = 0;

  // Suche den zu loeschenden Knoten
  current = anfang;

  while(current != NULL)
    {
      // Vergleiche die Strings
      vergleich = strcmp(s,current->name);

      // Wenn der String "kleiner" ist, bieg links ab
      if(vergleich < 0)
	{
	  richtung = 0;
	  current = current->left;
	}

      // Ist der String "groesser", bieg rechts ab
      else if(vergleich > 0)
	{
	  richtung = 1;
	  current = current->right;
	}

      // Wir haben den passenden Knoten gefunden
      else
	{
	  break;
	}
    }

  // Keinen passenden Eintrag gefunden?
    if(current == NULL)
    {
      printf("delete_node(): Cant find node %s!\n",s);
      return 1;
    }


  // Loeschen wir gerade das Wurzel Element?
  if(current == anfang)
    {
      // Ist das Wurzel Element das letzte Element des Baums?
      if( (current->left == NULL) && (current->right == NULL) )
	{
	  anfang = NULL;
	}
      else
	{
	  // Gibt es einen linken Knoten?
	  if(current->left != NULL)
	    {
	      anfang = current->left;
	    }

	  // Ansonsten wird der rechte genommen
	  else
	    {
	      anfang = current->right;
	    }
	}
    }
  
  // Es wird nicht das Wurzel Element geloescht
  else
    {
      // Hat der zu loeschende Knoten noch Kindknoten?
      if( (current->left != NULL) || (current->right != NULL) )
	{
	  // Falls es einen linken Knoten gibt, nimmt dieser
	  // den Platz des zu loeschenden ein
	  if(current->left != NULL)
	    {
	      new = current->left;
	    }
	  
	  // Ansonsten wird der rechte genommen
	  else
	    {
	      new = current->right;
	    }
	}
      else
	{
	  new = NULL;
	}

      // An welchem Ast des Vaters haengt der zu loeschende Knoten?
      parent = current->parent;

      if(richtung == 0)
	{
	  parent->left = new;
	}
      else
	{
	  parent->right = new;
	}
    }

  // Speicher des Knotens deallozieren
  free(current);

  return 0;
}


// Hangel Dich durch den Baum und gib die Strings aus
int show_tree(node *knoten)
{
  if(knoten != NULL)
    {
      // String ausgeben
      printf("%s\n",knoten->name);

      // Funktion rekursiv fuer den linken und rechten Ast aufrufen
      show_tree(knoten->left);
      show_tree(knoten->right);
    }

  return 0;
}

