#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>


int PORT = 1800;
int BUFFER_SIZE = 1000;


int main() {

    int server_fd;
    struct sockaddr_in server_addr;
    
    printf("Starting Sever \n");
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket could not connect");
        exit(-1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);


    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("failed to bind");
        exit(-1);
    }

    if (listen(server_fd, 10) < 0) {
        perror("failed to listen");
        exit(-1);
    }

    while (1) {

        int connfd;
        struct sockaddr_in addr;
        socklen_t addr_len = sizeof(addr);

        printf("waiting on connention \n");

        if ((connfd = accept(server_fd, (struct sockaddr *)&addr, &addr_len)) < 0) {
            perror("failed to accept");
            exit(-1);
        }
        
        close(connfd);
    }

    close(server_fd);
    return 0;
}