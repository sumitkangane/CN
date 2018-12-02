#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<fcntl.h>

#define sourcePORT 4950
#define MAXBUFLEN 50

int main(int argc, char *argv[])
{
	int sockfd,fp,len,cnt=0,z;
	struct sockaddr_in source_addr;
	struct sockaddr_in their_addr;
	int addr_len, numbytes;
	char *buff,*buf,*fname,*c,ack[2]="0";
	fname=(char *)malloc(sizeof(char)*20);
	buff=(char *)malloc(sizeof(char)*20);
	c=(char *)malloc(sizeof(char)*10);
	buf=(char *)malloc(sizeof(char)*11);

		if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		exit(1);

	source_addr.sin_family = AF_INET;
	source_addr.sin_port = htons(sourcePORT);
	source_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(source_addr.sin_zero), '\0', 8);

	if(bind(sockfd, (struct sockaddr *)&source_addr, sizeof(struct sockaddr)) == -1)
		exit(1);
	addr_len = sizeof(struct sockaddr);

	if((numbytes = recvfrom(sockfd, buff, MAXBUFLEN-1, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1)
	{
		perror("wrong.....");    
		exit(1);
	}
	else
	{
		strcpy(ack,"1");
		printf("\nhiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii\n");
		sendto(sockfd,ack,strlen(ack),0,(struct sockaddr *)&their_addr,sizeof(struct sockaddr));			
	}
	
	bzero(buff,sizeof(buff));
	if(close(sockfd) != 0)
		printf("socket closing failed!\n");

	return 0;
}
