/*
	Playing around with Bits.
	Programmed by Bastian Ballmann and Stefan Jaeger
	[ ballmann@co-de.de ] and [ jaeger@co-de.de ]

	Last Update: 17.04.2002
*/


// Loading header files
#include <stdio.h>


// Define bit streams

// Basti, Maehler, Jaeger, Keule
#define CODE_GEEKS	0x0000000F

#define CODE_BASTI 	0x00000001
#define CODE_JAEGER	0x00000002
#define CODE_KEULE	0x00000004
#define CODE_MAEHLER	0x00000008

typedef struct {
	unsigned int jaeger : 1;
	unsigned int basti : 1;
	unsigned int keule : 1;
	unsigned int maehler : 1;
	unsigned int temp : 4;
} bitfeld;

typedef enum { AN = 0, AUS = 1 } bool;
bool test;

void print_status(int st) {
	if ( st & CODE_BASTI ) {
		printf ("BASTI ist da !\n");
	}
	if ( st & CODE_JAEGER ) {
		printf ("SJ ist da !\n");
	}
	if ( st & CODE_KEULE ) {
		printf("Keule is da!\n");
	}
	if( st & CODE_MAEHLER ) {
		printf("Neues Meister isch da!\n");
	}
	if ( st & CODE_GEEKS ) {
		printf ("Mind. einer der 4 Geeks ist da\n");
	}	
	if ( (st & CODE_GEEKS) == CODE_GEEKS ) {
		printf ("Alle 4 Geeks sind da\n");
	}	
	

}


int main(void)
{
	
  int wer_ist_da;
  bitfeld bstatus;
  bstatus.jaeger = 0;
  bstatus.basti = 1;
   

  wer_ist_da = CODE_BASTI | CODE_KEULE;

  wer_ist_da |= CODE_JAEGER;

  wer_ist_da |= CODE_MAEHLER;
print_status(wer_ist_da);
  
   printf("\n\nDer Maehler und Jaeger hauen nu ab...\n");
   wer_ist_da &= ~(CODE_MAEHLER | CODE_JAEGER);

print_status(wer_ist_da);

/*
  test = AN;
  printf("TEST AN %d\n",test);
*/
  return 0;
}
