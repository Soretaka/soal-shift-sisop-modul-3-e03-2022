#define NUM_THREADS 5
#define SIZE 1000
#define PATH_MAX 256
#include <unistd.h>
#include <stdio.h>
#include <json-c/json.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

void extractFile(){
	char path[256] = "/home/daniel/modul3";
	if(fork()==0){
		char temp[100];
		strcpy(temp, path);
		strcat(temp, "/music");
		
		char *argv[] = {"unzip", "music.zip", "-d",temp, NULL};
		execv("/bin/unzip", argv);
	}
	else{
		char temp[100];
		strcpy(temp, path);
		strcat(temp, "/quote");
		
		char *argv2[] = {"unzip", "quote.zip", "-d",temp, NULL};
		execv("/bin/unzip", argv2);
	}
}
void downloadFile(){ // https://drive.google.com/uc?export=download&id=FILEID
		char path[256] = "/home/daniel/modul3";	
	if(fork()==0){
		char temp[100];
		strcpy(temp, path);
		//strcat(temp, "/gacha_gacha");
		strcat(temp, "/music.zip");
		char *argv[] = {"wget", "-q", "https://drive.google.com/uc?export=download&id=1_djk0z-cx8bgISFsMiUaXzty2cT4VZp1", "-O", temp};
		printf("music.zip downloaded.");
		execv("/bin/wget", argv);
		// https://drive.google.com/file/d/1_djk0z-cx8bgISFsMiUaXzty2cT4VZp1/view?usp=sharing
	}
	else{
		char temp[100];
		strcpy(temp, path);
		//strcat(temp, "/gacha_gacha");
		strcat(temp, "/quote.zip");
		char *argv2[] = {"wget", "-q", "https://drive.google.com/uc?export=download&id=1jR67_JAyozZPs2oYtYqEZxeBLrs-k3dt", "-O", temp};
		printf("quote.zip downloaded.");
        	execv("/bin/wget", argv2);
        	// https://drive.google.com/file/d/1jR67_JAyozZPs2oYtYqEZxeBLrs-k3dt/view?usp=sharing
	}
}

void music() {
	char cwd[PATH_MAX];
	if(getcwd(cwd,sizeof(cwd))!=NULL){
		printf("Current working dir : %s\n", cwd);
	}
	else {
		printf("getcwd() error\n");
	}
	char *argv[] = {"unzip", "music.zip", "-d", "/home/daniel/modul3/music", NULL};
	execv("/bin/unzip", argv);	
}

void quote(){
	char cwd[PATH_MAX];
	if(getcwd(cwd,sizeof(cwd))!=NULL){
		printf("Current working dir : %s\n", cwd);
	}
	else {
		printf("getcwd() error\n");
	}
	char *argv2[] = {"unzip", "quote.zip", "-d", "/home/daniel/modul3/quote", NULL};
	execv("/bin/unzip", argv2);
}

char* convertBase64(char input_str[])
{
	// calculates length of string
	int len_str = sizeof(input_str) / sizeof(input_str[0]);
	
	// to exclude '\0' character
	len_str -= 1;

	printf("Input string is : %s\n", input_str);
	//printf("Encoded string is : %s\n", base64Encoder(input_str, len_str));
	// Character set of base64 encoding scheme
	char char_set[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	// Resultant string
	char *res_str = (char *) malloc(SIZE * sizeof(char));
	
	int index, no_of_bits = 0, padding = 0, val = 0, count = 0, temp;
	int i, j, k = 0;
	
	// Loop takes 3 characters at a time from
	// input_str and stores it in val
	for (i = 0; i < len_str; i += 3)
		{
			val = 0, count = 0, no_of_bits = 0;

			for (j = i; j < len_str && j <= i + 2; j++)
			{
				// binary data of input_str is stored in val
				val = val << 8;
				
				// (A + 0 = A) stores character in val
				val = val | input_str[j];
				
				// calculates how many time loop
				// ran if "MEN" -> 3 otherwise "ON" -> 2
				count++;
			
			}

			no_of_bits = count * 8;

			// calculates how many "=" to append after res_str.
			padding = no_of_bits % 3;

			// extracts all bits from val (6 at a time)
			// and find the value of each block
			while (no_of_bits != 0)
			{
				// retrieve the value of each block
				if (no_of_bits >= 6)
				{
					temp = no_of_bits - 6;
					
					// binary of 63 is (111111) f
					index = (val >> temp) & 63;
					no_of_bits -= 6;		
				}
				else
				{
					temp = 6 - no_of_bits;
					
					// append zeros to right if bits are less than 6
					index = (val << temp) & 63;
					no_of_bits = 0;
				}
				res_str[k++] = char_set[index];
			}
	}

	// padding is done here
	for (i = 1; i <= padding; i++)
	{
		res_str[k++] = '=';
	}

	res_str[k] = '\0;';

	return res_str;
}

void createMusicTxt() // create .txt in the air folder
{
    // Open or Create Specific File
    FILE* file_ptr = fopen("/home/daniel/modul3/music/music.txt", "w");

    DIR *d;
    struct dirent *dir;
    d = opendir("/home/daniel/modul3/music"); // Open Directory
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
         if (strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0) 
	    {
	    printf("cek : %s\n", dir->d_name);
            char path[256] = "/home/daniel/modul3/music/";
            strcat(path, dir->d_name);
	    FILE* file_open = fopen(path,"r");
	    while (fgets(path,265,file_open)) fprintf(file_ptr, "%s\n", convertBase64(path));
            // check_permission(path,dir->d_name, file_ptr);
	    //fprintf(file_ptr, "%s", convertBase64(file_open));
            }
        }
        closedir(d);
        fclose(file_ptr);
    }
}

void createQuoteTxt() // create .txt in the air folder
{
    // Open or Create Specific File
    FILE* file_ptr = fopen("/home/daniel/modul3/quote/quote.txt", "w");

    DIR *d;
    struct dirent *dir;
    d = opendir("/home/daniel/modul3/quote"); // Open Directory
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
         if (strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0) 
	    {
	    printf("cek : %s\n", dir->d_name);
            char path[256] = "/home/daniel/modul3/quote/";
            strcat(path, dir->d_name);
	    FILE* file_open = fopen(path,"r");
	    while (fgets(path,265,file_open)) fprintf(file_ptr, "%s\n", convertBase64(path));
            }
        }
        closedir(d);
        fclose(file_ptr);
    }
}

void driverB() {
	if (fork() == 0) {
	createMusicTxt();
	}
	else {
	createQuoteTxt();
	}
}

void driverA() {	
	pid_t child_id;
  	child_id = fork();
  	int status;
  	
	if(child_id==0){
		music();
	}
	else{
		while ((wait(&status)) > 0);
		quote();
	}
}

int main() {
	if (fork()==0) driverA;
	else driverB;
	return 0;
}