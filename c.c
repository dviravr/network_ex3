#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/tcp.h> 
#define MB 1024 * 1024
#define PORT 8080 
#define SA struct sockaddr 

void changeCC(int sock) 
{
	char reno[256] = "reno";
	socklen_t len = strlen(reno);
	if (setsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, reno, len) != 0) {
		perror("setsockopt"); 
		return -1;
	}
	len = sizeof(reno); 
	if (getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, reno, &len) != 0) {
		perror("getsockopt"); 
		return -1; 
	} 
	printf("New CC: %s\n", reno); 
}

void func(FILE *fp, int sockfd) 
{ 
	char buff[MB]; 
	int i = 0;

	for (;;) { 
		bzero(buff, sizeof(buff)); 

		while(fgets(buff, MB, fp) != NULL) 
			; 
		long sent = write(sockfd, buff, sizeof(buff)); 
		bzero(buff, sizeof(buff)); 
		long got = read(sockfd, buff, sizeof(buff)); 
		printf("send to server %ld bytes\n", sent); 
		printf("got from server %ld bytes\n", got); 
		if (i++ >= 4)
        {
            printf("Server Exit...\n");
            break;
        }
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

    char* fileName = "1m.txt";
	FILE *fp;
	fp = fopen(fileName, "r");

	// function for chat 
	func(fp, sockfd);

	changeCC(sockfd);

	func(fp, sockfd);
	// close the socket 
	close(sockfd); 
} 
