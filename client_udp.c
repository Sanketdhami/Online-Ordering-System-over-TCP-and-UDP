/* UDP client in the internet domain */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void error(const char *);

struct dvd
{
char title[20];
int quantity;
int id;
}a1,a2,a3;

int main(int argc, char *argv[])
{
   int sock, n;
   unsigned int length;
   struct sockaddr_in server, from;
   struct hostent *hp;
   char buffer[1024];
   char buffer1[1024];
   char buffer2[1024];
   if (argc < 3) 
	{ 
	printf("Usage: server port\n");
                    exit(1);
	}
   sock= socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) 
	error("socket");

   server.sin_family = AF_INET;
   hp = gethostbyname(argv[1]);
   if (hp==0) error("Unknown host");

   bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
   server.sin_port = htons(atoi(argv[2]));
   length=sizeof(struct sockaddr_in);
   bzero(buffer,1024);
if(argv[3]==NULL)
{
printf("Connection established");
exit(0); 
} 
strcpy(buffer,argv[3]);
if(strcmp(buffer,"list")==0)
{

//fgets(buffer,255,stdin);

   n=sendto(sock,buffer,
            strlen(buffer),0,(const struct sockaddr *)&server,length);
   if (n < 0) error("Sendto");

 n = recvfrom(sock,buffer,30,0,(struct sockaddr *)&from, &length);
printf("%s\n",buffer);
   
n = recvfrom(sock,&a1,1024,0,(struct sockaddr *)&from, &length);
printf("%d\t\t%s\t%d",a1.id,a1.title,a1.quantity);
    printf("\n");
   n = recvfrom(sock,&a2,1024,0,(struct sockaddr *)&from, &length);
printf("%d\t\t%s\t%d",a2.id,a2.title,a2.quantity);
    printf("\n");
   n = recvfrom(sock,&a3,1024,0,(struct sockaddr *)&from, &length);
printf("%d\t\t%s\t\t%d",a3.id,a3.title,a3.quantity);
    printf("\n");
}
if(strcmp(buffer,"order")==0)
{
if(argv[4]==NULL)
{
printf("Invalid command\n");
exit(0);
}
if(argv[5]==NULL)
{
printf("Invalid command\n");
exit(0);
}
  bzero(buffer,1024);
    bzero(buffer1,1024);
    strcpy(buffer,argv[3]);
    strcpy(buffer1,argv[4]);
    strcpy(buffer2,argv[5]);
    //fgets(buffer,1024,stdin);
    n=sendto(sock,buffer, strlen(buffer),0,(const struct sockaddr *)&server,length);
    n = sendto(sock,buffer1,strlen(buffer1),0,(const struct sockaddr *)&server,length);					
    n = sendto(sock,buffer2,strlen(buffer2),0,(const struct sockaddr *)&server,length);
    n = recvfrom(sock,buffer,10,0,(struct sockaddr *)&from, &length);
    printf("%s",buffer);
bzero(buffer,1024);    
}


   if (n < 0) error("recvfrom");
   //write(1,"Got an ack: ",12);
   //write(1,buffer,n);
   close(sock);
   return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
