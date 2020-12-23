#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#define MB 1024 * 1024
#define PORT 8080 
#define SA struct sockaddr 
void func(FILE *fp, int sockfd) 
{ 
	char buff[MB]; 
	int i = 0;

	for (;;) { 
		bzero(buff, sizeof(buff)); 
		printf("Enter the string : "); 

		while(fgets(buff, MB, fp) != NULL) 
			; 
		write(sockfd, buff, sizeof(buff)); 
		bzero(buff, sizeof(buff)); 
		read(sockfd, buff, sizeof(buff)); 
		printf("got from Server : %lu\n", sizeof(buff)); 
		if (i >= 4)
        {
            printf("Server Exit...\n");
            break;
        }
        i++;
	} 
} 

int main() 
{ 
	int sockfd, connfd; 
	struct sockaddr_in servaddr, cli; 

	// socket create and varification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	servaddr.sin_port = htons(PORT); 

	// connect the client socket to server socket 
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed...\n"); 
		exit(0); 
	} 
	else
		printf("connected to the server..\n"); 

    char* fileName = "1mb.txt";
	FILE *fp;
	fp = fopen(fileName, "r");

	// function for chat 
	func(fp, sockfd); 

	// close the socket 
	close(sockfd); 
} 
