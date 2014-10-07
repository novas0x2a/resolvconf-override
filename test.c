#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define die(...) do {\
  fprintf(stderr, __VA_ARGS__);\
  exit(1);\
} while(0)


int main(int argc, char *argv[])
{
  size_t i;

  const char *preload = getenv("LD_PRELOAD");
  if (preload == NULL)
    die("LD_PRELOAD not set\n");

  int ret = setenv("OVERRIDE_HOSTNAME_google.com", "1.2.3.4", 1);
  if (ret < 0)
    die("Failed to setenv\n");

  const char* check = getenv("OVERRIDE_HOSTNAME_google.com");
  if (check == NULL)
    die("getenv did not return value\n");

  if (strcmp(check, "1.2.3.4") != 0)
    die("getenv did not return correct value\n");

  struct hostent *host = gethostbyname("google.com");
  if (host == NULL)
    die("gethostbyname failed\n");

  if (host->h_addr == NULL)
    die("No addresses associated with host\n");

  printf("google.com resolved to\n");

  struct in_addr** addr_list = (struct in_addr **)host->h_addr_list;
  for(i = 0; addr_list[i] != NULL; i++)
    printf("   %s\n", inet_ntoa(*addr_list[i]));


  if (strcmp(host->h_addr, "1.2.3.4") != 0)
    die("Wrong addr associated with host\n");

  return 0;
}
