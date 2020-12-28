#include <netdb.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define MB 1024 * 1024
#define PORT 8080
#define SA struct sockaddr

int changeCC(int sock) {
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

void sendFile(FILE *fp, int sockfd) {
    char buff[MB];
    int i = 0;

    bzero(buff, MB);

    while (fgets(buff, MB, fp) != NULL)
        ;
    long sent = write(sockfd, buff, MB);
    bzero(buff, MB);
    printf("send to server %ld bytes\n", sent);
}

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    } else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    } else
        printf("connected to the server..\n");

    char *fileName = "1mb.txt";
    FILE *fp;
    for (size_t i = 0; i < 5; i++) {
        fp = fopen(fileName, "r");
        sendFile(fp, sockfd);
    }

    changeCC(sockfd);

    for (size_t i = 0; i < 5; i++) {
        fp = fopen(fileName, "r");
        sendFile(fp, sockfd);
    }
    // close the socket
    close(sockfd);
}
