
#ifndef FILEFUNS_C
#define FILEFUNS_C

#define FAILED1 1
#define SUCCESS1 0
#define RETRY_FILE_OPEN 2

void write_char(FILE* out, char ch);
void write_int_or_int(FILE* out, int i,short will_write_int);
int read_char_arr(const char *fn,char arr[],int arr_len);
void write_char_arr(char *fn, char arr[], int size);
void no_ending_cr(char arr[], int* size);
char read_next_char(FILE* in);
char read_next_char2(FILE* in);
short read_int_arr(const char *fn,int arr[],int arr_len);
short read_int_arr_skip(const char *fn,int arr[],int first_skip_this_many_ints,int then_read_this_many_ints);
//void read_int_arr(char *fn,int arr[],int arr_len);
int get_file_len(FILE *file);
int get_file_len2(char *name);
void readFile(char *name,char *buf,int size);
void printBuf(char buf[],int size);
void printBuf2(int buf[],int size);
void display_int_arr(FILE* out,int arr[],int n);
int read_next_int(FILE* in);
void make_fn(char* path, char* name, char* construct_here);

void make_fn(char* path, char* name, char* construct_here){
	char sep = '/';
	int path_len = strlen(path);
	if(path_len > 0 && path[path_len-1] == sep) sprintf(construct_here,"%s%s",path,name);
	else sprintf(construct_here,"%s%c%s",path,sep,name);
}

void write_char_arr(char *fn, char arr[], int size){
  FILE* fout = fopen(fn,"w");
  int i;
  for(i=0; i < size; i++) write_char(fout,arr[i]);
  fclose(fout);
}

void readFile(char *name,char *buf,int size)
{
	FILE *file;
	int try1=1;
	
	do
	{
		file=fopen(name,"rb");
		if(!file)
		{
			//fprintf(stderr, "!!! dup: Attempt %d: problem opening file \"%s\" !!!\n", try1, name);
			if(try1==RETRY_FILE_OPEN) 
			{
				//fprintf(stderr, "!!! dup: Could not open file \"%s\" !!!\n", name);
				return;
			}
		}
		//if(file && try1>1) fprintf(stderr, "!!! Successfully opened file \"%s\" !!!\n", name);
		try1++;
	}while(!file && try1<=RETRY_FILE_OPEN);
	
	fread(buf,sizeof(char),size,file);
	fclose(file);
	
	#if DEBUG>0 
		{ printf("T="); printBuf(buf,size); printf("\n"); }
	#endif
}


int read_char_arr(const char *fn,char arr[],int arr_len){
	FILE* in=fopen(fn,"rb");
	int read_num = fread(arr,sizeof(char),arr_len,in);
	//printf("\n[%s]\n",arr);
	fclose(in);
	return read_num;
}

void no_ending_cr(char arr[], int* size){
	if(arr[(*size)-1] == '\n') arr[--(*size)] = '\0';
}

short read_int_arr(const char *fn,int arr[],int arr_len){
	FILE* in=fopen(fn,"rb");
	int num;
	if(!fn || arr_len<=0)
	{
		printf("*** Failed to read from %s ***\n\n",fn);
		return FAILED1;
	}//printf("arr_len%d \n",arr_len);
	//int i; for(i=0; i<arr_len; i++) arr[i]=0;
	num=fread(arr,sizeof(int),arr_len,in);
	if(num != arr_len){
		printf("*** FREAD ERROR return_val=%d, expected=%d***\n\n",num,arr_len);
		return FAILED1;
	}
//printf("HERE\n");
	//int i; for(i=0; i<arr_len; i++) printf("\n[%d]\n",arr[i]);
	fclose(in);
//printf("HERE\n");
	return SUCCESS1;
}

short read_int_arr_skip(const char *fn,int arr[],int first_skip_this_many_ints,int then_read_this_many_ints){
	int i;
	FILE* in=fopen(fn,"rb");
	if(!fn || first_skip_this_many_ints<=0 || then_read_this_many_ints<=0)
	{
		printf("*** Failed to read from %s ***\n\n",fn);
		return FAILED1;
	}
	for(i=0; i<first_skip_this_many_ints; i++) read_next_int(in);
	for(i=0; i<then_read_this_many_ints; i++) arr[i]=read_next_int(in);
	//int i; for(i=0; i<arr_len; i++) printf("\n[%d]\n",arr[i]);
	fclose(in);
	return SUCCESS1;
}

int read_next_int(FILE* in){
	int arr[]={0};
	fread(arr,sizeof(int),1,in);
	return arr[0];
}

char read_next_char(FILE* in){
	char arr[]={0};
	fread(arr,sizeof(char),1,in);
	return arr[0];
}

char read_next_char2(FILE* in){
	return fgetc(in);
}

/*void read_int_arr(char *fn,int arr[],int arr_len){
	FILE* in=fopen(fn,"rb");
	int* temp=(int *)malloc((arr_len)*(sizeof(int)));
	
	fread(temp,sizeof(int),arr_len,in);
	int i; 
	for(i=0; i<arr_len; i++) arr[i]=(int)temp[i];
	
	free(temp);
	fclose(in);
}*/

int get_file_len(FILE *file){
	int fbytes=0;
	if(file) //get size of file
	{
		fseek(file,0,SEEK_END);
		fbytes=ftell(file);
	}	
	return fbytes;
}

int get_file_len2(char *name)
{
	FILE *file;
	file=fopen(name,"rb");
	if(!file)
	{
		return -1;
	}
	int len=get_file_len(file);
	fclose(file);
	return len;
}

void write_char(FILE* out, char ch){
	char tmp[]={ch};
	fwrite(tmp,sizeof(char),1,out);
}

// when I made everything an INT, this function suffered; used to be LONG_OR_INT!!!!!
void write_int_or_int(FILE* out, int i,short will_write_int){
	if(will_write_int!=0){
		int tmp[]={i};
		fwrite(tmp,sizeof(int),1,out);
	}else{
		int tmp[]={(int)i};
		fwrite(tmp,sizeof(int),1,out);
	}
}

void printBuf(char buf[],int size)
{
	int i;
	for(i=0; i<size; i++) printf("[%d]",buf[i]); printf("\n\n");
}

void printBuf2(int buf[],int size)
{
	int i;
	for(i=0; i<size; i++) printf("[%d]",buf[i]); printf("\n\n");
}

void display_int_arr(FILE* out,int arr[],int n)
{
	int i;
	if(arr!=NULL)
	{
		for(i=0; i<n; i++)
			write_int_or_int(out,(int)arr[i],1);
	}
	fprintf(out,"\n");
}

#endif
