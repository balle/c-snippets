#include <time.h>

int main(void)
{
  struct tm *zeit;
  time_t t = time(0);
  zeit = localtime(&t);
  printf("Time: %d:%d:%d\n",zeit->tm_hour,zeit->tm_min,zeit->tm_sec);
  return 0;
}
