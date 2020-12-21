#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <time.h> 
#define MB 1024 
#define PORT 3000 
#define SA struct sockaddr 

void getFile(int sockfd){
  int n;
  FILE *fp;
  char *filename = "recv.txt";
  char buffer[MB];
  clock_t t;

  fp = fopen(filename, "w");
  t = clock();
  while (1) {
    n = recv(sockfd, buffer, MB, 0);
    if (n <= 0){
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    bzero(buffer, MB);
  }
  t = clock() - t;
  double time = ((double) t) / CLOCKS_PER_SEC;
  printf("it's took %lf seconds to get the file\n", time);
  return;
}


// Driver function 
int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in server, cli; 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		return 1;
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&server, sizeof(server)); 

	// assign IP, PORT 
	server.sin_family = AF_INET; 
	server.sin_addr.s_addr = htonl(INADDR_ANY); 
	server.sin_port = htons(PORT); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (SA*)&server, sizeof(server))) != 0) { 
		printf("socket bind failed...\n"); 
		return 1;
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		return 1; 
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cli); 

	// Accept the data packet from client and verification 
	connfd = accept(sockfd, (SA*)&cli, &len); 
	if (connfd < 0) { 
		printf("server acccept failed...\n"); 
		return 1; 
	} 
	else
		printf("server acccept the client...\n"); 

	// Function for chatting between client and server 
	getFile(connfd); 

	// After chatting close the socket 
	close(sockfd); 
} 
