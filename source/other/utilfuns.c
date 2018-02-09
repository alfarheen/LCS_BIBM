#ifndef UTILFUNS_C
#define UTILFUNS_C

#include <sys/time.h>

void repl(int arr[],int len,int repl_what,int repl_with);
void rev(int arr_from[],int arr_into[],int len);
void rev2(char arr_from[],char arr_into[],int len);
long double pow_int(long double x, int y);
long double get_current_microseconds();
long double mean(int size,int arr[]);
int max(int size,int arr[]);
void get_nonneg_num_as_str(int num, char buf[]);
void get_CORRECT_text_art(char buf[]);
void write_WRONG_text_art(FILE* out);

long double get_current_microseconds()
{
	struct timeval tv;
	struct timezone tz;
	struct tm *tm;
	gettimeofday(&tv, &tz);
	tm=localtime(&tv.tv_sec);
	//printf("%d:%02d:%02d %d \n", tm->tm_hour, tm->tm_min,tm->tm_sec, tv.tv_usec);
	return (long double)((3600*tm->tm_hour+60*tm->tm_min+tm->tm_sec)*1000000+tv.tv_usec);
}

long double pow_int(long double x, int y)
{
	long double z=1;
	int i;
	if(y<0) return 0;
	for(i=0; i<y; i++) z=z*x;
	return z;
}

long double mean(int size,int arr[])
{
	long double m=0;
	int i;
	
	for(i=0; i<size; i++)
		m+=arr[i];
	if(size>0) m/=size;
	
	return m;
}

int max(int size,int arr[])
{
	int m=0;
	int i;
	
	for(i=0; i<size; i++)
	{
		if(arr[i]>m)
			m=arr[i];
	}
	
	return m;
}

void repl(int arr[],int len,int repl_what,int repl_with)
{
	int i;
	for(i=0; i<len; i++)
	{
		if(arr[i]==repl_what)
			arr[i]=repl_with;
	}
}

void rev(int arr_from[],int arr_into[],int len)
{
	int i,j=len-1;
	for(i=0; i<len; i++)
		arr_into[j--]=arr_from[i];
}

void rev2(char arr_from[],char arr_into[],int len)
{
	int i,j=len-1;
	for(i=0; i<len; i++)
		arr_into[j--]=arr_from[i];
}

// returns num as string in buf if num>=0, otherwise, returns N/A in buf
void get_nonneg_num_as_str(int num, char buf[]){
	sprintf(buf,"N/A");
	if(num >= 0) sprintf(buf,"%d", num);
}

void write_CORRECT_text_art(FILE* out){
    fprintf(out, "_________________________________________________\n");
    fprintf(out, "|     ____ ___  ____  ____  _____ ____ _____    |\n");
    fprintf(out, "|    / ___/ _ \\|  _ \\|  _ \\| ____/ ___|_   _|   |\n");
    fprintf(out, "|   | |  | | | | |_) | |_) |  _|| |     | |     |\n" );
    fprintf(out, "|   | |__| |_| |  _ <|  _ <| |__| |___  | |     |\n");
    fprintf(out, "|    \\____\\___/|_| \\_\\_| \\_\\_____\\____| |_|     |\n");
    fprintf(out, "|_______________________________________________|\n\n");
    fflush(out);
}

void write_WRONG_text_art(FILE* out){
    fprintf(out," ________________________________________________________________________________\n");
    fprintf(out,"|                      __        ______   ___  _   _  ____                       |\n");
    fprintf(out,"|   __/\\____/\\____/\\__ \\ \\      / /  _ \\ / _ \\| \\ | |/ ___| __/\\____/\\____/\\__   |\n");
    fprintf(out,"|   \\    /\\    /\\    /  \\ \\ /\\ / /| |_) | | | |  \\| | |  _  \\    /\\    /\\    /   |\n");
    fprintf(out,"|   /_  _\\/_  _\\/_  _\\   \\ V  V / |  _ <| |_| | |\\  | |_| | /_  _\\/_  _\\/_  _\\   |\n");
    fprintf(out,"|     \\/    \\/    \\/      \\_/\\_/  |_| \\_\\\\___/|_| \\_|\\____|   \\/    \\/    \\/     |\n");
    fprintf(out,"|________________________________________________________________________________|\n");
    fflush(out);
}

#endif
