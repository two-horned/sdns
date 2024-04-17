#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<strings.h>

#define BUFLEN 512

struct header {
  short id;     // id
  short ms;     // misc
  short qc;     // question     count
  short ac;     // answer       count
  short nc;     // authoritized count
  short dc;     // additional   count
};

struct question {
  short name;    // name representated with labels
  short type;    // type of requested records
  short class;   // class of wanted address
};

// resource record
struct resource {
  short name;    // see question
  short type;    // type code of resource record
  short class;   // class of wanted address
  short ttl;     // time to live in seconds
  short length;  // length of following data
  short data;    // desired data
};

struct dns {
  struct header    header;
  struct question  question;
  struct resource *answer; 
  struct resource *authority;
  struct resource *additional; 
};

static int create_socket(void);

short
get_id(void *buffer)
{
  short *blocks;
  blocks = buffer;

  return blocks[0];
}

int
main (int argc, char *argv[])
{
  int udp_socket;
  struct sockaddr_in addr;

	udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (udp_socket < 0) {
    perror("Could not create socket");
    return 1;
  }
  puts("Socket created");

  addr.sin_family       = AF_INET;
  addr.sin_port         = htons(53);
  addr.sin_addr.s_addr  = inet_addr("127.0.0.1");
  if (bind(udp_socket, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
    perror("Could not bind socket");
    return 1;
  }
  puts("Socket bound");

  unsigned char buffer[BUFLEN];
  for (;;) {
    bzero(buffer, sizeof(buffer));
    if (recv(udp_socket, buffer, BUFLEN, 0) < 0) {
      perror("Could not receive properly");
      close(udp_socket);
      return 1;
    }

    puts("Message received");
    short id;
    id = get_id(buffer);

    printf("id: %d\n", id);
  }

  close(udp_socket);
}
