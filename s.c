#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/tcp.h> 
#include <time.h> 
#define MAX 1024 * 1024
#define PORT 8080
#define SA struct sockaddr

// Function designed for chat between client and server.
void func(int sockfd)
{
    char buff[MAX];
    clock_t t;
    size_t i;
    double avgTime = 0;
    // infinite loop for chat
    for (i = 0; i < 5; i++)
    {
        bzero(buff, MAX);

        // read the message from client and copy it in buffer
        t = clock();
        read(sockfd, buff, sizeof(buff));
        // print buffer which contains the client contents
        t = clock() - t;
        double time = ((double) t) / CLOCKS_PER_SEC * 1000;
        avgTime += time;
        printf("it's took %lf ms to get the file\n", time);
        printf("got from client: %lu bytes\n", sizeof(buff));
        // bzero(buff, MAX);
        // copy server message in the buffer
        // while ((buff[n++] = getchar()) != '\n')
            // ;
        
        // and send that buffer to client
        write(sockfd, buff, sizeof(buff));
    }
    avgTime /= i;
    printf("average time of %lf ms\n", avgTime);
}

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


// Driver function
int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA *)&cli, &len);
    if (connfd < 0)
    {
        printf("server acccept failed...\n");
        exit(0);
    }
    else
        printf("server acccept the client...\n");

    // Function for chatting between client and server
    func(connfd);

    changeCC(sockfd);

    func(connfd);
    // After chatting close the socket
    close(sockfd);
}
