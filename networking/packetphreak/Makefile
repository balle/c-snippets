COMPILER = gcc 
FLAGS = -O2 -Wall -D_BSD_SOURCE -D__BSD_SOURCE -D__FAVOR_BSD -DHAVE_NET_ETHERNET_H -DLIBNET_LIL_ENDIAN
LIBS = -lpcap -lnet

default:
	$(COMPILER) $(FLAGS) test-libnet.c $(LIBS) -o test-libnet 
	$(COMPILER) $(FLAGS) send-tcp.c $(LIBS) -o send-tcp 
	$(COMPILER) $(FLAGS) send-arp.c $(LIBS) -o send-arp 
	$(COMPILER) $(FLAGS) sniffer.c $(LIBS) -o sniffer
