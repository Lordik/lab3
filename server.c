#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>


#define THREADS

int sock;
char buf[1024];

int sendfile(int sock, const char *file)
{
  FILE *fileread;
  fileread = fopen(file, "r");
  if (!fileread) {
    strcpy(buf, "File not found\n");
    send(sock, buf,14, 0 );
    return -1;
  }
  int i;
  while (!feof(fileread)){
    for (i = 0; i < 256 || !feof(fileread) ; i++)
      buf[i] = getc(fileread);
//     fgets(buf, 256, fileread);
    if (send(sock, buf, sizeof(buf) ,0) < 0) {
      perror("send");
      return -2;
    }
  }
  fclose(fileread);
  return 0;
}

static void *recvandsend(void *arg )
{
  while(1) {
    int bytes_read = recv(sock, buf, 1024, 0);
    if(bytes_read <= 0) break;
    if (sendfile(sock, buf) < 0)
    {
      perror("sendfile");
      exit(4);
    }
  }
  close(sock);
  return NULL;
}

int main()
{
    int listener;
    struct sockaddr_in addr;
    int bytes_read;
    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(17017);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }
   
    listen(listener, 1);
#ifdef THREADS 
    pthread_t thread;
#endif    
    
    while(1)
    {
        sock = accept(listener, NULL, NULL);
        if(sock < 0)
        {
            perror("accept");
            exit(3);
        }
#ifdef THREADS 
	if (pthread_create(&thread, NULL, recvandsend, NULL) != 0){
	  return EXIT_FAILURE;
	}
#else 
	switch(fork())
	{
	  case -1:
	    perror("fork");
	    break;
	  case 0:
	    close(listener);
	    recvandsend(sock, buf);
	    _exit(0);
	  default:
	    close(sock);
	}
#endif
    }
    close(listener);
    return 0;
}