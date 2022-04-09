#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#define SIZE 1024
 
 void zipfile(){
    int status;
    if(fork()==0){
        char *argv[]={"zip","-rm","hartakarun.zip","hartakarun",NULL};
        execv("/bin/zip",argv);
    }else while(wait(&status)>0);
}

void send_file(FILE *fp, int sockfd){
  int n;
  char data[SIZE] = {0};
  bzero(data,SIZE);
  int size_file=0;
  while((size_file = fread(data,sizeof(char),SIZE,fp))>0) {
    if (send(sockfd, data, sizeof(data), 0) == -1) {
      perror("[-]Error in sending file.");
      exit(1);
    }
    bzero(data, SIZE);
  }
}
 
int main(){
    zipfile();
  char *ip = "127.0.0.1";
  int port = 8080;
  int e;
 int valread;
  int sockfd;
  struct sockaddr_in server_addr;
  FILE *fp;
  char *filename = "hartakarun.zip";
 
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");
 
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);
  char buffer[1024]={0};
  e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e == -1) {
    perror("[-]Error in socket");
    exit(1);
  }
 printf("[+]Connected to Server.\n");
 char choice[1024];
 int jalan=1;
 while(jalan){
    // getchar();
   bzero(choice, 1024);
		scanf(" %[^\n]s", choice);
    printf("%s\n",choice);
    write(sockfd, choice, strlen(choice));
  if(!(strcmp(choice,"send hartakarun.zip"))){
  fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("[-]Error in reading file.");
    exit(1);
  }
 
  send_file(fp, sockfd);
  printf("[+]File data sent successfully.\n");
  jalan=0;
  }else{
    printf("hah\n");
  }
  printf("[+]Closing the connection.\n");
  close(sockfd);
 }
  return 0;
}