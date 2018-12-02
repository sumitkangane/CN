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
#define MYPORT 4950
#define server_ip "192.168.1.213"

int main(int argc, char *argv[ ])
{
  int sockfd,addr_len,len,cnt=0,init=0;
  char *buff,fname[10],*ip_add;
  ip_add=(char *)malloc(sizeof(char)*15);
  char ack[2];
  buff=(char *)malloc(sizeof(char)*10);
	int fp;
	
	struct sockaddr_in dest_addr;
	struct hostent *he;
	int numbytes;
	
	addr_len=sizeof(struct sockaddr);
	if (argc != 3)
	{
	  printf("Filename is not given");
		exit(1);
	}
	
	
        if(strcmp(argv[1],"192.168.1.202")==0) //Check with IP address of self
	  {
	    printf("It's your machine IP address");
	    printf("\nWe did not allowed to send file to self");
	    return -1;
	  }
	if ((he = gethostbyname(server_ip)) == NULL)
		exit(1);
	
	strcpy(fname,argv[2]);
	
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
	  exit(1);
	}
	
	
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(MYPORT);
	dest_addr.sin_addr = *((struct in_addr *)he->h_addr);
	memset(&(dest_addr.sin_zero), '\0', 8);


	sendto(sockfd, argv[1], strlen(argv[1]), 0, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));
	
	if((numbytes = sendto(sockfd, argv[2], strlen(argv[2]), 0, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr))) == -1)
	  {
	    exit(-1);
	  }
	else
	  {
	        printf("%s tranfer starts...",fname);
		fp=open(fname,O_RDONLY,0600);
		bzero(fname,strlen(fname));
		
	if (fp!=-1) //check wheather file name is exist or not
		  { 
		     while(read(fp,buff,sizeof(buff)))
		      {
		 
			len=strlen(buff);
			/*
			if(init==0 && cnt==15)
			{
			  strcat(buff,"#"); //give error bit
				init=1;
				}
			else
			{*/
				strcat(buff,"$");
				//}
			
			sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));
			recvfrom(sockfd,ack,strlen(ack),0,(struct sockaddr *)&dest_addr,&addr_len);
		        while(ack[0]!='1') //if packet is lostx
			  {
			  	printf("\nPacket_no:%d Success:%c",cnt,ack[0]);
			  	strcat(buff,"$");
			    	sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));
				recvfrom(sockfd,ack,strlen(ack),0,(struct sockaddr *)&dest_addr,&addr_len);
			  }
			printf("\nPacket_no:%d Success:%c",cnt,ack[0]);
			cnt=cnt+1;
			bzero(buff,strlen(buff));
		      }
		    	sendto(sockfd,"EXIT$", strlen("EXIT$"), 0, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));
		  }
		else
		  {
		    printf("\n File not exixts !!!\n");
		    exit(0);
		  }
	   }
	
	 
	  
	close(fp);
	if(close(sockfd)>-1)
		printf("\nFile send successfully!\n");

	return 0;

}
