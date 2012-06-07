/* 
Beispiel Code fuer nslookup
By Bastian Ballmann
10.08.2003
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
  struct hostent *host;

  if(argc < 2) { printf("%s <host>\n",argv[0]); exit(1); }

  if(inet_addr(argv[1]) < 0)
    {
      host = gethostbyname(argv[1]);
    }
  else
    {
      host = gethostbyaddr(argv[1],4,AF_INET);
    }

  printf("[%s] %s\n",host->h_name,inet_ntoa(*((struct in_addr *) host->h_addr_list[0])));      
  return 0;
}
