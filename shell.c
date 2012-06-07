/*
  A simple chrooted shell

  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  Builtin commands:
  cd - change dir
  echo - print a string
  exit - exit the shell
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

  Programmed by Bastian Ballmann 
  Web: http://www.geektown.de
  Mail: Crazydj@chaostal.de
  Last update: 04.12.2003
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define MAXLINE 1024
#define ROOTDIR "/usr/local/knast"

int main(void)
{
  char buf[MAXLINE], *args[256], *dir, *cmd, *tmp, *delimiter = " ";
  pid_t pid;
  int i = 1, status = -1;
  int pipes[2];

  chdir(ROOTDIR);
  chroot(ROOTDIR);

  while(1)
    {
      // Init
      i = 1;
      memset(args,0,256);

      // Prompt
      printf("$ ");

      // Read command
      fgets(buf,MAXLINE,stdin);
      buf[strlen(buf)-1] = 0;

      // Split the command from the parameter
      args[0] = strtok(buf,delimiter); 
      args[1] = NULL;
      tmp = strtok(NULL,delimiter); 

      while(tmp != NULL)
	{
	  args[i] = tmp;
	  tmp = strtok(NULL,delimiter);
	  i++;
	}

      args[i] = NULL;

      // Change directory
      if(!strcmp(args[0],"cd"))
	{
	  if(args[1] != NULL)
	    {
	      dir = args[1];
	    }
	  else
	    {
	      dir = getenv("HOME");
	    }

	  if(chdir(dir) < 0)
	    {
	      printf("Cannot chdir %s\n",dir);
	    }
	}

      // Echo a string
      else if((!strcmp(args[0],"echo")) && (args[1] != NULL))
	{
	  printf("%s\n",args[1]);
	}

      // Exit the shell
      else if((!strcmp(args[0],"exit")) || (!strcmp(args[0],"logout")))
	{
	  exit(0);
	}

      // Execute a program
      else
	{
	  // Fork a child process
	  if( (pid = fork()) < 0)
	    {
	      perror("fork");
	      exit(1);
	    }
	  else
	    {
	      if(pid == 0)
		{
		  // Child executes command
		  if((execvp(args[0],args)) < 0)
		    {
		      perror("");
		    }

		  exit(0);
		}
	      else
		{
		  // Father waits for child to exit
		  if( (pid = waitpid(pid, &status, 0)) < 0)
		    {
		      perror("waitpid");
		      exit(1);
		    }
		}
	    }
	}
    }

  return 0;
}
