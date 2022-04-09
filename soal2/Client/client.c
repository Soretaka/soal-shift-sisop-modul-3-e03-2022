#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#define PORT 8080

char input[1024];
int sock = 0, valread;
struct sockaddr_in address;
struct sockaddr_in serv_addr;
char cmd[1024], clientIn[1024];
char dirPath[100] = "/home/user/Client";
int isValidc = 0;
char judul[1024];

void changeInput(char in[1024]){
    strcpy(input,"");
    strcat(input,in);
}
void readBuffer(){
    char buffer[1024] = {0};
    valread = read( sock , buffer, 1024);
    strcpy(cmd,buffer);
    printf("Server: %s\n",cmd );
}
void sendInput(){
    send(sock , input , strlen(input) , 0 );
}
void readBufferSp(){
    char buffer[1024] = {0};
    valread = read( sock , buffer, 1024);
    strcpy(cmd,buffer);
}
void sendFile(){
    strcpy(input,"");
    FILE * fp;
    fp = fopen (clientIn, "r");
    int ct=0;
    do
    {
        char c = fgetc(fp);
        if (feof(fp))
            break ;
 
        input[ct]=c;
        ct++;
    }  while(1);
    input[ct]='\0';
    fclose(fp); 
    send(sock , input , strlen(input) , 0 );
}
void downloadSoal(){
    mkdir(judul,0777);
    for(int i=0;i<2;i++){
        if(i==0){//download description.txt
            strcpy(input,"");
            char temp[100];
            strcpy(temp,cmd);
            strcat(temp,"/description.txt");
            FILE * fp;
            fp = fopen (temp, "r");
            int ct=0;
            do
            {
                char c = fgetc(fp);
                if (feof(fp))
                    break ;

                input[ct]=c;
                ct++;
            }  while(1);
            input[ct]='\0';
            fclose(fp); 
            
            char tempPath[100];
            strcpy(tempPath,dirPath);
            strcat(tempPath,"/");
            strcat(tempPath,judul);
            strcat(tempPath,"/description.txt");
            FILE *fp2 = fopen(tempPath, "a+");
	    fprintf(fp2, "%s",input);
	    fclose(fp2);
        }
        else{//download input.txt
            strcpy(input,"");
            char temp[1000];
            strcpy(temp,cmd);
            strcat(temp,"/input.txt");
            FILE * fp;
            fp = fopen (temp, "r");
            int ct=0;
            do
            {
                char c = fgetc(fp);
                if (feof(fp))
                    break ;

                input[ct]=c;
                ct++;
            }  while(1);
            input[ct]='\0';
            fclose(fp); 
            
            char tempPath[100];
            strcpy(tempPath,dirPath);
            strcat(tempPath,"/");
            strcat(tempPath,judul);
            strcat(tempPath,"/input.txt");
            FILE *fp2 = fopen(tempPath, "a+");
	    fprintf(fp2, "%s",input);
	    fclose(fp2);
        }
    }
    strcpy(input,"");
}

int main(int argc, char const *argv[]) {
    strcpy(cmd,"");
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    strcpy(cmd, "");
    printf("Connected to Server\n");
    readBufferSp();
    printf("Server: %s\n",cmd );
    while (strcmp(cmd, "Register / Login? (case sensitive)")!=0) {
        printf("Refresh connection with random chat\n");
        scanf("%[^\n]%*c", clientIn);
        changeInput(clientIn);
        sendInput();
        readBufferSp();
        printf("Server: %s\n",cmd );
    }
    
    strcpy(cmd, "");
    scanf("%[^\n]%*c", clientIn);
    changeInput(clientIn);
    sendInput();
    readBufferSp();
    printf("Server: %s\n",cmd );
    while(strcmp(cmd,"Register / Login? (case sensitive)")==0){
        scanf("%[^\n]%*c", clientIn);
        changeInput(clientIn);
        sendInput();
        readBufferSp();
        printf("Server: %s\n",cmd );
    }
    
    if(strcmp(cmd,"Silakan Login")==0 || strcmp(cmd,"Silakan Login\nServer: Masukkan username")==0){
        isValidc = 0;
        while(!isValidc){
            // input username
            scanf("%[^\n]%*c", clientIn);
            changeInput(clientIn);
            sendInput();

            // input password
            readBuffer();
            scanf("%[^\n]%*c", clientIn);
            changeInput(clientIn);
            sendInput();
            
            readBufferSp();
            printf("Server: %s\n",cmd );
            if(strcmp(cmd, "Login berhasil")==0)isValidc=1;
        }
    }
    else if(strcmp(cmd,"Silakan Register")==0 || strcmp(cmd,"Silakan Register\nServer: Masukkan username")==0){
        isValidc = 0;
        while(!isValidc){
            // input username
            scanf("%[^\n]%*c", clientIn);
            changeInput(clientIn);
            sendInput();

            // input password
            readBuffer();
            scanf("%[^\n]%*c", clientIn);
            changeInput(clientIn);
            sendInput();
            
            readBufferSp();
            printf("Server: %s\n",cmd );
            if(strcmp(cmd, "Register berhasil")==0)isValidc=1;
        }
    }
    scanf("%[^\n]%*c", clientIn);
    changeInput(clientIn);
    sendInput();
    while(1){
        if(strcmp(clientIn,"see")==0){
            readBufferSp();
            printf(".tsv list:\n%s",cmd);
        }
        else if(strcmp(clientIn,"logout")==0){
            readBuffer();
            break;
        }
        else if(strcmp(clientIn,"add")==0){
            //masukkan judul
            readBuffer();
            scanf("%[^\n]%*c", clientIn);
            changeInput(clientIn);
            sendInput();
            readBuffer();
            while(strcmp(cmd,"Directory exists, ganti judul problem:")==0){
                scanf("%[^\n]%*c", clientIn);
                changeInput(clientIn);
                sendInput();
                readBufferSp();
                printf("Server: %s\n",cmd);
            }
            //masukkan path description.txt
            scanf("%[^\n]%*c", clientIn);
            sendFile();
            
            readBuffer();
            //masukkan path input.txt
            scanf("%[^\n]%*c", clientIn);
            sendFile();
            
            readBuffer();
            //masukkan path output.txt
            scanf("%[^\n]%*c", clientIn);
            sendFile();
            readBuffer();
        }
        else {
            if(strlen(clientIn)>=8 && clientIn[0]=='d' && clientIn[1]=='o' && clientIn[2]=='w' && clientIn[3]=='n' && clientIn[4]=='l'
               && clientIn[5]=='o' && clientIn[6]=='a' && clientIn[7]=='d'){
                strcpy(judul,"");
                int countzz=0, startzz=9;
		        while(cmd[startzz]!='\0'){
		            judul[countzz]=clientIn[startzz];
		            countzz++;startzz++;
		        }
		        judul[countzz]='\0';
                readBufferSp();
                downloadSoal();
                printf("Soal %s berhasil terdownload!\n",judul);
            }
            else if(strlen(clientIn)>=6 && clientIn[0]=='s' && clientIn[1]=='u' && clientIn[2]=='b' && clientIn[3]=='m' && clientIn[4]=='i' && clientIn[5]=='t'){
                readBuffer();//AC or WA
            }
            else readBuffer();
        }
        
        scanf("%[^\n]%*c", clientIn);
        changeInput(clientIn);
        sendInput();
    }
  
    return 0;
}
