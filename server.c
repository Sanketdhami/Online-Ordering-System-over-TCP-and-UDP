#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>

#define	MAX(x, y)	((x) > (y) ? (x) : (y))

#define	QLEN		 32

#define	LINELEN		128


int	passiveTCP(const char *service, int qlen);
int	passiveUDP(const char *service);

pthread_mutex_t m; 
int count_tcp= 0;
int count_udp = 0;
int sum = 0;
int total_count =0;

int	usock;
struct sockaddr_in serv_addr, cli_addr, from;
socklen_t clilen;

struct thread_data
{
int sock;
char command[1024];
int quant;
int itemno;
} t1;


struct sockaddr_in from;
int usock,fromlen;

struct Dvd
{
char title[20];
int quantity;
int id;
}a1,a2,a3;


/*-----------------TCP function for list and order--------------*/

void order(int newsockfd)
{
	int i,c,d;
	int n;
	char buffer[100];
        char buffer1[1024];
        char buffer2[1024];
        bzero(buffer,1024);
	pthread_mutex_lock(&m);
	count_tcp=count_tcp+1;
	printf("%d TCP clients conected so far\n",count_tcp);
        n = read(newsockfd,buffer,5);
     	if (n < 0) 
	printf("ERROR reading from socket");
	pthread_mutex_unlock(&m);

if(strcmp(buffer,"list")==0)
{
pthread_mutex_lock(&m);
printf("The client has requested for LIST\n");
printf("_____________________________________________\n");

n= send(newsockfd,"Item Number\tTitle\t\tQuantity\n",30,0);
n= send(newsockfd,&a1,1024,0);
n= send(newsockfd,&a2,1024,0);
n= send(newsockfd,&a3,1024,0);

pthread_mutex_unlock(&m);
bzero(buffer,1024);
}
 
if((strcmp(buffer,"order")==0))
{
pthread_mutex_lock(&m);
printf("The client has requested for ORDER\n");    

n = read(newsockfd,buffer1,4);

n = read(newsockfd,buffer2,1024);

n = send(newsockfd,"OK\n",1024,0);
c = atoi(buffer1);
d =atoi(buffer2);
sum = sum +d;
printf("Total DVD's Purchased by TCP and UDP clients: %d\n",sum);
printf("********************************************\n");
    if(a1.id == c)
    {
	a1.quantity = a1.quantity-d;
    }
    else if(a2.id == c)
    {
	a2.quantity = a2.quantity-d;	
    }
    else if(a3.id == c)
    {
	a3.quantity = a3.quantity-d;
    }
bzero(buffer1,1024);
bzero(buffer2,1024);
pthread_mutex_unlock(&m);
}
   if (n < 0) 
	printf("ERROR writing to socket");
  close(newsockfd);
}




/*-----------------UDP function for list and order--------------*/

void *listorder_udp(void *info)
{
struct thread_data *t1 = info;
char comm[1024];
strcpy(comm,t1->command);
int sockfd = t1->sock; 

	char buf1[1024];
	char buf2[1024];
	int n,e,d;
	bzero(buf1,1024);
	bzero(buf2,1024);
	pthread_mutex_lock(&m);
	count_udp=count_udp+1;
	printf("%d UDP clients connected so far\n",count_udp);
	printf("_____________________________________\n");	
	pthread_mutex_unlock(&m);
if((strcmp(comm,"list")==0))
{
bzero(buf1,1024);	
	
pthread_mutex_lock(&m);
n= sendto(sockfd,"Item Number\tTitle\t\tQuantity",30,0,(struct sockaddr *)&from, fromlen);
if(n==-1)
{
printf("Error:%s\n",strerror(errno));
exit(1);
}
       n = sendto(sockfd,&a1,1024,0,(struct sockaddr *)&from,fromlen);

if(n==-1)
{
printf("Error:%s\n",strerror(errno));
exit(1);
}


       n = sendto(sockfd,&a2,1024,
                  0,(struct sockaddr *)&from,fromlen);

if(n==-1)
{
printf("Error:%s\n",strerror(errno));
exit(1);
}
     
  n = sendto(sockfd,&a3,1024,
                  0,(struct sockaddr *)&from,fromlen);

if(n==-1)
{
printf("Error:%s\n",strerror(errno));
exit(1);
}
pthread_mutex_unlock(&m);
}

if((strcmp(comm,"order")==0))
{
int itemnum = t1-> itemno;
int amount = t1-> quant;

if((itemnum>0)&&(amount>0))
n = sendto(sockfd,"OK\n",10,0,(struct sockaddr *)&from, fromlen);
if(n==-1)
{
printf("Error:%s\n",strerror(errno));
exit(1);
}
pthread_mutex_lock(&m);
if(a1.id == itemnum)
{
	a1.quantity = a1.quantity-amount;
}
else if(a2.id == itemnum)
{
	a2.quantity = a2.quantity-amount;	
}
else if(a3.id == itemnum)
{
	a3.quantity = a3.quantity-amount;
}
sum = sum +amount;
printf("Total DVD's Purchased by TCP and UDP clients: %d\n",sum);
printf("_____________________________________________\n");
pthread_mutex_unlock(&m);
}
}

/*------------------------Main function---------------------------*/
int main(int argc, char *argv[])
{
int sockfd, newsockfd, portno;
	struct sockaddr_in serv_addr, cli_addr;
socklen_t clilen;
	int	alen;		/* from-address length		*/
	int	tsock; 			/* TCP master socket		*/
	int usock;				/* UDP socket			*/
	int	nfds;
	fd_set	rfds;			/* readable file descriptors	*/

strcpy(a1.title,"Insidious");
strcpy(a2.title,"Avengers");
strcpy(a3.title,"Ironman");

a1.id = 1000;
a2.id = 1001;
a3.id = 1002;

a1.quantity = 120;
a2.quantity = 100;
a3.quantity = 70;
	
if (argc < 2) 
     {
         printf("ERROR, no port provided\n");
         exit(1);
     }

tsock = socket(AF_INET, SOCK_STREAM, 0);
     if (tsock < 0)
        printf("ERROR opening socket");

usock = socket(AF_INET, SOCK_DGRAM, 0);
     if (usock < 0)
        printf("ERROR opening socket");
     fromlen=sizeof(struct sockaddr_in);
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
     serv_addr.sin_port = htons(portno);
     
    if (bind(tsock, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              printf("ERROR on binding");
	if (bind(usock, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              printf("ERROR on binding");

		/* bit number of max fd	*/
	FD_ZERO(&rfds);

	
listen(tsock,5);
while (1) 
{
nfds = MAX(tsock,usock) + 1;
		FD_SET(usock, &rfds);
		FD_SET(tsock, &rfds);

int sret;
	sret = select(nfds, &rfds, (fd_set *)0, (fd_set *)0,(struct timeval *)0);
if(sret<=0)
{
		printf("select error: %s\n", strerror(errno));
exit(1);
}


		if (FD_ISSET(tsock, &rfds)) 
		{ 
		pthread_t thread;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		pthread_mutex_init(&m, NULL);
			
		clilen = sizeof(cli_addr);

		newsockfd = accept(tsock,(struct sockaddr *) &cli_addr, &clilen); 
		     if (newsockfd < 0)
			  printf("ERROR on accept");
		     else 
			{
			  printf("Server is accepting connection on %s address and port no. %d\n",inet_ntoa(serv_addr.sin_addr),(int)ntohs(serv_addr.sin_port));
			  printf("Client has connected to server on its %s address and port no. %d\n",inet_ntoa(cli_addr.sin_addr),(int)ntohs(cli_addr.sin_port));	
			}
int PID = pthread_create(&thread,&attr,(void*(*)(void*))order,(void *)(long) newsockfd);
		if(PID!=0)
		{
			printf("pthread create:%s\n",strerror(errno));
		}

		pthread_join(thread,NULL);
		pthread_attr_destroy(&attr);
	}


if (FD_ISSET(usock, &rfds))
{
	printf("UDP connection\n");
	char buf[1024];
	char buf1[1024];
	char buf2[1024];
	int n,e,d;
	char c[1024];
	bzero(buf,1024);
	fromlen = sizeof(from);
		pthread_t thread;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		pthread_mutex_init(&m, NULL);
			        
n = recvfrom(usock,buf,1024,0,(struct sockaddr *) &from, &fromlen);

if (n < 0)
{
error("recvfrom");
}
else
{
printf("Server is accepting connection on %s address and port no. %d\n",inet_ntoa(serv_addr.sin_addr),(int)ntohs(serv_addr.sin_port));
printf("Client has connected to server on its %s address and port no. %d\n",inet_ntoa(cli_addr.sin_addr),(int)ntohs(cli_addr.sin_port));
strcpy(c,buf);
printf("The client has requested for %s\n",c);
printf("_____________________________________________\n");	
}

	
       if ((strcmp(c,"list")==0)||(strcmp(c,"order")==0))
       { 
	strcpy(t1.command,c);
	t1.sock = usock;
if((strcmp(c,"order")==0))
{
n = recvfrom(usock,buf1,1024,0,(struct sockaddr *)&from, &fromlen);
e = atoi(buf1);

t1.itemno = e;
n = recvfrom(usock,buf2,1024,0,(struct sockaddr *)&from, &fromlen);
d =atoi(buf2);

t1.quant = d;
}
int PID = pthread_create(&thread,&attr,listorder_udp,(void *)&t1);

		if(PID!=0)
		{
			printf("pthread create:%s\n",strerror(errno));
		}

pthread_join(thread,NULL);
		pthread_attr_destroy(&attr);

	}
}
}
return 0;
}
