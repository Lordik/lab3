#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

char message[] = "README.txt";

int recvfile(int sock)
{
  FILE *newfile;
  newfile = fopen("recvfile.txt","w");
  if (!newfile)
    return -1;
  char buf[1024];
  int i = 0;
  while (1){
    int num = recv(sock, buf, 256 , 0);
    if (strlen(buf) < 1)
      break;
    while (i < strlen(buf)){
      putc(buf[i],newfile);
      i++;
    }
    i = 0;
  }
  printf("File recived\n");
  return 0;
}

int main()
{
    char buf[1024];
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(17017); 
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }
    send(sock, message, sizeof(message), 0);
//     recv(sock, buf, 1024, 0);
    if (recvfile(sock) < 0)
    {
      perror("recvfile");
      exit(3);
    }
    printf("%s", buf);
    close(sock);
    return 0;
}