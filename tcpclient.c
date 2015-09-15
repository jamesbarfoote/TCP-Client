// NWEN 243, Victoria University of Wellington. Author, Kris Bubendorfer.

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#define bufsize 1024
int socket(int domain, int type, int protocol);
int connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen);
int read(int sockfd, void *buf, int count);
int write(int sockfd, const void *buf, int count);
int close(int fd);

int main(argc, argv) int argc; char *argv[];{
  int sock, rval;
  struct hostent *host;
  struct sockaddr_in server;  // not a pointer.
  char buf[bufsize];

  if(argc != 4){
    printf("usage:\ntcpclient hostname port string\n\n");
    return(-1);
  }

  // look up hostname (server) using DNS

  if ((host = gethostbyname(argv[1])) == 0) {
    fprintf(stderr, "%s: unknown host\n", argv[1]);
    return(-1);
  }

  // Set up fields for socket to point to host and port

  bcopy(host->h_addr, &server.sin_addr, host->h_length);
  server.sin_family = AF_INET;
  server.sin_port = htons(atoi(argv[2]));

  // Create socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0)
  {
    fprintf(stderr,"ERROR creating socket\n");
  }
  else
  {
    fprintf(stderr,"created socket\n");
  }

  // connect (3-way handshake)
  if(connect(sockfd,  (struct sockaddr *)&server, bufsize) < 0)
  {
    fprintf(stderr,"ERROR connecting\n");
  }
  else
  {
    fprintf(stderr,"connected\n");
  }
  fprintf(stderr,"connect done\n");

  // Copy the arg into buf so we can send it to the server

  strncpy(buf, argv[3], bufsize);

  // Send sentence to server
  printf("Message: %s\n",buf);
  if(write(sockfd, buf, bufsize) < 0)
  {
    fprintf(stderr,"ERROR writing\n");
  }
  else
  {
    fprintf(stderr,"write successful\n");
  }


  // read response from server
  if(read(sockfd, buf, bufsize) < 0)
  {
    fprintf(stderr,"ERROR reading\n");
  }
  else
  {
    fprintf(stderr,"read successful\n");
  }

  // print result to window

  fprintf(stdout,"%s\n", buf);
  close(sockfd);
}
