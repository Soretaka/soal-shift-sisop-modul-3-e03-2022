#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <sys/stat.h>
#include<dirent.h>
#include<libgen.h>
#include <arpa/inet.h>
#define SIZE 1024


void zipfile(){
    int status;
    if(fork()==0){
        char *argv[]={"zip","-r","hartakarun.zip","hartakarun",NULL};
        execv("/bin/zip",argv);
    }else while(wait(&status)>0);
}

int main(int argc, char * argv[]){
    zipfile();
    //server
    char *ip = "127.0.0.1";
    int port = 8080;
    int e;

    int sockfd;
    struct sockaddr_in server_addr;
    FILE *fp;
    char *filename = "hartakarun.zip";
    printf("wow");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(e == -1) {
        perror("[-]Error in socket");
        exit(1);
    }
        printf("[+]Connected to Server.\n");
    sendfile(fp, sockfd);
    printf("wow2");
    printf("[+]File data sent successfully.\n");

        printf("[+]Closing the connection.\n");
    close(sockfd);

    return 0;
}