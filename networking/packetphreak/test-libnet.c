/*
Playing around with the libnet library.
Programmed by Bastian Ballmann
bytebeater@crazydj.de
http://www.crazydj.de
*/


/* INCLUDES */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <libnet.h>



/* CONFIGURATION */

// source mac
#define SMAC "aa:bb:cc:aa:bb:cc"

// destination mac
# define DMAC "aa:bb:cc:aa:bb:cc"

// source ip
#define SOURCE "192.168.3.33"

// destination ip
#define DEST "192.168.3.32"

// source port
#define SPORT 7777

// destination port
#define DPORT 80



/* MAIN PART */

int main(int argc, char *argv[])
{
  // packet type (arp/udp/tcp)
  char type[5];

  // network device
  char *device = "eth0\0";
  struct libnet_link_int *link;

  // Raw socket
  int sock;
  
  // error buffer
  char errbuff[LIBNET_ERRBUF_SIZE];

  // src and dst mac
  u_char dmac[6],smac[6];

  // src and dst ip
  unsigned int src_ip, dst_ip;

  // packet buffer
  unsigned char *packet;

  // packet payload
  char *payload;

  // payload size
  int p_size;

  // bytes send over the wire
  int send;

  // check parameter
  if(argc == 1)
    {
      printf("Usage: %s <arp/udp/tcp>\n",argv[0]);
      exit(0);
    }
  else
    {
      strcpy(type,argv[1]);
    }
  
  // Allocate memory for the payload
  payload = malloc(50);

  // Packets payload
  strcpy(payload,"TEST LIBNET\0");
  packet = NULL;

  // payload size
  p_size = strlen(payload);

  // mac address
  strcpy(smac,SMAC);
  strcpy(dmac,DMAC);

  // ip address in network byte order
  src_ip = inet_addr(SOURCE);
  dst_ip = inet_addr(DEST);

  // Lookup ip addresses
  src_ip = libnet_name_resolve(libnet_host_lookup(src_ip,0),0);
  dst_ip = libnet_name_resolve(libnet_host_lookup(dst_ip,0),0);

  // Build an ARP packet?
  if(!strcmp(type,"arp"))
    {
      // open the network device
      link = libnet_open_link_interface(device,errbuff);
      
      // Failed?
      if(link == NULL)
	{
	  printf("Error while opening device %s!\n%s\n",device,errbuff);
	  exit(1);
	}

      printf("Using device %s\n",device);
      
      // Allocate memory for the packet
      if(libnet_init_packet(LIBNET_ETH_H+LIBNET_ARP_H+p_size,&packet) == -1)
	{
	  printf("libnet_init_packet error!\n%s\n", strerror(errno));
	  exit(1);
	}

      if(packet == NULL)
	{
	  printf("libnet_init_packet error!\n");
	  exit(1);
	}

      // Build ethernet header
      libnet_build_ethernet(dmac,         // destination mac
			    smac,         // source mac
			    ETHERTYPE_ARP,// ethernet packet type
			    NULL,         // pointer to payload
			    0,            // payload size
			    packet        // pointer to packet buffer
			    );

      printf("Sending ARP reply packet %s --> %s\n",SOURCE,DEST);
      
      libnet_build_arp(ARPHRD_ETHER,       // hardware type
		       ETHERTYPE_IP,       // protocol type
		       ETHER_ADDR_LEN,     // hardware address size
		       4,                  // protocol address size
		       ARPOP_REPLY,        // ARP operation
		       smac,               // source mac
		       (u_char *)&src_ip,   // source ip
		       dmac,               // destination mac
		       (u_char *)&dst_ip,   // destination ip
		       NULL,               // pointer to payload
		       0,                  // payload size
		       packet+LIBNET_ETH_H // pointer to packet buffer
		       );

      // Get the packet on the wire
      send = libnet_write_link_layer(link,device,packet,LIBNET_ETH_H+LIBNET_IP_H+LIBNET_TCP_H+p_size);

      // was the complete packet send over the wire?
      if(send < LIBNET_IP_H+LIBNET_TCP_H+p_size)
	{
	  printf("error while writing packet into the socket...\n");
	}

      // close the network device
      libnet_close_link_interface(link);
    }
  else 
    {
      // Open a raw sock
      sock = libnet_open_raw_sock(IPPROTO_RAW);
      
      // Allocate memory for the packet
      if(libnet_init_packet(LIBNET_IP_H+LIBNET_TCP_H+p_size,&packet) == -1)
	{
	  printf("libnet_init_packet error!\n%s\n", strerror(errno));
	  exit(1);
	}

      // Build ip header
      libnet_build_ip(LIBNET_TCP_H+p_size,// packet length without ip header length
		      0,                  // ip type of service
		      242,                // ip id
		      0,                  // fragmentation bit
		      48,                 // time to live
		      IPPROTO_TCP,        // Transport Control Protokoll
		      src_ip,             // source ip
		      dst_ip,             // destination ip
		      NULL,               // pointer to ip payload
		      0,                  // ip options
		      packet              // pointer to packet buffer
		      );


      // Build UDP packet?
      if(!strcmp(type,"udp"))
	{
	  libnet_build_udp(ntohs(SPORT),                          // Source port
			   ntohs(DPORT),                          // destination port
			   payload,                        // pointer to packet payload
			   p_size,                         // payload size
			   packet+LIBNET_IP_H // pointer to packet buffer
			   );
	  
	  printf("Sending UDP packet %s:%d --> %s.%d\n",SOURCE,SPORT,DEST,DPORT);
	}


      // Build TCP/IP packet per default
      else
	{
	  // Build tcp header
	  libnet_build_tcp(ntohs(SPORT),                   // source port
			   ntohs(DPORT),                   // destination port
			   1000000000,                     // acknowledge number
			   1000000000,                     // sequence number
			   TH_ACK,                         // tcp flags
			   1024,                           // window size
			   0,                              // urgent pointer
			   payload,                        // pointer to packet payload
			   p_size,                         // payload size
			   packet+LIBNET_IP_H // pointer to the packet buffer
			   );

	  printf("Sending TCP packet %s:%d --> %s.%d\n",SOURCE,SPORT,DEST,DPORT);
	}

      // Calculize checksum
      libnet_do_checksum(packet,IPPROTO_TCP,LIBNET_IP_H+LIBNET_TCP_H+p_size);

      // Get the packet on the wire
      send = libnet_write_ip(sock,packet,LIBNET_IP_H+LIBNET_TCP_H+p_size);

      // was the complete packet send over the wire?
      if(send < LIBNET_IP_H+LIBNET_TCP_H+p_size)
	{
	  printf("error while writing packet into the socket...\n");
	}

      // close the socket
      libnet_close_raw_sock(sock);
    }

  // free the memory
  //libnet_destroy_packet(packet);
  free(payload);
  free(errbuff);

  return 1;
}
