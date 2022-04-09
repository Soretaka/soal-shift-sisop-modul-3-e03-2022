#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#define PORT 8080

char input[1024];
int valread;
int server_fd;
struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);
char cmd[1024], socketIn[1024];
char dirPath[100] = "/home/user/Server", txtPath[100], tsvPath[100];
char usernameArr[100], passwordArr[100], fromFile[100];
int isValid = 0, isValidPass = 0, isValidRewel = 0, isValidDir = 0;
pthread_t tid[100];
int ctrClient = 0;
char judulProblem[100], problemPath[100], answerPath[100];
int checkEmpty;

void changeInput(char in[1024]){
    strcpy(input,"");
    strcat(input,in);
}
void readBuffer(int new_socket){
    char buffer[1024] = {0};
    valread = read( new_socket , buffer, 1024);
    printf("Client: %s\n",buffer );
}
void sendInput(int new_socket){
    send(new_socket , input , strlen(input) , 0 );
}
void readBufferSp(int new_socket){
    char buffer[1024] = {0};
    valread = read( new_socket , buffer, 1024);
    strcpy(cmd,buffer);
}
void createTxt(){
    strcpy(txtPath,dirPath);
    strcat(txtPath,"/users.txt");
    FILE * fp;
    fp = fopen (txtPath, "a+");
    fclose(fp);   
}
void createTsv(){
    strcpy(tsvPath,dirPath);
    strcat(tsvPath,"/files.tsv");
    FILE *fp = fopen(tsvPath, "a+");
    fclose(fp);
}
void findUser(){
    isValid=0, isValidPass=0, isValidRewel=0;
    FILE * fp;
    fp = fopen (txtPath, "r");
    checkEmpty=0;
    while (fscanf(fp,"%s\n",fromFile) != EOF){
	    checkEmpty++;
	    int i=0, j=0;
	    char userTemp[100], passTemp[100];
	    strcpy(userTemp,"");strcpy(passTemp,"");
	    while(fromFile[i] && fromFile[i]!=':'){
	        userTemp[j]=fromFile[i];
		i++;j++;
	    }
	    userTemp[j]='\0';
	    i++;j=0;
	    while(fromFile[i] && fromFile[i]!='\n'){
	        passTemp[j]=fromFile[i];
		i++;j++;
	    }
	    passTemp[j]='\0';
	    
	    if(strcmp(userTemp, usernameArr)==0)isValid = 1; // username ditemukan
	    if(strcmp(passTemp, passwordArr)==0)isValidPass = 1; // password benar
	    int cekzz[4]={0};
	    for(int i=0;i<strlen(passwordArr);i++){
	        if(passwordArr[i]>=48 && passwordArr[i]<=57)cekzz[0]=1;
	        if(passwordArr[i]>=65 && passwordArr[i]<=90){cekzz[1]=1;cekzz[3]++;}
	        if(passwordArr[i]>=97 && passwordArr[i]<=122){cekzz[2]=1;cekzz[3]++;}
	    }
	    if(cekzz[0] && cekzz[1] && cekzz[2] && cekzz[3]>=6)isValidRewel = 1; // password angka lowercase uppercase
	    if(isValid)break;
    }
    if(checkEmpty==0){//user pertama
	int cekzz[4]={0};
    	for(int i=0;i<strlen(passwordArr);i++){
	    if(passwordArr[i]>=48 && passwordArr[i]<=57)cekzz[0]=1;
	    if(passwordArr[i]>=65 && passwordArr[i]<=90){cekzz[1]=1;cekzz[3]++;}
	    if(passwordArr[i]>=97 && passwordArr[i]<=122){cekzz[2]=1;cekzz[3]++;}
	}
	if(cekzz[0] && cekzz[1] && cekzz[2] && cekzz[3]>=6)isValidRewel = 1; // password angka lowercase uppercase
    }
    fclose(fp);   
}
void addUser(){
    FILE * fp;
    fp = fopen (txtPath, "a+");
    char insertz[100];
    strcpy(insertz,"");
    strcpy(insertz,usernameArr);
    strcat(insertz,":");
    strcat(insertz,passwordArr);
    fprintf(fp, "%s\n", insertz);
    fclose(fp);
}
void seeTsv(int new_socket){
    strcpy(input,"");
    FILE *fp = fopen("files.tsv", "r");
    char left[100],right[100];
    while (fscanf(fp,"%s\t%s\n",left,right) != EOF){
	strcat(input,left);
	strcat(input," by ");
	strcat(input,right);
	strcat(input,"\n");
    }
    fclose(fp);
    if(strcmp(input,"")==0)strcpy(input,"No problem yet\n");
    send(new_socket , input , strlen(input) , 0 );
}
void checkDir(){
    DIR* dir = opendir(judulProblem);
    if (dir) {
        closedir(dir);
	isValidDir = 0;
    }
    else if (ENOENT == errno)isValidDir = 1;
    else isValidDir = 0;
}
void addData(){
    mkdir(judulProblem, 0777);
    FILE *fp = fopen("files.tsv", "a+");
    fprintf(fp,"%s\t%s\n",judulProblem,usernameArr);
    fclose(fp);
    strcpy(problemPath,"/home/user/Server/");
    strcat(problemPath,judulProblem);
}
void saveTo(int order){
    if(order==1){// description.txt
	char tempPath[1024];
	strcpy(tempPath,problemPath);
	strcat(tempPath,"/description.txt");
	
	FILE *fp = fopen(tempPath, "a+");
	fprintf(fp, "%s",cmd);
	fclose(fp);
    }
    else if(order==2){// input.txt
	char tempPath[1024];
	strcpy(tempPath,problemPath);
	strcat(tempPath,"/input.txt");
	
	FILE *fp = fopen(tempPath, "a+");
	fprintf(fp, "%s",cmd);
	fclose(fp);
    }
    else{// output.txt
	char tempPath[1024];
	strcpy(tempPath,problemPath);
	strcat(tempPath,"/output.txt");
	
	FILE *fp = fopen(tempPath, "a+");
	fprintf(fp, "%s",cmd);
	fclose(fp);
    }
}
void verdict(){
    FILE * fp;
    char correct[1024],tested[1024], correctPath[1024];
    strcpy(correctPath,dirPath);
    strcat(correctPath,"/");
    strcat(correctPath,judulProblem);
    strcat(correctPath,"/output.txt");
    strcpy(tested,"");
    strcpy(correct,"");
    //get tested output
    fp = fopen (answerPath, "r");
    int ct=0;
    do
    {
	char c = fgetc(fp);
        if (feof(fp))
            break ;

        tested[ct]=c;
        ct++;
    }  while(1);
    tested[ct]='\0';
    fclose(fp); 
    //get correct output
    fp = fopen (correctPath, "r");
    ct=0;
    do
    {
	char c = fgetc(fp);
        if (feof(fp))
            break ;

        correct[ct]=c;
        ct++;
    }  while(1);
    correct[ct]='\0';
    fclose(fp); 
    
    if(strcmp(correct,tested)==0)changeInput("AC");
    else changeInput("WA");
}

void *client(void *tmp){
    int new_socket = *(int *)tmp;
    printf("A client connected\n");
    if (ctrClient == 1) {
	changeInput("Register / Login? (case sensitive)");
	sendInput(new_socket);
    }
    else {
	changeInput("Wait other client to close connection...");
	sendInput(new_socket);
    }
    
    while (ctrClient > 1) {
        readBuffer(new_socket);
        if (ctrClient == 1) {
            changeInput("Register / Login? (case sensitive)");
	    sendInput(new_socket);
        }
        else {
            changeInput("Wait other client to close connection...");
	    sendInput(new_socket);
        }
    }

    readBufferSp(new_socket);
    while(strcmp(cmd,"Login")!=0 && strcmp(cmd,"Register")!=0){
        changeInput("Register / Login? (case sensitive)");
        sendInput(new_socket);
        readBufferSp(new_socket);
    }
    
    strcpy(socketIn,"Silakan ");
    strcat(socketIn,cmd);
    strcat(socketIn,"\nServer: ");
	
    if(strcmp(cmd,"Login")==0){
	isValid=0, isValidPass=0;
	while(!isValid || !isValidPass){
	    strcat(socketIn,"Masukkan username");
	    changeInput(socketIn);
	    sendInput(new_socket);
	    readBufferSp(new_socket);
	    strcpy(usernameArr,cmd);

	    changeInput("Masukkan password");
	    sendInput(new_socket);
	    readBufferSp(new_socket);
	    strcpy(passwordArr,cmd);

	    findUser();
	    if(isValid && isValidPass)break; // username ditemukan dan password sesuai
	    strcpy(socketIn,"Username atau password salah, ulangi proses !\nServer: ");
	}
        changeInput("Login berhasil");
	sendInput(new_socket);
    }
    else if(strcmp(cmd,"Register")==0){
	isValid=1, isValidRewel=0;
	while(isValid || !isValidRewel){
	    strcat(socketIn,"Masukkan username");
	    changeInput(socketIn);
	    sendInput(new_socket);
	    readBufferSp(new_socket);
	    strcpy(usernameArr,cmd);

	    changeInput("Masukkan password (ada angka, lowercase, uppercase, 6 huruf)");
	    sendInput(new_socket);
	    readBufferSp(new_socket);
	    strcpy(passwordArr,cmd);
	    
            findUser();
	    if(!isValid && isValidRewel)break; // username belum ditemukan dan kriteria password sesuai
	    strcpy(socketIn,"Data tidak valid atau sudah ada, ulangi proses !\nServer: ");
	    isValid=1, isValidRewel=0;	
	}
	changeInput("Register berhasil");
	sendInput(new_socket);
	addUser();
    }
    readBufferSp(new_socket);
    while(1){
	if(strcmp(cmd,"see")==0){
	    seeTsv(new_socket);
	}
	else if(strcmp(cmd,"logout")==0){
	    printf("A client closed connection");
	    changeInput("Successfully disconnected");
	    sendInput(new_socket);
	    close(new_socket);
    	    ctrClient--;
	    break;
	}
	else if(strcmp(cmd,"add")==0){
	    strcpy(judulProblem,"");
	    strcpy(problemPath,"");
	    isValidDir = 0;
	    changeInput("Judul problem:");
	    sendInput(new_socket);
	    readBufferSp(new_socket);
	    strcpy(judulProblem,cmd);
	    checkDir();
	    while(!isValidDir){
	    	changeInput("Directory exists, ganti judul problem:");
	    	sendInput(new_socket);
		readBufferSp(new_socket);
		strcpy(judulProblem,cmd);
		checkDir();
	    }
	    addData(); //create dir, save dir path, add to .tsv
	
	    changeInput("Filepath description.txt:");
	    sendInput(new_socket);
	    readBufferSp(new_socket);
	    saveTo(1);
	
	    changeInput("Filepath input.txt:");
	    sendInput(new_socket);
	    readBufferSp(new_socket);
	    saveTo(2);
		
	    changeInput("Filepath output.txt:");
	    sendInput(new_socket);
	    readBufferSp(new_socket);
	    saveTo(3);
	    
	    changeInput("Data berhasil ditambahkan!");
	    sendInput(new_socket);
	}
	else{
	    if(strlen(cmd)>=8 && cmd[0]=='d' && cmd[1]=='o' && cmd[2]=='w' && cmd[3]=='n' && cmd[4]=='l' && cmd[5]=='o' && cmd[6]=='a' && cmd[7]=='d'){
		int countzz=0, startzz=9;
		while(cmd[startzz]!='\0'){
		    judulProblem[countzz]=cmd[startzz];
		    countzz++;startzz++;
		}
		judulProblem[countzz]='\0';
		
		strcpy(problemPath,"");
		strcat(problemPath,dirPath);
		strcat(problemPath,"/");
		strcat(problemPath,judulProblem);
		changeInput(problemPath);
		sendInput(new_socket);
	    }
	    else if(strlen(cmd)>=6 && cmd[0]=='s' && cmd[1]=='u' && cmd[2]=='b' && cmd[3]=='m' && cmd[4]=='i' && cmd[5]=='t'){
		int countzz=0, startzz=7;
		while(cmd[startzz]!=' '){
		    judulProblem[countzz]=cmd[startzz];
		    countzz++;startzz++;
		}
		judulProblem[countzz]='\0';
		    
		countzz=0;startzz++;
		while(cmd[startzz]!='\0'){
		    answerPath[countzz]=cmd[startzz];
		    countzz++;startzz++;
		}    
		answerPath[countzz]='\0';   
		verdict();
		sendInput(new_socket);
	    }
	    else{
		changeInput("Invalid argument");
	        sendInput(new_socket);
	    }
	    
	}
	readBufferSp(new_socket);
    }
	
    
}
int main(int argc, char const *argv[]) {
    int new_socket;
    strcpy(cmd,"");
    createTxt();
    createTsv();

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while(1){
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
	    perror("accept");
	    exit(EXIT_FAILURE);
	}
	pthread_create(&(tid[ctrClient]), NULL, &client, &new_socket);
        ctrClient++;
    }
    
    return 0;
}
