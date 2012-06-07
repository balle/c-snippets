/*
Beispiel Code um ein TCP/IP Packet ohne Libnet zu erstellen
und auf die Reise zu schicken.

By Bastian Ballmann
11.08.2003

-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

Man pages zum Thema:

man 2 socket
man 2 setsockopt
man 7 raw
man 7 ip
man 7 tcp

-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

*/

// Includes
#include <stdio.h>         // Standard I/O Funktionen wie printf()
#include <stdlib.h>        // Standard Funktionen wie exit() und malloc()
#include <string.h>        // String und Memory Funktionen wie strcmp() und memset()
#include <unistd.h>        // System Calls wie open(), read() und write()
#include <errno.h>         // Detailliertere Fehlermeldungen
#include <sys/socket.h>    // Socket Funktionen wie socket(), bind() und listen()
#include <arpa/inet.h>     // Funktionen wie inet_addr()
#include <netinet/in.h>    // IP Protokolle, sockaddr_in Struktur und Funktionen wie htons()
#include <netinet/ip.h>    // IP Header Struktur
#include <netinet/tcp.h>   // TCP Header Struktur


// Main part
int main(void)
{
  int rawsock, uid;
  struct sockaddr_in addr;
  unsigned int packetsize = sizeof(struct iphdr) + sizeof(struct tcphdr);
  unsigned char packet[packetsize];
  struct iphdr *ip = (struct iphdr *)packet;
  struct tcphdr *tcp = (struct tcphdr *)(packet + sizeof(struct iphdr));
  int one = 1;

  // Are you root?
  uid = getuid();
  if(uid != 0) { printf("You must have UID 0 instead of %d.\n",uid); exit(1); }

  // Packet Buffer initialisieren
  memset(packet,0,packetsize);

  // Erstelle einen IP RAW Socket Deskriptor
  if( (rawsock = socket(AF_INET,SOCK_RAW,IPPROTO_TCP)) == -1 ) { perror("socket"); exit(1); }

  // IP_HDRINCL muss eingeschaltet sein, um sicher zu stellen, dass uns der Kernel nicht
  // in den Headern rum fummelt
  if( setsockopt(rawsock,IPPROTO_IP,IP_HDRINCL,&one,sizeof(one)) == -1 ) { perror("setsockopt"); exit(1); }

  // IP Header zusammen basteln
  ip->version = 4;                        // IP Version
  ip->ihl = 5;                            // Internet Header Length
  ip->id = htonl(random());               // IP ID
  ip->saddr = inet_addr("127.0.0.1");     // Source IP
  ip->daddr = inet_addr("127.0.0.1");     // Destination IP
  ip->ttl = 123;                          // Time to live
  ip->protocol = IPPROTO_TCP;             // Transport Protokoll TCP (6)
  ip->tot_len = packetsize;               // Groesse des IP Pakets
  ip->check = 0;                          // IP Checksum (Wenn die Checksumme 0 ist, wird sie 
                                          // vom Kernel berechnet)

  // TCP Header zusammen basteln
  tcp->source = htons(1234);              // Source Port
  tcp->dest = htons(23);                  // Destination Port
  tcp->seq = htonl(1000000000);           // Sequence number
  tcp->ack_seq = htonl(1000000000);       // Acknowledgement number
  tcp->ack = 1;                           // TCP Flags
  tcp->syn = 1;
  tcp->window = htons(1024);              // Window size
  tcp->check = 0;                         // TCP Checksum

  // Schicke das Paket auf die Reise  
  addr.sin_family = AF_INET;
  addr.sin_port = tcp->source;
  addr.sin_addr.s_addr = ip->saddr;

  if( (sendto(rawsock,packet,packetsize,0,(struct sockaddr*)&addr,sizeof(struct sockaddr_in))) == -1 )
    {
      perror("send");
      exit(1);
    }
  
  // Speicher fuer Packet Buffer deallozieren
  free(packet);
  return 0;
}
