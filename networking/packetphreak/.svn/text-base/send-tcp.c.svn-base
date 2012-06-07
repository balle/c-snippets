/*
Playing around with the libnet library.
Send an TCP SYN packet
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


/* MAIN PART */

int main(int argc, char *argv[])
{
  // Raw socket
  int sock;

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

  // Allocate memory for the payload
  payload = malloc(50);

  // Packets payload
  strcpy(payload,"TEST LIBNET\0");

  // payload size
  p_size = strlen(payload);

  // ip address in network byte order
  src_ip = inet_addr("192.168.3.33");
  dst_ip = inet_addr("192.168.3.33");

  // Lookup ip addresses
  src_ip = libnet_name_resolve(libnet_host_lookup(src_ip,0),0);
  dst_ip = libnet_name_resolve(libnet_host_lookup(dst_ip,0),0);

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

	  // Build tcp header
	  libnet_build_tcp(ntohs(7777),                    // source port
			   ntohs(21),                      // destination port
			   1000000000,                     // acknowledge number
			   1000000000,                     // sequence number
			   TH_SYN,                         // tcp flags
			   1024,                           // window size
			   0,                              // urgent pointer
			   payload,                        // pointer to packet payload
			   p_size,                         // payload size
			   packet+LIBNET_IP_H  // pointer to the packet buffer
			   );

	  printf("Sending TCP packet.\n");

      // Calculize checksum
      libnet_do_checksum(packet,IPPROTO_TCP,LIBNET_IP_H+LIBNET_TCP_H+p_size);

      // Get the packet on the wire
      send = libnet_write_ip(sock,packet,LIBNET_IP_H+LIBNET_TCP_H+p_size);

      // was the complete packet send over the wire?
      if(send < LIBNET_IP_H+LIBNET_TCP_H+p_size)
	{
	  printf("error while writing packet into the socket...\n");
	}

      printf("Closing socket.\n");

      // close the socket
      libnet_close_raw_sock(sock);

      // free the memory
      //libnet_destroy_packet(packet);
      free(payload);

      return 1;
}
