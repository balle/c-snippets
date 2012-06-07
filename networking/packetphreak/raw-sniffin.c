/*
Sniffen ohne Libpcap
By Bastian Ballmann
16.08.2003
*/

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

// Main Part
int main(void)
{
  int sock, uid;
  int packetsize = sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct tcphdr);
  char packet[packetsize];
  struct ether_header *eth = (struct ether_header *) packet;
  struct iphdr *ip = (struct iphdr  *) (packet + sizeof(struct ether_header));
  struct tcphdr *tcp = (struct tcphdr *) (packet + sizeof(struct ether_header) + sizeof(struct iphdr));

  // Are you root?
  uid = getuid();
  if(uid != 0) { printf("You must have UID 0 instead of %d.\n",uid); exit(1); }

  // Raw Socket oeffnen
  if( (sock = socket(AF_INET,SOCK_PACKET,htons(0x3))) == -1) { perror("socket"); exit(1); }

  // Lese Pakete aus dem Raw Socket und dumpe es
  while(1)
    {
      read(sock,packet,packetsize);
      printf("%s:%d\t --> \t%s:%d \tSeq: %d \tAck: %d\n",inet_ntoa(*(struct in_addr *)&ip->saddr),ntohs(tcp->source),inet_ntoa(*(struct in_addr *)&ip->daddr),ntohs(tcp->dest),ntohl(tcp->seq),ntohl(tcp->ack_seq));
    }

  return 0;
}
