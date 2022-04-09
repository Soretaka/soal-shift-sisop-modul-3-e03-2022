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
pthread_mutex_t bufferlock;

typedef struct file{
    char DIR[1024];
    char filename[1024];
}file_t;

//mengecilkan huruf besar
void tolowerstr(char* str){
    for(int i = 0 ; i < strlen(str);i++){
        if(str[i]<=90 && str[i]>=65){
            str[i]+=32;
        }else{  
            continue;
        }
    }
}

//mendapatkan ekstensi file
char* filename_ext(char *filename){
  printf("jalan1 %s\n",filename);
    char* extname = (char*)malloc(sizeof(char)*(PATH_MAX + PATH_MAX));
    memset(extname,'\0',sizeof(char)*(PATH_MAX + PATH_MAX));
    char* dot = strchr(filename,'.');
    char *dotted=strrchr(filename,'/');
    if(dotted[1]=='.'){
        strcpy(extname,"Hidden");
        return extname;
    }else if(dot == NULL){
        strcpy(extname,"Unknown");
        return extname;
    }else{
        strcpy(extname,dot+1);
        tolowerstr(extname);
        return extname;
    }
}

//mengkategorikan berdasarkan ekstensi
void* movetofolder(void* args){
    file_t* filenow = (file_t*)args;
    printf("jalan1 %s\n",filenow->filename);
    char* extname=filename_ext(filenow->filename);
    printf("jalan %s\n",filenow->filename);
    char* pathname = (char*)malloc(sizeof(char)*(PATH_MAX + PATH_MAX));
    memset(pathname,0,sizeof(char)*(PATH_MAX + PATH_MAX));
    strcpy(pathname,filenow->DIR);
    strcat(pathname,"/hartakarunans/");
    strcat(pathname,extname);
    mkdir(pathname,0777);
    pthread_mutex_lock(&bufferlock);
    strcat(pathname,"/");
    char newpath[(PATH_MAX + PATH_MAX)];
    memset(newpath,0,sizeof(newpath));
    strcpy(newpath,pathname);
    strcat(newpath,basename(filenow->filename));
      printf("moving %s to %s\n",filenow->filename,newpath);
    rename(filenow->filename,newpath);
    pthread_mutex_unlock(&bufferlock);
}

int it=0;
//mengiterasi file
void kategoriin (char* base, pthread_t *thread){
    char path[1024];
    struct dirent *dp;
    DIR *dir = opendir(base);
    if(!dir) return;
    while ((dp = readdir(dir))!=NULL){
        if(strcmp(dp->d_name,".")!=0 && strcmp(dp->d_name,"..")!=0){
            strcpy(path,base);
            strcat(path,"/");
            strcat(path,dp->d_name);
            file_t* filenow=(file_t*)malloc(sizeof(file_t));
            strcpy(filenow->filename,path);
            printf("%s\n",filenow->filename);
            strcpy(filenow->DIR,"/home/soreta/sisop3");
            if(strcmp(path,"/home/soreta/sisop3")!=0){
                pthread_create(&thread[it],NULL,movetofolder,(void*)filenow);
                sleep(1);
                it++;
            }
            
        }
    }
    closedir(dir);
}


int main(int argc, char * argv[]){
    char curDir[2048];
    getcwd(curDir,sizeof(curDir));
    pthread_mutex_init(&bufferlock,NULL);
    pthread_t copy_thread[1000];
    char renamefolder[2048];
    strcpy(renamefolder,curDir);
    mkdir("hartakarunans",07770);
    strcat(curDir,"/hartakarun");
    DIR *d;
    struct dirent *dir;
    struct dirent *dp;
    d=opendir(curDir);
    if(d){
        kategoriin(curDir,copy_thread);
    }
    closedir(d);
    d=opendir(".");
    if(d){
        while((dp=readdir(d))!=NULL){
            if(strcmp(dp->d_name,".")!=0 && strcmp(dp->d_name,"..")!=0){
                if(dp->d_type==DT_REG){
                    continue;
                }else{
                    printf("%s\n",dp->d_name);
                    if(strcmp(dp->d_name,"hartakarunans")==0){
                        printf("jalan");
                        rename(dp->d_name,"hartakarun");
                    }
                }
            }
        }
    }
    closedir(d);
}