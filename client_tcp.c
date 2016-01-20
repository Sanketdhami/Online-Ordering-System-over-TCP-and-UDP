#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

struct dvd
{
char title[20];
int quantity;
int id;
}a1,a2,a3;


int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[1024];
    char buffer1[1024];
    char buffer2[1024];
    if (argc < 3) 
    {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    
portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }
    server = gethostbyname(argv[1]);
    if (server == NULL) 
    {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
if (argv[3]==NULL)
{
printf("Invalid command entered");
exit(0);
}
if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
          error("ERROR connecting");

//check for list command
if (strcmp(argv[3],"list")==0)
    { 
    bzero(buffer,1024);
    strcpy(buffer,argv[3]);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)
         error("ERROR writing to socket");
    bzero(buffer,1024);
    recv(sockfd,buffer,30,0);
    printf("%s",buffer);
    n = recv(sockfd,&a1,1024,0);
    printf("%d\t\t%s\t%d",a1.id,a1.title,a1.quantity);
    printf("\n");
    n = recv(sockfd,&a2,1024,0);
    printf("%d\t\t%s\t%d",a2.id,a2.title,a2.quantity);
    printf("\n");
    n = recv(sockfd,&a3,1024,0);
    printf("%d\t\t%s\t\t%d",a3.id,a3.title,a3.quantity);
    printf("\n");
	exit(0);
}
        if (n < 0)
	{
            error("ERROR reading from socket");
	    exit(0);
	}
	
//check for order command 
else if(argc==6)
{
	if((strcmp(argv[3],"order")==0))
	{
         
            bzero(buffer,1024);
            bzero(buffer1,1024);
	    bzero(buffer2,1024);
            strcpy(buffer,argv[3]);
            strcpy(buffer1,argv[4]);
            strcpy(buffer2,argv[5]);
		//bzero(buffer,1024);
            n = write(sockfd,buffer,strlen(buffer));
	
            n = write(sockfd,buffer1,strlen(buffer1));
		
            n = write(sockfd,buffer2,strlen(buffer2));
	bzero(buffer,1024); 
	bzero(buffer,1024);           
		n = recv(sockfd,buffer,1024,0);
            printf("%s",buffer);
	}
}

close(sockfd);
return 0;
}
