#include <stdio.h>
#include "ssarray.c"

int main(void)
{
	long i,SIZE=6;
	long x[]={1,3,2,1,1,0};
	
	if(ssarray(x)<0) printf("FAILED\n\n");;
	
	for(i=0; i<SIZE; i++)
		printf("%d\n",x[i]);
}
