#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include<fcntl.h>
#include<stdbool.h>
#define TIMEOUT 100
#define MYPORT 4950

struct timeval read_timeout;


int main(int argc,char *argv[])
{
  int fp;
  fp=open("ip_address1",O_WRONLY|O_CREAT,0600);
  read_timeout.tv_sec=0;
  read_timeout.tv_usec=1;
  
  int sockfd,addr_len,i,numbytes;
  char *ip_add,*j;
  char ack[2];
  j=(char *)malloc(sizeof(char)*3);
  ip_add=(char *)malloc(sizeof(char)*20);
  struct sockaddr_in dest_addr;
  struct hostent *he;
  static int time=0;
  
  
  addr_len=sizeof(struct sockaddr);

  for(i=0;i<256;i++)
    {

      sprintf(j,"%d",i);
      strcat(ip_add,"192.168.1.");
      strcat(ip_add,j);
      // printf("\n%s",ip_add);
      if ((he = gethostbyname(ip_add)) == NULL)
		exit(1);
      if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
	  exit(1);
	}
      //setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,&read_timeout,sizeof read_timeout);
      int flags=fcntl(sockfd,F_GETFL);
      flags|=O_NONBLOCK;
      fcntl(sockfd,F_SETFL,flags);

      dest_addr.sin_family = AF_INET;
      dest_addr.sin_port = htons(MYPORT);
      dest_addr.sin_addr = *((struct in_addr *)he->h_addr);
      memset(&(dest_addr.sin_zero), '\0', 8);

      sendto(sockfd,"hello", strlen("hello"), 0, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));
      usleep(10000);

       if((numbytes=recvfrom(sockfd,ack,strlen(ack),0,(struct sockaddr *)&dest_addr,&addr_len))==-1)
	{
		printf("\n%15s   Not Runing",ip_add);
		
	}
	else
	{
		write(fp,ip_add,strlen(ip_add));
		write(fp,"\n",1);
		
		printf("\n%15s   Online",ip_add);
	}
											 
	
      bzero(ip_add,strlen(ip_add));
      
      }
}
 
	
















 
