/*
Playing around with the pcap library
Programmed by Bastian Ballmann
bytebeater@crazydj.de
http://www.crazydj.de
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>
#include <net/bpf.h>
#include <unistd.h>
#include <libnet.h>

void decode_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *pkt);

int main(int argc, char *argv[])
{
  // error buffer
  char errbuf[PCAP_ERRBUF_SIZE];

  // Device
  char *device;
  
  // Pcap expression string
  char *pcap_expr;

  // Compiled pcap expression
  struct bpf_program filter;
  
  // Pcap handle
  pcap_t *handle;

  // Netmask / ip
  bpf_u_int32 net,mask;

  // Check return values
  int check;

  // EUID
  uid_t user;

  // Get users EUID
  user = geteuid();

  // Are you root?
  if(user != 0)
    {
      printf("You must be r00t!\nYa EUID is %d\n",user);
      exit(0);
    }

  // Pcap expression
  pcap_expr = malloc(256);

  if(argc > 1)
    {
      // Buffer Overflow?
      if(strlen(argv[1]) > 256)
	{
	  printf("Pcap expression is too long!\n");
	  exit(0);
	}

      strcpy(pcap_expr,argv[1]);
    }
  else
    {
      strcpy(pcap_expr,"tcp\0");
    }

  printf("Lookup device.\n");

  // Lookup the device
  device = pcap_lookupdev(errbuf);

  printf("Open device %s.\n",device);

  // Open device for sniffing in promisc mode
  // device, snaplen, promics-mode, timeout, errorbuffer
  handle = pcap_open_live(device,BUFSIZ,1,-1,errbuf);
  
  printf("Lookup network mask and ip of device %s.\n",device);

  // Lookup IP and network mask for device
  pcap_lookupnet(device, &net, &mask, errbuf);

  printf("Compiling pcap expression.\n");

  // Compile pcap expression
  // pcap_handle, compiled filter, filter, optimized, netmask
  check = pcap_compile(handle,&filter,pcap_expr,0,net);

  printf("Compiled.\n");

  // Failed?
  if(check == -1)
    {
      printf("There was an error while compiling pcap expression!");
      exit(0);
    }

  // Use the compiled pcap filter
  pcap_setfilter(handle,&filter);

  printf("Start sniffing %s on device %s.\n",pcap_expr,device);

  // Start sniffing
  // pcap_handle, num_packets, callback, params
  pcap_loop(handle,-1,decode_packet,NULL);

  return 1;
}


// Decode a sniffed packet
void decode_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *pkt)
{
/* Allgemeines */
  struct libnet_ip_hdr *ip;
  struct libnet_tcp_hdr *tcp;
  struct in_addr dhost, shost;

/* Capturing-Kram */
  unsigned char* data=NULL;
  int tcphlen=0;

  ip=(struct libnet_ip_hdr*)(pkt+14);
  tcp=(struct libnet_tcp_hdr*)(pkt+14+LIBNET_IP_H);
  tcphlen=tcp->th_off*4;  /* Headerlänge kann variieren!! */
  data=(char*)(pkt+14+LIBNET_IP_H+tcphlen);

  shost = ip->ip_src;
  dhost = ip->ip_dst;

  printf("%s:%d --> %s:%d [%d:%d]\nPayload %s\n\n",inet_ntoa(shost),tcp->th_sport,inet_ntoa(dhost),tcp->th_dport,(int)tcp->th_seq,(int)tcp->th_ack,data);
}
