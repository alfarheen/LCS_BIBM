#ifndef PSTRING_C
#define PSTRING_C

#define TERMINAL 2
#define CONSTANT 1
#define PARAMETER 0
#define DISTANCE 0
#define NO_PARAM '-'
#define ALL_PARAM '*'

int prev(int pv[],char typespv[],char buf[],short type[],int size);
int remapPrevAlpha(int pv[],char pvtypes[],int max,int pvsize,int remappv[],int remappv_size);
int mapfun(int pvi,char typei,int max,int remap[]);
void forw(char T[],int fw[],int pv[],int pvsize,short type[]);
void setAlphaType(short alg_type,char *parameters,short type[]);
int getAlphaFreq(char *buf,int size,int freq[],int remap[],short adjust_buf);


void forw(char T[],int fw[],int pv[],int pvsize,short type[])
{	
	char *trev=NULL;
	int *fw2=NULL;
	
	trev=(char *)malloc(pvsize*(sizeof(char)));
	fw2=(int *)malloc(pvsize*(sizeof(int)));
	if(trev && fw2)
	{
		//forw(T)=rev(repl(prev(rev(T)),0,n))
		rev2(T,trev,pvsize);
		prev(fw2,NULL,trev,type,pvsize);
		repl(fw2,pvsize,0,pvsize);
		rev(fw2,fw,pvsize);
	}
	#if DEBUG>0
		{ printf("forw(T)="); printBuf2(fw,pvsize); }
	#endif
	
	free(fw2);free(trev);
}

int mapfun(int pvi,char typei,int max,int remap[])
{
	if(typei==DISTANCE)
		return pvi;
	else if(typei==CONSTANT)
		return pvi+max;
	return 0;
}

int remapPrevAlpha(int pv[],char pvtypes[],int max,int pvsize,int remappv[],int remappv_size)
{
	int i,j=0;
	
	for(i=0; i<remappv_size; i++) remappv[i]=j;
	j=j+2;
	for(i=0; i<pvsize; i++)
	{
		remappv[mapfun(pv[i],pvtypes[i],max,remappv)]=j;
	}
	for(i=0; i<remappv_size; i++)
	{
		if(remappv[i]>0)
			remappv[i]=j++;
	}
	
	#if DEBUG>0 
		{ printf("max=%d, #symbols=%d, remap: \n$) 1\n",max,j-1); for(i=0; i<remappv_size; i++) if(remappv[i]>0) printf("%d) %d\n",i,remappv[i]); printf("\n");  }
	#endif
	
	return j-1;
}

void setAlphaType(short alg_type,char *parameters,short type[])
{
	int i;
	
	for(i=0; i<MAP_SIZE; i++) type[i]=CONSTANT;
	if(parameters != NULL && strlen(parameters)>0)
	{
		if(parameters[0]==ALL_PARAM)
		{
			for(i=0; i<MAP_SIZE; i++) type[i]=PARAMETER;
		}
		else
		{
			for(i=0; i<strlen(parameters); i++)
				type[parameters[i]]=PARAMETER;
		}
	}
	if(alg_type==PSA_FOR_PLRF_AND_PLF || alg_type==PLRF || alg_type==PLF) type[FREE_CONST]=CONSTANT;
	
	#if DEBUG>0 
		{ printf("parameters: ");for(i=0; i<MAP_SIZE; i++) if(type[i]==PARAMETER) printf("[%d]",i); printf("\n"); }
	#endif
}

int prev(int pv[],char typespv[],char buf[],short type[],int size)
{
	int i;
	int loc[MAP_SIZE];
	int max=0;
	
	for(i=0; i<MAP_SIZE; i++) loc[i]=-1;

	for(i=0; i<size; i++)
	{
		if(type[buf[i]]==CONSTANT) 
		{
			pv[i]=buf[i];
			if(typespv!=NULL) typespv[i]=CONSTANT;
		}
		else if(type[buf[i]]==PARAMETER){
			if(loc[buf[i]]<0)
				pv[i]=0;
			else
			{
				pv[i]=i-loc[buf[i]];
				if(pv[i]>max) max=pv[i];
			}
			loc[buf[i]]=i;
			if(typespv!=NULL) typespv[i]=DISTANCE;
		}
	}
	#if DEBUG>0
		{ printf("prev(T)="); printBuf2(pv,size); printf("\n"); if(typespv!=NULL) { printf("pv types="); printBuf(typespv,size); } printf("\nmax=%d\n\n",max);}
	#endif
	return max;
}

int getAlphaFreq(char *buf,int size,int freq[],int remap[],short adjust_buf)
{
	int i,j=1;
	for(i=0; i<MAP_SIZE; i++) freq[i]=remap[i]=0;
		
	for(i=0; i<size; i++) freq[buf[i]]++;
	
	for(i=0; i<MAP_SIZE; i++)
	{
		if(freq[i]>0)
			remap[i]=j++;
	}
	if(adjust_buf==YES) for(i=0; i<size; i++) buf[i]=remap[buf[i]];
	
	#if DEBUG>0
	{ 
		printf("freq: \n"); for(i=0; i<MAP_SIZE; i++) if(freq[i]>0) printf("%d) %d\n",i,freq[i]); printf("\n"); 
		printf("remap: \n"); for(i=0; i<MAP_SIZE; i++) if(remap[i]>0) printf("%d) %d\n",i,remap[i]); printf("\n"); 
		if(adjust_buf==YES) { printf("remapped T="); printBuf(buf,size); }
	}
	#endif
	
	return j-1;
}

#endif
