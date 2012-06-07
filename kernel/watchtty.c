/*
  Simple program to read input from /dev/ttysniff
  Programmed by Bastian Ballmann
  http://www.geektown.de
  30.06.2004

  This program is free software; you can redistribute 
  it and/or modify it under the terms of the 
  GNU General Public License version 2 as published 
  by the Free Software Foundation.
  
  This program is distributed in the hope that it will 
  be useful, but WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS 
  FOR A PARTICULAR PURPOSE. 
  See the GNU General Public License for more details. 
*/

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
  char c;
  
  /* open device */
  int fd = open("/dev/ttysniff",O_RDONLY);

  if(fd == -1)
    {
      perror("open /dev/ttyhijack");
      exit(1);
    }

  /* Unbuffered output */
  setvbuf(stdout, NULL, _IONBF, 0);

  /* Read input and write it to stdout */
  while(1) 
    { 
      if(read(fd,&c,1) > 0) 
	putchar(c);
      else
	usleep(1);
    }

  close(fd);
  return 0;
}

// [EOF]
