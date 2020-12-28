#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#define MB 1024 * 1024
#define PORT 8080
#define SA struct sockaddr

int getFile(int sockfd) {
    char buff[MB];
    size_t i;
    for (i = 0; i < 5; i++) {
        bzero(buff, MB);
        long b = read(sockfd, buff, sizeof(buff));
        printf("got from client: %lu bytes\n", b);
    }
    return i;
}

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

int main() {
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    } else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    } else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    } else
        printf("Server listening..\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA *)&cli, &len);
    if (connfd < 0) {
        printf("server acccept failed...\n");
        exit(0);
    } else
        printf("server acccept the client...\n");

    clock_t t = clock();
    double avgTime;

    size_t i = getFile(connfd);

    t = clock() - t;
    double time = ((double)t) / CLOCKS_PER_SEC * 1000;
    avgTime = time / i;
    printf("average time of %lf ms\n", avgTime);

    changeCC(sockfd);

    t = clock();
    i = getFile(connfd);

    t = clock() - t;
    time = ((double)t) / CLOCKS_PER_SEC * 1000;
    avgTime = time / i;
    printf("average time of %lf ms\n", avgTime);

    close(sockfd);
}
