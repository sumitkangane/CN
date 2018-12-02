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
#define sourcePORT 4950
#define MAXBUFLEN 50


char *fname,ack[2]="0";
int len,sockfd1,sockfd2;





void sendtoclient(char *client2_ip)
{
	int cnt=0,fp2,addr_len2,numbytes;
	struct sockaddr_in dest_addr;
	struct hostent *he2;
	char *buff;
	buff=(char *)malloc(sizeof(buff)*11);
	addr_len2=sizeof(struct sockaddr);
	if ((he2 = gethostbyname(client2_ip)) == NULL)
		exit(1);
	

	if((sockfd2=socket(AF_INET,SOCK_DGRAM,0))==-1)
		exit(1);

 	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(sourcePORT);
	dest_addr.sin_addr = *((struct in_addr *)he2->h_addr);
	memset(&(dest_addr.sin_zero), '\0', 8);

	if((numbytes = sendto(sockfd2,fname, strlen(fname), 0, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr))) == -1)
		exit(1);
	else
	{
		printf("%s tranfer starts...",fname);
		fp2=open(fname,O_RDONLY,0600);
		bzero(fname,strlen(fname));

		if (fp2!=-1)
		{
			while(read(fp2,buff,sizeof(buff)))
			{

				len=strlen(buff);
				strcat(buff,"$");


				sendto(sockfd2, buff, strlen(buff), 0, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));
				recvfrom(sockfd2,ack,strlen(ack),0,(struct sockaddr *)&dest_addr,&addr_len2);
				while(ack[0]!='1')
				{
					printf("\nPacket_no:%d Success:%c",cnt,ack[0]);
					strcat(buff,"$");
					sendto(sockfd2, buff, strlen(buff), 0, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));
					recvfrom(sockfd2,ack,strlen(ack),0,(struct sockaddr *)&dest_addr,&addr_len2);
				}
				printf("\nPacket_no:%d Success:%c",cnt,ack[0]);
				cnt=cnt+1;
				bzero(buff,strlen(buff));
			}
			sendto(sockfd2,"EXIT$", strlen("EXIT$"), 0, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));
		}
		else
		{
			printf("\n File not exixts !!!\n");
			exit(0);
		}
	}
	close(fp2);

}

char *receivefrmclient()
{
	struct sockaddr_in source_addr;
	struct sockaddr_in their_addr;		
	char *client2_ip,*buf,*c;
	int j,cnt=0,fp1,addr_len1,numbytes;
	addr_len1 = sizeof(struct sockaddr);
	fname=(char *)malloc(sizeof(char)*20);	
	c=(char *)malloc(sizeof(char)*10);
	buf=(char *)malloc(sizeof(char)*11);	
	client2_ip=(char *)malloc(sizeof(char)*20);
	if((sockfd1 = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		exit(1);

	source_addr.sin_family = AF_INET;
	source_addr.sin_port = htons(sourcePORT);
	source_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(source_addr.sin_zero), '\0', 8);

	if(bind(sockfd1, (struct sockaddr *)&source_addr, sizeof(struct sockaddr)) == -1)
		exit(1);
	if(recvfrom(sockfd1,client2_ip,MAXBUFLEN-1, 0, (struct sockaddr *)&their_addr, &addr_len1)>-1)
		printf("%s",client2_ip);
	else 
		printf("******************");
	if((numbytes = recvfrom(sockfd1, fname, MAXBUFLEN-1, 0, (struct sockaddr *)&their_addr, &addr_len1)) == -1)
	{
		perror("error while receiving message...");    
		exit(1);
	}
	else
		printf("message received successfully....\n");

	printf("message contains \"%s\"\n", fname);


	fp1=open(fname,O_WRONLY|O_CREAT,0600);
	while(recvfrom(sockfd1, buf, MAXBUFLEN-1, 0, (struct sockaddr *)&their_addr, &addr_len1))
	{
		if(strcmp(buf,"EXIT$")==0)
		{
			printf("\n\nThe file has been transferred\n");
			close(fp1);
				
			return (char *)client2_ip;
		}
		else
		{
			len=strlen(buf);	
			while(buf[len-1]!='$')
			{
				strcpy(ack,"0");
				printf("\nPacket No:%d \t received..%c",cnt,ack[0]);
				sendto(sockfd1,ack,strlen(ack),0,(struct sockaddr *)&their_addr,sizeof(struct sockaddr)); 			
				recvfrom(sockfd1, buf, MAXBUFLEN-1, 0, (struct sockaddr *)&their_addr, &addr_len1);
				len=strlen(buf);
			}
			strcpy(ack,"1");
			sendto(sockfd1,ack,strlen(ack),0,(struct sockaddr *)&their_addr,sizeof(struct sockaddr)); 
			for(j=0;buf[j]!='$';j++) c[j]=buf[j];

			write(fp1,c,strlen(c));
			printf("\nPacket No:%d \t received..%c",cnt,ack[0]);
			cnt++;
			bzero(c,sizeof(c));
			bzero(buf,sizeof(buf));
		}
	}

}

int main(int argc, char *argv[ ])
{	
	char *cl2_ip;
	cl2_ip=(char *)malloc(sizeof(char)*20);
	cl2_ip=receivefrmclient();
	sendtoclient(cl2_ip);
	if(close(sockfd1)>-1)
		printf("\nFile receive successfully!\n");
	if(close(sockfd2)>-1)
		printf("\nFile send successfully!\n");
	return 0;

}
