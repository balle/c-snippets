#define _GNU_SOURCE 1
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
  int uid, euid, i;
  
  uid = getuid();
  euid = geteuid();

  printf("UID: %d\n",uid);
  printf("EUID: %d\n",euid);

  for(i=0; i<strlen(*environ); i++)
    {
      printf("%s\n",environ[i]);
    }

  return 0;
}
