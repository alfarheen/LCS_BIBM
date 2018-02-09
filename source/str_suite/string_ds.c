#ifndef STRINGDS_C
#define STRINGDS_C

#include "../other/doublyLinkedList.h"
#include "../other/filefuns.c"

struct LPF_data {
	int ind;
	int len;
	int locs_of_len[BUF_SIZE];
	int num_locs_of_len;
};

void print_LPF_data(struct LPF_data ld);
struct LPF_data get_LPF_data_NAIVE(char* T,int i,int size);
void compute_LCP_Kasai(char T[],int LCP[],int SA[],int R[],int size);
void compute_LPF_direct_Crochemore(char* T,int* T_int,int LPF[],int prev_lt[],int prev_gt[],int SA[],int size,char* output_lpf_filename,char* output_deciding_arr_fn);
void compute_LPF_direct_Crochemore__LIGHT(char* textfn,char* SAfn,int size,char* output_lpf_filename,char* output_deciding_arr_fn);
void construct_rank_array(int R[],int SA[],int n);
void test_preprocess();
void preprocess_SA_arrays(int out_arr[],short type,int SA[],int n);
void preprocess_SA_arrays__LIGHT(int* arr,int ALGORITHM,int* SA,int size);
long preprocess_BEFORE_arrays__LIGHT(int* arr,int ALGORITHM,char* SAfn,int size,char* tempfn);
int is_correct_LPF_NAIVE(char* T,int LPFi,int i,int size);
int match(char* T, int size, int i, int j);
int substit_for_eq(char s1[],char s2[]);


void preprocess_SA_arrays(int out_arr[],short type,int SA[],int n)
{
	int i, j=0, h=n-1, c=NOT_EXIST, s=0, e=h;
	doublyLinkedList<int> x;
	nodeType<int>** ptr=NULL;

	if(n==NOT_EXIST)
	{
		printf("*** process_SA_arrays: n==NOT_EXIST problem ***\n\n");
		return;
	}
	if(type==REV_LT || type==REV_GT)
	{
		h=2*n+1; // reset h to account for new SA, SA_{prev(T)$1.rev(prev(T))$2}
		
		if(type==REV_GT)
		{
			s=h;
			e=0;
			for(i=s; i>=e; i--)
			{
				if(SA[i]>=n)
					c=SA[i]-n-1;
				else
				{
					out_arr[SA[i]]=c;
					if(out_arr[SA[i]]<0)
						out_arr[SA[i]]=NOT_EXIST;
				}
				if(i==e) break;
			}
		}
		else
		{
			s=0;
			e=h;
			for(i=s; i<=e; i++)
			{
				if(SA[i]>=n)
					c=SA[i]-n-1;
				else
					out_arr[SA[i]]=c;
				if(out_arr[SA[i]]<0)
					out_arr[SA[i]]=NOT_EXIST;
			}
		}
	}
	else
	{	
		ptr=(nodeType<int>**)malloc((n)*(sizeof(nodeType<int>*)));
		if(!ptr)
		{
			free(ptr);
			return;
		}
		
		x.insert(INSERT_END,NOT_EXIST); // *** implementation: max to 2nd param to keep range of int
		for(i=0; i<=h; i++)
		{
			ptr[SA[i]]=x.insert(INSERT_END,SA[i]);
		}
		x.insert(INSERT_END,NOT_EXIST);
		for(i=h; i>=0; i--)
		{
			switch(type)
			{
				case BEFORE_LT:
					out_arr[i]=ptr[i]->back->info;
					x.delete_nodeType(DELETE_FORCE,ptr[i]);
					break;
				case BEFORE_GT:
					out_arr[i]=ptr[i]->next->info;
					x.delete_nodeType(DELETE_FORCE,ptr[i]);
					break;
				case AFTER_LT:
					out_arr[j]=ptr[j]->back->info;
					x.delete_nodeType(DELETE_FORCE,ptr[j++]);
					break;
				case AFTER_GT:
					out_arr[j]=ptr[j]->next->info;
					x.delete_nodeType(DELETE_FORCE,ptr[j++]);
					break;
				case NEQ_LT:
					out_arr[i]=ptr[i]->back->info;
					break;
				case NEQ_GT:
					out_arr[i]=ptr[i]->next->info;
					break;
			}
			if(i==0) break; // implementation issue since no negative value with int
		}
	}
	x.destroy();
	free(ptr);
}


#define PREPROCESS_TEST_SIZE 8
void test_preprocess()
{
	//int SA[SIZE]={6,5,4,3,2,1,7,0};
	/*
		SA

			(0:6)(1:5)(2:4)(3:3)(4:2)(5:1)(6:7)(7:0)
		before>
			(0:-1)(1:0)(2:1)(3:2)(4:3)(5:4)(6:5)(7:0)
		before<

			(0:-1)(1:-1)(2:-1)(3:-1)(4:-1)(5:-1)(6:-1)(7:1)
		after>
			(0:-1)(1:7)(2:7)(3:7)(4:7)(5:7)(6:7)(7:-1)
		after<

			(0:7)(1:2)(2:3)(3:4)(4:5)(5:6)(6:-1)(7:-1)
		neq>
			(0:-1)(1:7)(2:1)(3:2)(4:3)(5:4)(6:5)(7:0)
		neq<

			(0:7)(1:2)(2:3)(3:4)(4:5)(5:6)(6:-1)(7:1)
		rev>
			(0:-1)(1:3)(2:3)(3:3)
		rev<

			(0:3)(1:0)(2:0)(3:0)
	*/
	
	int SA[PREPROCESS_TEST_SIZE]={5,6,2,3,0,1,4,7};
	/*

		SA
			(0:5)(1:6)(2:2)(3:3)(4:0)(5:1)(6:4)(7:7)
		before>
			(0:-1)(1:-1)(2:0)(3:0)(4:-1)(5:2)(6:2)(7:-1)

		before<
			(0:-1)(1:0)(2:-1)(3:2)(4:1)(5:-1)(6:5)(7:4)
		after>
			(0:1)(1:4)(2:3)(3:4)(4:7)(5:6)(6:7)(7:-1)

		after<
			(0:3)(1:3)(2:6)(3:6)(4:6)(5:-1)(6:-1)(7:-1)
		neq>
			(0:1)(1:4)(2:3)(3:0)(4:7)(5:6)(6:2)(7:-1)

		neq<
			(0:3)(1:0)(2:6)(3:2)(4:1)(5:-1)(6:5)(7:4)
		rev>
			(0:0)(1:0)(2:0)(3:0)

		rev<
			(0:2)(1:2)(2:2)(3:2)
	*/
	
	int *q=(int*)malloc((PREPROCESS_TEST_SIZE)*(sizeof(int)));
	
	if(!q) return;
	
	printf("SA\n");
	display_int_arr(stdout,SA,PREPROCESS_TEST_SIZE);

	printf("before>\n");
	preprocess_SA_arrays(q,BEFORE_GT,SA,PREPROCESS_TEST_SIZE);
	display_int_arr(stdout,q,PREPROCESS_TEST_SIZE);

	printf("before<\n");
	preprocess_SA_arrays(q,BEFORE_LT,SA,PREPROCESS_TEST_SIZE);
	display_int_arr(stdout,q,PREPROCESS_TEST_SIZE);

	printf("after>\n");
	preprocess_SA_arrays(q,AFTER_GT,SA,PREPROCESS_TEST_SIZE);
	display_int_arr(stdout,q,PREPROCESS_TEST_SIZE);

	printf("after<\n");
	preprocess_SA_arrays(q,AFTER_LT,SA,PREPROCESS_TEST_SIZE);
	display_int_arr(stdout,q,PREPROCESS_TEST_SIZE);

	printf("neq>\n");
	preprocess_SA_arrays(q,NEQ_GT,SA,PREPROCESS_TEST_SIZE);
	display_int_arr(stdout,q,PREPROCESS_TEST_SIZE);

	printf("neq<\n");
	preprocess_SA_arrays(q,NEQ_LT,SA,PREPROCESS_TEST_SIZE);
	display_int_arr(stdout,q,PREPROCESS_TEST_SIZE);

	printf("rev>\n");
	preprocess_SA_arrays(q,REV_GT,SA,PREPROCESS_TEST_SIZE/2);
	display_int_arr(stdout,q,PREPROCESS_TEST_SIZE/2);

	printf("rev<\n");
	preprocess_SA_arrays(q,REV_LT,SA,PREPROCESS_TEST_SIZE/2);
	display_int_arr(stdout,q,PREPROCESS_TEST_SIZE/2);

	free(q);
}

void construct_rank_array(int R[],int SA[],int n)
{
	int i;
	for(i=0; i<n; i++)
		R[SA[i]]=i;
}

// use whichever text (T or T_int) that is not NULL
void compute_LPF_direct_Crochemore(char* T,int* T_int,int LPF[],int prev_lt[],int prev_gt[],int SA[],int size,char* output_lpf_filename,char* output_deciding_arr_fn)
{
	FILE* deciding_arr_out=fopen(output_deciding_arr_fn,"wb");
	FILE* lpf_out=fopen(output_lpf_filename,"wb");
	int* lpf_lt=(int *)malloc((size)*(sizeof(int)));
	int* lpf_gt=(int *)malloc((size)*(sizeof(int)));
	int i,j,k;
	

//printf("\n\n!!!!!!!!!!!!!!!!!!!!!!!!!! compute_LPF  T=NULL?%d  T_int=NULL?%d   size=%d\n\n",(T==NULL),(T_int==NULL),size);
	preprocess_SA_arrays(prev_lt,BEFORE_LT,SA,size);
	preprocess_SA_arrays(prev_gt,BEFORE_GT,SA,size);
	
	LPF[0]=lpf_lt[0]=lpf_gt[0]=0;
	
	write_int_or_int(deciding_arr_out,(int)NOT_EXIST,1);

	write_int_or_int(lpf_out,(int)LPF[0],1);
	
	for(i=1; i<size; i++)
	{
		if(lpf_lt[i-1]-1>0) j=lpf_lt[i-1]-1;
		else j=0;
		if(lpf_gt[i-1]-1>0) k=lpf_gt[i-1]-1;
		else k=0;
		
		if(prev_lt[i]==NOT_EXIST) lpf_lt[i]=0;
		else
		{
			while(i+j<size && ((T != NULL && T[i+j]==T[prev_lt[i]+j]) || (T_int != NULL && T_int[i+j]==T_int[prev_lt[i]+j]))) j++;
			lpf_lt[i]=j;
		}
		if(prev_gt[i]==NOT_EXIST) lpf_gt[i]=0;
		else
		{
			while(i+k<size && ((T != NULL && T[i+k]==T[prev_gt[i]+k]) || (T_int != NULL && T_int[i+k]==T_int[prev_gt[i]+k]))) k++;
			lpf_gt[i]=k;
		}
		if(lpf_lt[i]>lpf_gt[i]) {
			LPF[i]=lpf_lt[i];
			if(LPF[i]==0) write_int_or_int(deciding_arr_out,(int)NOT_EXIST,1);
			else write_int_or_int(deciding_arr_out,(int)prev_lt[i],1);
		}else{
			LPF[i]=lpf_gt[i];
			if(LPF[i]==0) write_int_or_int(deciding_arr_out,(int)NOT_EXIST,1);
			else write_int_or_int(deciding_arr_out,(int)prev_gt[i],1);
		}write_int_or_int(lpf_out,(int)LPF[i],1);
	}

	free(lpf_lt);free(lpf_gt);
	fclose(deciding_arr_out);
	fclose(lpf_out);
}

void compute_LCP_Kasai(char T[],int LCP[],int SA[],int R[],int size)
{
	int h=0,i,j;
	LCP[0]=0;
	for(i=0; i<size; i++)
	{
		if(R[i]>0){
			j=SA[R[i]-1];
			while(T[i+h]==T[j+h]) h++;
			LCP[R[i]]=h;
			if(h>0) h--;
		}
	}
}

#define DEBUG_compute_LPF_direct_Crochemore__LIGHT 0 // 0=none, 1=just check match, 2=also show before<,before>
// when not debugging, uses 8*size bytes in RAM and 5*4*size bytes in external memory
void compute_LPF_direct_Crochemore__LIGHT(char* textfn,char* SAfn,int size,char* output_lpf_filename,char* output_deciding_arr_fn)
{
	char PREVLT_FN[]=TEMP_ARR_FN;
	char PREVGT_FN[]=TEMP_ARR_FN2;
	FILE* temp_out=NULL;
	FILE* prevlt_in=NULL;
	FILE* prevgt_in=NULL;
	FILE* deciding_arr_out=fopen(output_deciding_arr_fn,"wb");
	FILE* lpf_out=fopen(output_lpf_filename,"wb");
	int i,j,k,lpfi,lpflt,lpfgt,prevlt,prevgt;
	char* T=NULL;
	int* prev_lt=NULL;
	int* prev_gt=NULL;
	long moves;

	/////////////////////////////////
	#if DEBUG_compute_LPF_direct_Crochemore__LIGHT>1
		int* SA=NULL;
		printf(": reading length-%d SA from %s\n\n",size,SAfn);
		SA=(int *)malloc((size)*(sizeof(int)));							
		read_int_arr(SAfn,SA,size);
	#endif
	/////////////////////////////////

	printf("\nMaking prev< ...\n");
	prev_lt=(int *)malloc((size)*(sizeof(int)));
	moves=preprocess_BEFORE_arrays__LIGHT(prev_lt,BEFORE_LT,SAfn,size,PREVLT_FN);
	printf("\n# of moves were %ld\n",moves);
	
	//////////////////////////////////
	#if DEBUG_compute_LPF_direct_Crochemore__LIGHT>1
		printBuf2(prev_lt,size);
		printf("\nActual prev<\n");	
		preprocess_SA_arrays(prev_lt,BEFORE_LT,SA,size);
		printBuf2(prev_lt,size);
	#endif
	//////////////////////////////////

	free(prev_lt);

	printf("\nMaking prev> ...\n");
	prev_gt=(int *)malloc((size)*(sizeof(int)));
	moves=preprocess_BEFORE_arrays__LIGHT(prev_gt,BEFORE_GT,SAfn,size,PREVGT_FN);
	printf("\n# of moves were %ld\n",moves);
		
	/////////////////////////////////
	#if DEBUG_compute_LPF_direct_Crochemore__LIGHT>1
		printBuf2(prev_gt,size);
		printf("\nActual prev>\n");
		preprocess_SA_arrays(prev_gt,BEFORE_GT,SA,size);
		printBuf2(prev_gt,size);
		free(SA);	
	#endif
	/////////////////////////////////

	free(prev_gt);

	printf("\nReading T ...\n");
	T=(char *)malloc((size+1)*(sizeof(char)));
	readFile(textfn,T,size);
	T[size]=0;
	
	lpflt=lpfgt=0;
		
	write_int_or_int(deciding_arr_out,(int)NOT_EXIST,1);
	write_int_or_int(lpf_out,(int)0,1);
		
	prevlt_in=fopen(PREVLT_FN,"rb");
	read_next_int(prevlt_in); // don't need prev_lt[0]
	prevgt_in=fopen(PREVGT_FN,"rb");
	read_next_int(prevgt_in); // don't need prev_gt[0]

	printf("\nMaking LPF ...\n");
	for(i=1; i<size; i++)
	{
		if(i%50000000==0) printf("i=%d, size=%d\n",i,size);

		if(lpflt-1>0) j=lpflt-1;
		else j=0;
		if(lpfgt-1>0) k=lpfgt-1;
		else k=0;
		
		prevlt=read_next_int(prevlt_in);//prev_lt[i];
		prevgt=read_next_int(prevgt_in);//prev_gt[i];
		if(prevlt==NOT_EXIST) lpflt=0;
		else
		{
			while(i+j<size && T[i+j]==T[prevlt+j]) j++;
			lpflt=j;
		}
		if(prevgt==NOT_EXIST) lpfgt=0;
		else
		{
			while(i+k<size && T[i+k]==T[prevgt+k]) k++;
			lpfgt=k;
		}
		if(lpflt>lpfgt) {
			lpfi=lpflt;
			if(lpfi==0) write_int_or_int(deciding_arr_out,(int)NOT_EXIST,1);
			else write_int_or_int(deciding_arr_out,(int)prevlt,1);
		}else{
			lpfi=lpfgt;
			if(lpfi==0) write_int_or_int(deciding_arr_out,(int)NOT_EXIST,1);
			else write_int_or_int(deciding_arr_out,(int)prevgt,1);
		}write_int_or_int(lpf_out,(int)lpfi,1);

		///////////////////////////////////////////
		#if DEBUG_compute_LPF_direct_Crochemore__LIGHT>0
			is_correct_LPF_NAIVE(T,lpfi,i,size);
		#endif
		///////////////////////////////////////////
	}
	fclose(prevlt_in);
	fclose(prevgt_in);

	printf("\nMade LPF!\n");

	printf("\nClearing temp files\n");
	temp_out=fopen(PREVLT_FN,"wb");
	fclose(temp_out);
	temp_out=fopen(PREVGT_FN,"wb");
	fclose(temp_out);

	free(T);/*free(prev_lt);free(prev_gt);*/
	fclose(deciding_arr_out);
	fclose(lpf_out);
}

long preprocess_BEFORE_arrays__LIGHT(int* arr,int ALGORITHM,char* SAfn,int size,char* tempfn){
	FILE* temp_out=NULL;
	FILE* temp_in=NULL;
	int i,j,val,curr,saval,readval;
	long moves=0L;
	int* SA=NULL;

	printf(": reading length-%d SA from %s\n\n",size,SAfn);
	SA=(int *)malloc((size)*(sizeof(int)));							
	read_int_arr(SAfn,SA,size);

	//for(i=0; i<size; i++) arr[i]=NOT_EXIST;

	if(ALGORITHM==BEFORE_LT) i=0;
	else if(ALGORITHM==BEFORE_GT) i=size-1;

	for(;;){
		if(i<0 || i>=size) break;

		if(i%50000000==0) printf("i=%d, size=%d, moves=%ld\n",i,size,moves);
		curr=SA[i];
		val=NOT_EXIST;

		if(ALGORITHM==BEFORE_LT) j=i-1;
		else if(ALGORITHM==BEFORE_GT) j=i+1;
		
		for(;;){
			if(j<0 || j>=size) break;

			moves++;
			
			//printf("\ni=%d,j=%d,size=%d\n",i,j,size);

			// clear and easy!
			if(curr>SA[j]){
				val=SA[j];
				//printf("For SA[i]=%d, found prev SA[j]=%d\n",curr,SA[j]);
				break;
			}

			saval=arr[j];

			// use older answers
			if(curr<SA[j] && curr>saval) {
				val=arr[j];
				//printf("For SA[%d]=%d, found SA[%d]=%d with prev=%d\n",i,curr,j,SA[j],saval);
				break;
			}
		
			// update
			if(ALGORITHM==BEFORE_LT) j--;
			else if(ALGORITHM==BEFORE_GT) j++;
		}
		arr[i]=val;
		if(ALGORITHM==BEFORE_LT) i++;
		else if(ALGORITHM==BEFORE_GT) i--;
	}
	printf("\nWriting intermediate result...\n");
	temp_out=fopen(tempfn,"wb");
	display_int_arr(temp_out,arr,size);
	printf("\nWrote \n");
	fclose(temp_out);

	printf("\nunraveling...\n");

	temp_in=fopen(tempfn,"rb");
	for(i=0; i<size; i++){
		readval=read_next_int(temp_in);
		arr[SA[i]]=readval;		
	}
	fclose(temp_in);

	printf("\nWriting answer...\n");
	temp_out=fopen(tempfn,"wb");
	display_int_arr(temp_out,arr,size);
	printf("\nWrote \n");
	fclose(temp_out);

	free(SA);
	return moves;
}

int match(char* T, int size, int i, int j){
	int k=0;
	while(i+k<size && j+k<size && T[i+k]==T[j+k]) k++;
	return k;
}

int is_correct_LPF_NAIVE(char* T,int lpfi,int i,int size){
	int valid=1,curr;
	int j,k;
	curr=0;
		
	for(j=0; j<i; j++){
		k=match(T,size,i,j);
		if(k>curr) curr=k;
	}

	if(curr!=lpfi){
		valid=0;
		printf("invalid LPF at i=%d, given=%d, actual=%d\n",i,lpfi,curr);
	}
	return valid;
}

struct LPF_data get_LPF_data_NAIVE(char* T,int i,int size){
	int j,k,curr;
	struct LPF_data ld;

	ld.ind=i;
	ld.num_locs_of_len=0;

	curr=0;
		
	for(j=0; j<i; j++){
		k=match(T,size,i,j);
		if(k>curr) ld.num_locs_of_len=0;
		if(k>=curr && ld.num_locs_of_len<BUF_SIZE){ 
			curr=k;
			ld.locs_of_len[ld.num_locs_of_len++]=j;
		}
	}
	return ld;
}

void print_LPF_data(struct LPF_data ld){
	int i;	
	printf("LPF DATA REPORTED\n...index=%d\n... len=%d\n",ld.ind,ld.len);
	for(i=0; i<ld.num_locs_of_len; i++)
		printf("... %d) @ index=%d\n",i,ld.locs_of_len[i]);	
	printf("\n\n");
}

int substit_for_eq(char s1[],char s2[]){
	int count=0;
	int i, len=strlen(s1);
	if(len != strlen(s2)) printf("\nsustit_for_eq: strings different size!\n");
	else{
		for(i=0; i<len; i++){
			if(s1[i]!=s2[i]){
				if(count==0) printf("first mismatch, s1[%d]=%c, s2[%d]=%c\n",i,s1[i],i,s2[i]);
				count++;
			}
		}
	}
	return count;
}

#endif
