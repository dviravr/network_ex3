#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#define MB 1024 
#define PORT 3000 
#define SA struct sockaddr 

// https://www.binarytides.com/server-client-example-c-sockets-linux/
// https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
// https://idiotdeveloper.com/file-transfer-using-tcp-socket-in-c/

void sendFile(FILE *fp, int sockfd) 
{ 
	char file[MB]; 
	int n; 

	while(fgets(file, MB, fp) != NULL) 
	{
		if (send(sockfd, file, sizeof(file), 0) == -1) 
		{
			perror("Error in sending file.");
			return;
		}
    	bzero(file, MB);
  	}
} 

int main() 
{ 
	int sockfd, connfd; 
	struct sockaddr_in server, cli; 

	// socket create and varification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket failed to create.\n"); 
		return 1; 
	} 
	else
		printf("Socket created.\n"); 
	bzero(&server, sizeof(server)); 

	// assign IP, PORT 
	server.sin_family = AF_INET; 
	server.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	server.sin_port = htons(PORT); 

	// connect the client socket to server socket 
	if (connect(sockfd, (SA*)&server, sizeof(server)) != 0) { 
		printf("failed to connect to the server\n"); 
		return 1; 
	} 
	else
		printf("connected to the server.\n"); 

	char* fileName = "1mb.txt";
	FILE *fp;
	fp = fopen(fileName, "r");

	int i = 0;	
	while (i++ < 5)
	{
		printf("%d\n", i);
		sendFile(fp, sockfd); 
	}
	
	// close the socket 
	close(sockfd); 
} 
