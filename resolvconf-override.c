// gcc -Wall -g -shared -fPIC -ldl -o libtest.so test.c

#include <resolv.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <assert.h>

#define __USE_GNU
#include <dlfcn.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* For debugging */
#if 0
static void
print_ns (void)
{
  int i;
  for(i=0;i<_res.nscount;i++) {
    char buf[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &_res.nsaddr_list[i].sin_addr, buf, sizeof(buf));
    fprintf(stderr, "dnssrvr: %s\n", buf);
  }
}
#endif

static void
override_ns (void)
{
  int i;
  int valid_ns = 0;

  for (i = 0; i < MAXNS; i++) {
    char envvar[] = "NAMESERVERx";
    const char *ns;

    /* Rename to NAMESERVERx where x is the name server number,
     * and the first one is NAMESERVER1 */
    envvar[10] = '1' + i;

    ns = getenv (envvar);
    if (ns == NULL)
      break;
    if (inet_pton (AF_INET, ns, &_res.nsaddr_list[i].sin_addr) < 1) {
      fprintf (stderr, "Ignoring invalid nameserver '%s'\n", ns);
      continue;
    }

    valid_ns++;
  }

  /* Set the number of valid nameservers */
  if (valid_ns > 0)
    _res.nscount = valid_ns;
}

#define OVERRIDE "OVERRIDE_HOSTNAME_"
#define OVERRIDE_LEN 18
#define MAX_HOSTNAME_LEN 255
#define MAX_ADDR_LEN 15

static struct hostent *host = NULL;
//static char hostbuf[8*1024];
//static char *host_aliases[1] = {NULL};
//static char *h_addr_ptrs[2];
//static char addr[MAX_ADDR_LEN];

struct hostent* (*oldgethostbyname)(const char *) = NULL;

struct hostent *gethostbyname(const char *name)
{
  struct in_addr **addr_list;
  size_t i;
  char envvar[OVERRIDE_LEN + MAX_HOSTNAME_LEN + 1] = OVERRIDE;

  oldgethostbyname = dlsym(RTLD_NEXT, "gethostbyname");

  strncat(envvar, name, MAX_HOSTNAME_LEN);

  const char *addr = getenv(envvar);
  if (addr != NULL) {
    host = oldgethostbyname("pistoncloud.com");
    printf("host is %p\n", host);
    assert(host != NULL);
    for( i = 0; addr_list[i] != NULL; i++) {
      inet_aton(addr, addr_list[i]);
    }
    //strncpy(hostbuf, name, MAXDNAME);
    //hostbuf[MAXDNAME] = '\0';

    //host.h_name     = hostbuf;
    //host.h_aliases  = host_aliases;
    //host_aliases[0] = NULL;
    //host.h_addrtype = AF_INET;
    //host.h_length   = INADDRSZ;
    //strncpy(h_addr_ptrs[0], addr, MAX_ADDR_LEN);
    //h_addr_ptrs[1]  = NULL;
    //host.h_addr_list = h_addr_ptrs;
    //return &host;
  }

  struct hostent * (*f)() = dlsym (RTLD_NEXT, "gethostbyname");
  struct hostent *ret =  f(name);

  return ret;
}

//int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)
//{
//  strict addrinfo
//  int (*f)() = dlsym (RTLD_NEXT, "getaddrinfo");
//  return f(node, service, hints, res);
//}
