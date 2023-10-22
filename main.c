#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


int PORT = 1801;
int BUFFER_SIZE = 1000;

int main() {
    printf("Starting Sever \n");
    int server_fd;
    struct sockaddr_in server_addr;
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

    // struct sockaddr_in addr;
    // socklen_t addr_len;
    // int connection_fd;
    // if (connection_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len) < 0) {
    //     perror("failed to accept");
    //     exit(-1);
    // }
 
    // FILE * file; 
    // file = fopen("index.html", "r");
    // if (file) {
    //     printf("file opened!");
    // } else {
    //     printf("file not opened :(");
    // }
    
    // fclose(file);   
    FILE * file; 
    file = fopen("index.html", "r");
    if (file) {
        printf("file opened! \n");
    } else {
        printf("file not opened :( \n");
    }
    long file_size;
    fseek(file, 0, SEEK_END); // seek to end of file
    file_size = ftell(file); // get current file pointer
    fseek(file, 0, SEEK_SET); // seek back to beginning of file
    char myString[file_size];
    int size = 76;
    char testString[76] = "Content-Type: text/html; charset=utf-8\r\n\r\n \n";
    fgets(myString, file_size, file);

    
    while (1) {
        int connfd;
        struct sockaddr_in addr;
        socklen_t addr_len = sizeof(addr);
        printf("waiting on connention \n");

        if ((connfd = accept(server_fd, (struct sockaddr *)&addr, &addr_len)) < 0) {
            perror("failed to accept");
            exit(-1);
        }

        
        printf("end of while loop \n");
        
        char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

        ssize_t bytes_revieced = recv(connfd, buffer, BUFFER_SIZE,0);
        // printf("%.*s",BUFFER_SIZE,buffer);
        // if (send(connfd, testString, size, 0) < 0 ) {
        //     perror("error sending");
        // }
        // if (write(connfd, testString, size) < 0) {
        //     perror("could not write header");
        // }
        if (send(connfd, myString, file_size, 0) < 0) {
            perror("could not write");
        }
        close(connfd);
        // printf("%.*s",sizeof(file),file);
    }

    fclose(file);
    close(server_fd);
    return 0;

}