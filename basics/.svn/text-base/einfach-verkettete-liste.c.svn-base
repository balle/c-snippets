/*
Beispiel fuer eine einfach verkettete Liste

[node_1]-->[node_2]-->[node_3]-->[node_n]

By Bastian Ballmann
23.07.2003
*/


// Includes
#include <stdio.h>  // I/O Funktionen wie printf() und fopen()
#include <stdlib.h> // Standardfunktionen wie exit() und malloc()


// Einfacher eigener Datentyp node
typedef struct node {
  char *name;
  struct node *next;
} node;


// Funktionsdefinitionen
node *create_node(void);
int insert_node(char *s);
int delete_node(char *s);
int show_list(void);


// Globale Variablen fuer den Anfang und das Ende der Liste
node *anfang = NULL;
node *ende = NULL;


// Main Part
int main(void)
{
  // Fuege ein paar Knoten hinzu
  insert_node("foo");
  insert_node("bar");
  insert_node("bla");

  // Loesche einen Knoten
  delete_node("bar");

  // Gib die Liste aus
  show_list();

  return 0;
}


// Erstelle einen neuen Knoten (Speicher reservieren + Anfangswerte setzen)
node *create_node(void)
{
  // Reserviere Speicher fuer einen neuen Knoten
  node *knoten = malloc(sizeof(struct node));

  // Startwerte setzen
  knoten->name = NULL;
  knoten->next = NULL;
  return knoten;
}


// Fuege einen neuen Knoten hinzu
int insert_node(char *s)
{
  node *knoten;

  // Neuen Knoten erstellen
  if( (knoten = create_node()) == NULL) 
    { 
      printf("Duh! We ran out of memory! :-(\n");
      exit(1);
    }

  // Speicher fuer den String allozieren
  knoten->name = malloc(sizeof(*s));

  strcpy(knoten->name,s);

  // Gibts schon einen Knoten in der Liste?
  if(anfang == NULL)
    {
      anfang = knoten;
      ende = knoten;
    }
  else
    {
      // Der letzte Knoten muss jetzt auf den neuen zeigen
      ende->next = knoten;

      // Der neue Knoten ist jetzt der letzte
      ende = knoten;
    }
     
  return 0;
}


// Loesche einen Knoten
int delete_node(char *s)
{
  node *current = anfang;
  node *before = NULL;

  // Suche von Anfang bis Ende den zu loeschenden Knoten
  while(current != ende)
    {
      if(!strcmp(current->name,s))
	{
	  break;
	}

      before = current;
      current = current->next;
    }

  // Der vorherige Knoten muss jetzt auf den Nachfolger
  // des zu loeschenden Knotens zeigen
  // Speicher fuer den zu loeschenden Knoten deallozieren
  if(!strcmp(ende->name,s))
    {
      before->next = NULL;
      free(ende);
    }
  else
    {
      before->next = current->next;
      free(current);
    }

  return 0;
}


// Gib die Liste aus
int show_list(void)
{
  node *current;

  // Laufe von Anfang bis Ende durch die Liste
  current = anfang;

  while(current != ende)
    {
      printf("%s\n",current->name);
      current = current->next;
    }

  printf("%s\n",ende->name);
  return 1;
}
