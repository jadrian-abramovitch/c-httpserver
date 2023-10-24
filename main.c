#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>


int PORT = 1800;
int BUFFER_SIZE = 1000;
char *HTML_FILE_NAME = "error.html";

char * createHttpResponse(int status_code, int content_length, char* response_html) {

    char *response;
    sprintf(response, "HTTP/1.1 %d\nContent-Length: %d\nContent-Type: text/html\nConnection: Closed\n\n%s", status_code, content_length, response_html);
    return(response);
}

long getFileLength(char* file_name) {

    FILE * file; 
    file = fopen(file_name, "rb");
    if (file) {
        printf("file opened! \n");
    } else {
        printf("file not opened :( \n");
    }

    long file_size;
    fseek(file, 0, SEEK_END); 
    file_size = ftell(file);
    fclose(file);
    return(file_size); 
}

char * readFile(char* strBuffer, char* file_name, long file_length) {

    FILE * file; 
    file = fopen(file_name, "rb");
    if (file) {
        printf("file opened! \n");
    } else {
        printf("file not opened :( \n");
    }

    fread(strBuffer, file_length, 1, file);
    fclose(file);
}

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

    while (1) {

        int connfd;
        struct sockaddr_in addr;
        socklen_t addr_len = sizeof(addr);
        printf("waiting on connention \n");

        if ((connfd = accept(server_fd, (struct sockaddr *)&addr, &addr_len)) < 0) {
            perror("failed to accept");
            exit(-1);
        }
        
        char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

        ssize_t bytes_revieced = recv(connfd, buffer, BUFFER_SIZE,0);
        printf("%.*s\n",BUFFER_SIZE,buffer);
        int method_length = strcspn(buffer, " ");
        char *method;
        if (memcmp(buffer, "GET", strlen("GET")) == 0) {
            method = "GET";
        } 

        buffer += method_length + 1; //move pointer past method
        
        size_t url_length = strcspn(buffer, " ");
        char *url = malloc(url_length);
        memcpy(url, buffer, url_length);

        if (strcmp(url, "/") == 0) {
            HTML_FILE_NAME = "index.html";
        } else if (strcmp(url, "/hello") == 0) {
            HTML_FILE_NAME = "hello.html";
        } else {
            HTML_FILE_NAME = "error.html";
        }


        long file_length = getFileLength(HTML_FILE_NAME);
        char responseHtml[file_length];
        readFile(responseHtml, HTML_FILE_NAME, file_length);

        char *httpTest = createHttpResponse(200, strlen(responseHtml), responseHtml);
        
        if (write(connfd, httpTest, strlen(httpTest)) < 0) {
            perror("could not write");
        }
        close(connfd);
    }

    close(server_fd);
    return 0;
}
