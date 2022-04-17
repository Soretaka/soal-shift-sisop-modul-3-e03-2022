# Soal-shift-sisop-modul-3-E03-2022
1. Daniel Hermawan (5025201087)
2. Florentino Benedictus (5025201222)
3. Anak Agung Yatestha Parwata (5025201234)

## Soal No. 1  
Novak adalah seorang mahasiswa biasa yang terlalu sering berselancar di internet. Pada suatu saat, Ia menemukan sebuah informasi bahwa ada suatu situs yang tidak memiliki pengguna. Ia mendownload berbagai informasi yang ada dari situs tersebut dan menemukan sebuah file dengan tulisan yang tidak jelas. Setelah diperhatikan lagi, kode tersebut berformat base64. Ia lalu meminta kepada anda untuk membuat program untuk memecahkan kode-kode di dalam file yang Ia simpan di drive dengan cara decoding dengan base 64. Agar lebih cepat, Ia sarankan untuk menggunakan thread.  
  
### A)  
Download dua file zip dan unzip file zip tersebut di dua folder yang berbeda dengan nama quote untuk file zip quote.zip dan music untuk file zip music.zip. Unzip ini dilakukan dengan bersamaan menggunakan thread.  
Soal tersebut meminta kita untuk melakukan download .zip yang berapa pada link yang disediakan. Dengan menggunakan `exec` serta argumen `/bin/wget` , kita dapat melakukan download sebuah file dari alamat website yang diberikan. Kita juga perlu membuat folder `music` dan folder `quote` untuk menampung hasil dari zip tersebut.  
```c
void zip_download(){
    pid_t child_id;
    int status;

    child_id = fork();

    if (child_id < 0) {
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if (child_id == 0) {
        // this is child
        char *argv[] = {"wget", "-q", "https://drive.google.com/u/0/uc?id=1jR67_JAyozZPs2oYtYqEZxeBLrs-k3dt&export=download", "-O", "quote.zip", NULL};
        execv("/bin/wget", argv);
        exit(0);
    } else {
        // this is parent
        while ((wait(&status)) > 0);

        char *argv[] = {"wget", "-q", "https://drive.google.com/u/0/uc?id=1_djk0z-cx8bgISFsMiUaXzty2cT4VZp1&export=download", "-O", "music.zip", NULL};
        execv("/bin/wget", argv);
        exit(0);
    }
}

void unzip_file(char* dir, char* pass){
    pid_t child_id;
    int status;

    char zip_file[100];
    sprintf(zip_file, "%s.zip", dir);

    child_id = fork();

    if (child_id < 0) {
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if(child_id == 0){
	if(pass == NULL){
            char* argv[] = {"unzip", "-q","-n", zip_file, "-d", dir, NULL};
            execv("/bin/unzip",argv);
            exit(0);
        }
	
	else {
            char* argv[] = {"unzip", "-P", pass, zip_file, NULL};
            execv("/bin/unzip",argv);
            exit(0);
	}
    }
    else while ((wait(&status)) > 0);
}

void make_dir(char* dir){
    pid_t child_id;
    int status;

    child_id = fork();

    if (child_id < 0) {
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if(child_id == 0){
        char *argv[] = {"mkdir", "-p", dir, NULL};
        execv("/bin/mkdir", argv);
        exit(0);
    }
    else while ((wait(&status)) > 0);  
}
```  
  
### B)
Decode semua file .txt yang ada dengan base 64 dan masukkan hasilnya dalam satu file .txt yang baru untuk masing-masing folder (Hasilnya nanti ada dua file .txt) pada saat yang sama dengan menggunakan thread dan dengan nama quote.txt dan music.txt. Masing-masing kalimat dipisahkan dengan newline/enter.  
  
Kita perlu melakukan deklarasi variable-variable yang diperlukan untuk melakukan decode base64.  
   
```c
const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int b64invs[] = { 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58,
	59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5,
	6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
	21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28,
	29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
	43, 44, 45, 46, 47, 48, 49, 50, 51 };

size_t b64_decoded_size(const char *in)
{
	size_t len;
	size_t ret;
	size_t i;

	if (in == NULL)
		return 0;

	len = strlen(in);
	ret = len / 4 * 3;

	for (i=len; i-->0; ) {
		if (in[i] == '=') {
			ret--;
		} else {
			break;
		}
	}

	return ret;
}

void b64_generate_decode_table()
{
	int    inv[80];
	size_t i;

	memset(inv, -1, sizeof(inv));
	for (i=0; i<sizeof(b64chars)-1; i++) {
		inv[b64chars[i]-43] = i;
	}
}

int b64_isvalidchar(char c)
{
	if (c >= '0' && c <= '9')
		return 1;
	if (c >= 'A' && c <= 'Z')
		return 1;
	if (c >= 'a' && c <= 'z')
		return 1;
	if (c == '+' || c == '/' || c == '=')
		return 1;
	return 0;
}

int b64_decode(const char *in, unsigned char *out, size_t outlen)
{
	size_t len;
	size_t i;
	size_t j;
	int    v;

	if (in == NULL || out == NULL)
		return 0;

	len = strlen(in);
	if (outlen < b64_decoded_size(in) || len % 4 != 0)
		return 0;

	for (i=0; i<len; i++) {
		if (!b64_isvalidchar(in[i])) {
			return 0;
		}
	}

	for (i=0, j=0; i<len; i+=4, j+=3) {
		v = b64invs[in[i]-43];
		v = (v << 6) | b64invs[in[i+1]-43];
		v = in[i+2]=='=' ? v << 6 : (v << 6) | b64invs[in[i+2]-43];
		v = in[i+3]=='=' ? v << 6 : (v << 6) | b64invs[in[i+3]-43];

		out[j] = (v >> 16) & 0xFF;
		if (in[i+2] != '=')
			out[j+1] = (v >> 8) & 0xFF;
		if (in[i+3] != '=')
			out[j+2] = v & 0xFF;
	}

	return 1;
}

void *decode(void *file){
    char *filename, buffer[1024], *decoded, loc[20];
    filename = (char *) file;
    size_t len;
    FILE *fptr;
    fptr = fopen(filename, "r");
    fscanf(fptr, "%s", buffer);
    fclose(fptr);

    len = b64_decoded_size(buffer) + 1;
    decoded = (char*) malloc(len*sizeof(char));

    if(b64_decode(buffer, (unsigned char *) decoded, len)){
        decoded[len] = '\0';
        if(filename[0] == 'm') strcpy(loc, "music.txt");
        else if(filename[0] == 'q') strcpy(loc, "quote.txt");
        fptr = fopen(loc, "a");
        fprintf(fptr, "%s\n", decoded);
        fclose(fptr);
    }
}
```  
  
  
### C)  
Pindahkan kedua file .txt yang berisi hasil decoding ke folder yang baru bernama hasil.  
  
Kita dapat melakukan pemindahan file dengan membuka file yang akan dipindah dengan permission `r` serta membuka directory yang akan dituju dengan permission `w`. Setelah itu kita akan menggunakan `while` untuk mengambil semua isi yang hendak dipindah hingga terjadi `EOF`.  
  
```c
void move_file(char* dest, char* src){
    FILE *fptr1, *fptr2;
    char temp;

    fptr1 = fopen(src, "r");
    fptr2 = fopen(dest, "w");

    while(fscanf(fptr1, "%c", &temp) != EOF) fprintf(fptr2, "%c", temp);

    fclose(fptr1);
    fclose(fptr2);
}

move_file("hasil/music.txt", "music.txt");
move_file("hasil/quote.txt", "quote.txt");
```  
  
  
### D)  
Folder hasil di-zip menjadi file hasil.zip dengan password 'mihinomenest[Nama user]'. (contoh password : mihinomenestnovak)  
  
```c
void zip_file(char *dir){
    pid_t child_id;
    int status;

    char zip_file[100];
    sprintf(zip_file, "%s.zip", dir);

    child_id = fork();

    if (child_id < 0) {
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if(child_id == 0){
        char* argv[] = {"zip", "-P","mihinomenestdaniel", "-r", zip_file, dir, NULL};
        execv("/bin/zip",argv);
        exit(0);
    }
    else while ((wait(&status)) > 0);
}

zip_file("hasil");
```  
  
  
### E)  
Karena ada yang kurang, kalian diminta untuk unzip file hasil.zip dan buat file no.txt dengan tulisan 'No' pada saat yang bersamaan, lalu zip kedua file hasil dan file no.txt menjadi hasil.zip, dengan password yang sama seperti sebelumnya.  
  
```c
void unzip_file(char* dir, char* pass){
    pid_t child_id;
    int status;

    char zip_file[100];
    sprintf(zip_file, "%s.zip", dir);

    child_id = fork();

    if (child_id < 0) {
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if(child_id == 0){
	if(pass == NULL){
            char* argv[] = {"unzip", "-q","-n", zip_file, "-d", dir, NULL};
            execv("/bin/unzip",argv);
            exit(0);
        }
	
	else {
            char* argv[] = {"unzip", "-P", pass, zip_file, NULL};
            execv("/bin/unzip",argv);
            exit(0);
	}
    }
    else while ((wait(&status)) > 0);
}

void zip_file(char *dir){
    pid_t child_id;
    int status;

    char zip_file[100];
    sprintf(zip_file, "%s.zip", dir);

    child_id = fork();

    if (child_id < 0) {
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if(child_id == 0){
        char* argv[] = {"zip", "-P","mihinomenestdaniel", "-r", zip_file, dir, NULL};
        execv("/bin/zip",argv);
        exit(0);
    }
    else while ((wait(&status)) > 0);
}

unzip_file("hasil.zip", "mihinomenestdaniel");
create_txt("hasil/no.txt");
zip_file("hasil");
```  
  
## Kendala  
- Tidak ada.  
  
  
# Soal No. 2
Pada soal ini, program akan dibagi menjadi `server.c` dan `client.c` yang masing-masing berada dalam directory `/home/user/Server` dan `/home/user/Client`.
Program diharapkan dapat menjalankan fungsi seperti online judge berbasis client-server. Dalam menyelesaikan soal ini, terdapat beberapa fungsi utilitas yang digunakan
pada client-side (juga digunakan pada server-side dengan sock diganti new_socket):
```c
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
```
`sendInput()` berfungsi mengirim string input antar server-client, `readBuffer()` berfungsi menerima input dari lawan lalu print dengan format `<pengirim>: <isi pesan>`,
`changeInput()` berfungsi mengubah isi array input yang akan digunakan untuk mengirim pesan, `readBufferSp()` mirip seperti `readBuffer()` tetapi menyimpan hasil read
pada array cmd yang selanjutnya digunakan pada kondisional.

## 2A.
## Server-side
```c
    int new_socket = *(int *)tmp;
    printf("A client connected\n");
// Digunakan pada 1G.
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
// Digunakan pada 1G.
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
	isValid=1, isValidRe=0;
	while(isValid || !isValidRe){
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
	    if(!isValid && isValidRe)break; // username belum ditemukan dan kriteria password sesuai
	    strcpy(socketIn,"Data tidak valid atau sudah ada, ulangi proses !\nServer: ");
	    isValid=1, isValidRe=0;	
	}
	changeInput("Register berhasil");
	sendInput(new_socket);
	addUser();
    }
```
### Client-side
```c
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
```
### createTxt() dan findUser()
```c
void createTxt(){
    strcpy(txtPath,dirPath);
    strcat(txtPath,"/users.txt");
    FILE * fp;
    fp = fopen (txtPath, "a+");
    fclose(fp);   
}
void findUser(){
    isValid=0, isValidPass=0, isValidRe=0;
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
	    if(cekzz[0] && cekzz[1] && cekzz[2] && cekzz[3]>=6)isValidRe = 1; // password angka lowercase uppercase
	    if(isValid)break;
    }
    if(checkEmpty==0){//user pertama
	int cekzz[4]={0};
    	for(int i=0;i<strlen(passwordArr);i++){
	    if(passwordArr[i]>=48 && passwordArr[i]<=57)cekzz[0]=1;
	    if(passwordArr[i]>=65 && passwordArr[i]<=90){cekzz[1]=1;cekzz[3]++;}
	    if(passwordArr[i]>=97 && passwordArr[i]<=122){cekzz[2]=1;cekzz[3]++;}
	}
	if(cekzz[0] && cekzz[1] && cekzz[2] && cekzz[3]>=6)isValidRe = 1; // password angka lowercase uppercase
    }
    fclose(fp);   
}
```
Pada bagian soal ini diminta membuat fitur login/register. Pertama-tama ketika menjalankan program akan dibuat file `users.txt` pada server yang digunakan menyimpan
username dan password. Selanjutnya, fitur login/register dibuat dengan melakukan `strcmp()` pada input dari client. Jika memilih register maka client akan memasukkan
username dan password yang diinginkan. Data dianggap valid jika memenuhi kriteria:
- tidak ada username yang sama (diatasi dengan mengecek isi `users.txt`)
- password terdiri dari 6 huruf, lowercase, uppercase, dan angka (diatasi dengan mengecek tiap ASCII dari input password client)
Server akan menolak register sampai username dan password memenuhi syarat, lalu jika sudah akan meng-append string `username:password\n` pada `users.txt`.<br>
Jika memilih login maka client akan memasukkan username dan password, lalu jika ditemukan username dan password yang sesuai pada `users.txt` maka client berhasil login.
Selain itu, data nama user setelah berhasil login/register juga akan tersimpan pada array. Data nama ini akan digunakan pada beberapa poin berikutnya.
## 2B.
```c
void createTsv(){
    strcpy(tsvPath,dirPath);
    strcat(tsvPath,"/files.tsv");
    FILE *fp = fopen(tsvPath, "a+");
    fclose(fp);
}
```
Pada saat menjalankan server akan dibuat file berextension `.tsv` yang digunakan menyimpan judul dan author problem dipisahkan `\t`. Author problem bisa didapat dari
array nama yang tersimpan dari 1A dan formatting `\t` akan dilakukan ketika user melakukan add problem, karena pada kondisi awal file masih kosong.
## 2C.
### Server-side
```c
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
```
### Client-side
```c
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
```
### CheckDir(), addData(), dan saveTo()
```c
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
```
Setelah berhasil login/register, maka akan dilakukan infinite loop untuk menerima perintah dari client sampai client menggunakan perintah `logout`, salah satunya
adalah `add`. Ketika menjalankan perintah `add`, client akan diminta memasukkan judul problem dimana ketika directory dengan nama judul problem tersebut sudah
ada pada server maka client diminta memilih judul problem lain. Lalu client akan diminta memasukkan path description.txt, input.txt, dan output.txt.<br><br>
Agar dapat mengirim ke server, salah satu cara yang dapat dilakukan adalah dengan membuka isi file pada sisi client lalu menyimpan isi file pada buffer dan mengirim
ke server. Selanjutnya server akan menyimpan isi buffer pada directory judul problem sesuai ketentuan. Selain itu server juga akan mengupdate file `.tsv` dengan format
`<judul>\t<author>`
## 2D.
### Server-side
```c
if(strcmp(cmd,"see")==0){
	    seeTsv(new_socket);
	}
```
### Client-side
```c
 if(strcmp(clientIn,"see")==0){
            readBufferSp();
            printf(".tsv list:\n%s",cmd);
 }
```
### seeTsv()
```c
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
```
Client juga dapat melakukan perintah `see` yang menampilkan isi dari file `.tsv` dengan format `<judul> by <author>`. Caranya dengan server membuka file `.tsv` lalu
mengganti delimiter `\t` dengan ` by ` lalu mengirim isinya ke client. Dalam hal ini agar terlihat rapi maka tampilan pada client akan memiliki format:
`Server: .tsv list` diikuti baris berikutnya yaitu `<judul> by <author>`. Jika file `.tsv` kosong maka server juga akan mengirim pesan yang menunjukkan bahwa file
`.tsv` masih kosong.
## 2E.
### Server-side
```c
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
```
### Client-side
```c
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
```
### downloadSoal()
```c
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
```
Selanjutnya, terdapat fitur `download`. Fitur ini terdiri dari 2 kata yaitu download dan <nama problem>. Untuk membuat fitur ini maka Server akan mengirim path
dari directory nama problem yang diinginkan, lalu dari client akan mengambil isi `input.txt` dan `description.txt` dari problem tersebut dan membuat directory
dengan nama yang sama pada client-side.
## 2F.
### Server-side
```c
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
```
### Client-side
```c
else if(strlen(clientIn)>=6 && clientIn[0]=='s' && clientIn[1]=='u' && clientIn[2]=='b' && clientIn[3]=='m' && clientIn[4]=='i' && clientIn[5]=='t'){
                readBuffer();//AC or WA
}
```
### verdict()
```c
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
```
Terdapat juga fitur `submit` yang terdiri dari 3 kata submit, <nama problem>, dan path hasil output client. Cara menyelesaikan poin ini adalah dengan server
menerima nama problem dan path dari hasil output client, lalu membandingkan hasil output tersebut dengan `output.txt` pada nama problem yang tersimpan pada server-side
yang berfungsi seperti kunci jawaban. Jika hasil berbeda maka server akan mengembalikan pesan `WA` dan ketika sama maka server akan mengembalikan pesan `AC`.
## 2G.
### Server-side
```c
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
```
### Client-side
```c
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
}
```
Untuk menyelesaikan poin ini, perlu menggunakan thread sehingga tiap client akan berjalan pada thread yang berbeda. Selain itu diperlukan juga `ctrClient` yang menyimpan
jumlah client yang terhubung. Hal ini bertujuan agar ketika jumlah client lebih dari 1, maka client yang lain akan terjebak dalam loop sampai client pertama melakukan
logout, logout dalam hal ini juga akan otomatis mengurangi nilai `ctrClient`. Nilai `new_socket` pada server akan berbeda untuk tiap client, tujuannya agar pesan tidak tercampur.<br>

## Dokumentasi
### Register User
![Register User](https://snipboard.io/Blfrmk.jpg)
### See Sebelum dan Setelah Add Problem
![See Sebelum dan Setelah Add Problem](https://snipboard.io/wEyS7g.jpg)<br>
*cobasoal adalah directory yang sudah dibuat sebelum server dijalankan untuk mencoba syarat judul problem unique
### Hasil Command Add Pada Server
![Hasil Command Add Pada Server](https://snipboard.io/C7vfxw.jpg)
### Command Download
![Command Download](https://snipboard.io/SaewLq.jpg)
### Hasil Download Pada Client
![Hasil Download Pada Client](https://snipboard.io/MtZyOE.jpg)
### Hasil Submit
![Hasil Submit](https://snipboard.io/bf9yVr.jpg)
### Logout dan Client Lain
![Logout](https://snipboard.io/s2ETRK.jpg)<br>
![Client Lain](https://snipboard.io/cynpWd.jpg)
### Isi users txt dan tsv
![Txt](https://snipboard.io/fJ8MUh.jpg)<br>
![Tsv](https://snipboard.io/F4BLgn.jpg)
### Isi Soal
#### description.txt
![desc](https://snipboard.io/fJ8MUh.jpg)
#### input.txt
![in](https://snipboard.io/dZzlxk.jpg)
#### output.txt (kunci jawaban server-side)
![out](https://snipboard.io/0w2xQN.jpg)
#### outputbenar.txt (client-side)
![outac](https://snipboard.io/djYVbz.jpg)
#### outputsalah.txt (client-side)
![outwa](https://snipboard.io/TU7q4G.jpg)

## Kendala
Tidak ada.

## Soal No. 3
### Nami adalah seorang pengoleksi harta karun handal. Karena Nami memiliki waktu luang, Nami pun mencoba merapikan harta karun yang dimilikinya berdasarkan jenis/tipe/kategori/ekstensi harta karunnya. 

**a,b, dan c) mengkategorikan seluruh file pada directory secara rekursif, pengkategorian dilakukan dengan cara memasukan file ke dalam directory ke ektensi masing masing file (dengan file unknown dimasukan ke dalam directory unkown, dan file hidden dimasukan ke dalam directory hidden). Agar proses kategori berjalan lebih cepat, tiap 1 file yang dikategorikan dioperasikan oleh 1 thread**

```c
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
                        rename(dp->d_name,"client/hartakarun");
                    }
                }
            }
        }
    }
    closedir(d);
}
```
Diatas merupakan program yang dapat dijalankan untuk mengkategorikan file. pertama - tama, kita membuat sebuah directory dummy untuk menyimpan hasil pengkategorian, lalu file directory dummy tersebut akan direname dan menggantikan directory "hartakarun" ketika proses pengkategorian selesai. 
lalu kita buka directory hartakarun, dan iterasi semua file yang ada pada directory dan memanggil fungsi kategoriin. Pada fungsi kategoriin,akan membuat sebuah thread untuk mengkategorikan file.

sebelum program soal3.c dijalankan:
![Screenshot_from_2022-04-17_17-31-22](/uploads/9b4191fca0cfc01d51f11738b07f2839/Screenshot_from_2022-04-17_17-31-22.png)

sesudah program soal3.c dijalankan:
![Screenshot_from_2022-04-17_17-38-11](/uploads/6ba1b85fae9f2a7a4afc8fadb2acfb17/Screenshot_from_2022-04-17_17-38-11.png)

**d dan e)Untuk mengirimkan file ke Cocoyasi Village, nami menggunakan program client-server. Saat program client dijalankan, maka folder /home/[user]/shift3/hartakarun/” akan di-zip terlebih dahulu dengan nama “hartakarun.zip” ke working directory dari program client. dan akan mengirim file ke server ketika command "send hartakarun.zip" dimasukkan**
### client-side 
```c
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
```
saat client dibuka, client akan langsung menjalankan fungsi zip untuk zip file yang ada, lalu connect dengan server, setelah client berhasil terkoneksi dengan server
client akan meminta masukan input, jika inputannya bukan "send hartakarun.zip" maka program akan berhenti, jika inputannya "send hartakarun.zip" maka client akan mengirim file hartakarun.zip ke server

### server-side
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024
#define PORT 8080
void write_file(int sockfd){
  int n;
  FILE *fp;
  char *filename = "./hartakarun.zip";
  char buffer[SIZE];
 
  fp = fopen(filename, "wb");
  while (1) {
    n = recv(sockfd, buffer, SIZE, 0);
    if (n <= 0){
      break;
      return;
    }
    int size_write=fwrite(buffer,sizeof(char),n,fp);
    bzero(buffer, SIZE);
  }
  return;
}
 
int main(){
  char *ip = "127.0.0.1";
  int port = 8080;
  int e;
 
  int sockfd, new_sock;
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;
  char buffer[SIZE];
 
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");
 
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);
 
  e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e < 0) {
    perror("[-]Error in bind");
    exit(1);
  }
  printf("[+]Binding successfull.\n");
 
  if(listen(sockfd, 10) == 0){
 printf("[+]Listening....\n");
 }else{
 perror("[-]Error in listening");
    exit(1);
 }
 
  addr_size = sizeof(new_addr);
  new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
  write_file(new_sock);
  printf("[+]Data written in the file successfully.\n");
 
  return 0;
}
```
Ketika server sudah terkoneksi dengan client, maka server akan menunggu file yang akan dikirim dari client


sebelum client dijalankan:
![Screenshot_from_2022-04-17_17-43-03](/uploads/6a7dd72d06cbabdf26c5c68bba28bc94/Screenshot_from_2022-04-17_17-43-03.png)

setelah client dijalankan:
![Screenshot_from_2022-04-17_17-40-47](/uploads/31fa2caf37520563ec9571f86e44a869/Screenshot_from_2022-04-17_17-40-47.png)

setelah client diberikan command "send hartakarun.zip":
![Screenshot_from_2022-04-17_17-40-58](/uploads/9590c2eb2b99f5b1a0bf3622ccfeb695/Screenshot_from_2022-04-17_17-40-58.png)

setelah server menerima file:
![Screenshot_from_2022-04-17_17-41-05](/uploads/d0dd35027ad16ae16099642f4d7b6af0/Screenshot_from_2022-04-17_17-41-05.png)

![Screenshot_from_2022-04-17_17-41-12](/uploads/499eacd8aba22f64ea2c94ea262cd21e/Screenshot_from_2022-04-17_17-41-12.png)

![Screenshot_from_2022-04-17_17-41-16](/uploads/959f08406404e4606b4aa496da71f183/Screenshot_from_2022-04-17_17-41-16.png)
