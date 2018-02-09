#include "types.c"
#include "pstring.c"

#ifndef PAC_C
#define PAC_C

int FACTOR=1; // currently only works for 1
long double UPPER_AC_CODE=1; // currently only works for 1
int RECALCULATE_PAC=1;// in pAC2 and pAC3 functions, when to recalculate the code with pAC function
short MAKE_BLOCKS=1; // nonzero to use pAC function only to make blocks
short FIND_MAX_BLOCK=0; // nonzero to programmatically find max block


struct AC { long double lo; long double hi; };


struct AC pAC(int psuffix,int mblock,int start,int pv[],char pvtypes[],int max,int pvsize,long double cdf[],int remap[]);
long double tag(struct AC c);
struct AC remove_pAC_transition(int pv[],char pvtypes[],int fw[],int pvsize,struct AC mblockAC,int mblock,int removei,int psuffixq,long double prob,long double cdf[],int max,int remap[]);
struct AC add_pAC_transition(int pv[],char pvtypes[],int pvsize,struct AC mblockAC,int mblock,int addi,int psuffixq,long double cdf[],int max,int remap[]);
struct AC condense_AC(struct AC code);
struct AC expand_AC(struct AC code);
long double calc_epsilon(int block,long double cdf[]);
int max_block(int n,long double cdf[]);
short p_suffix_sort_pAC1(int mblock,int pSA[],int pv[],int fw[],int pvsize,char typespv[],int max,long double cdf[],int remap[],long double prob);
int map_to_cdf(int pvi,char typei,int max,int remap[]);
void make_uniform_cdf(int usymbols,long double *cdf);
void print_psuffixes(int start1,int mblock,int pv[],int fw[],int pvsize,char typespv[],int max,long double cdf[],int remap[],long double prob);
int find_recalculate_pAC_window(int mblock,int pv[],int fw[],int pvsize,char typespv[],int max,long double cdf[],int remap[],long double prob);



void print_psuffixes(int start1,int mblock,int pv[],int fw[],int pvsize,char typespv[],int max,long double cdf[],int remap[],long double prob)
{
	int block,start,suf;
	struct AC code;
	int psuffixq,removei,addm;
	long double finalcode1,finalcode2,finalcode3;
	
	if(DEBUG<=0) return; 
	
	printf("psuffixes (with pAC function):\n\n");
	for(start=start1,suf=0; suf<pvsize; suf++)
		tag(pAC(suf,mblock,start,pv,typespv,max,pvsize,cdf,remap));
	
	// testing transitions... with block>pvsize
/*	block=pvsize+1;
	printf("\np-suffixes (using remove transition):\n\n");
	code=pAC(0,block,0,pv,typespv,max,pvsize,cdf,remap);
	for(psuffixq=1,removei=0; psuffixq<pvsize; psuffixq++)
		code=remove_pAC_transition(pv,typespv,fw,pvsize,code,block,removei,psuffixq,prob,cdf,max,remap);
*/
	// testing transitions with blocks...
	printf("\np-suffixes (using blocks and add/remove transitions):\n\n");
	
	for(start=0; start<pvsize; start+=block)
	{
		block=3;
		printf("blocks of %d:\n",block);
		for(psuffixq=0; psuffixq<pvsize; psuffixq++)
			finalcode1=tag(pAC(psuffixq,block,start,pv,typespv,max,pvsize,cdf,remap));
		printf("\n");
		
		block=4;
		printf("blocks of %d:\n",block);
		for(psuffixq=0; psuffixq<pvsize; psuffixq++)
			finalcode2=tag(pAC(psuffixq,block,start,pv,typespv,max,pvsize,cdf,remap));
		printf("\n");
	}
	printf("using transitions:\n");
	code=pAC(0,block,start1,pv,typespv,max,pvsize,cdf,remap);
	
	for(psuffixq=1,removei=start1,addm=start1+block; psuffixq<pvsize && removei<pvsize; psuffixq++,removei++,addm++)
	{
		//code=expand_AC(code);
		code=remove_pAC_transition(pv,typespv,fw,pvsize,code,block,removei,psuffixq,prob,cdf,max,remap);
		
		//
		//code=pAC(psuffixq-1,block-1,start1+1,pv,typespv,max,pvsize,cdf,remap);
		//
		
		code=add_pAC_transition(pv,typespv,pvsize,code,block,addm,psuffixq,cdf,max,remap);
		finalcode3=tag(code);
		//code=condense_AC(code);
	}
	printf("\nhow precise are the codes? finalcode1=%.9Lf,finalcode2=%.9Lf,finalcode3=%.9Lf; if finalcode1==finalcode2==finalcode3?: %d; how close are finalcode2 and finalcode3? %.100Lf\n\n",finalcode1,finalcode2,finalcode3,(int)(finalcode1==finalcode2&&finalcode2==finalcode3),finalcode3-finalcode2);
}

void make_uniform_cdf(int usymbols,long double *cdf)
{
	int i;
	
	for(i=0; i<usymbols; i++)
		cdf[i]=(i/((long double)(usymbols-1)))*UPPER_AC_CODE;
	
	#if DEBUG>0
		{ printf("cdf: "); for(i=0; i<usymbols; i++) printf("[%Lf]",cdf[i]); printf("\n"); }
	#endif
}

int map_to_cdf(int pvi,char typei,int max,int remap[])
{
	if(typei==DISTANCE)
		return remap[pvi];
	else if(typei==CONSTANT)
		return remap[pvi+max];
	return 0;
}

int max_block(int n,long double cdf[])
{
	long double e=0;
	int k=0;
	
	do
	{
		k++;
		e=calc_epsilon(k,cdf);
		
		//
		//printf("k=%d, e=%.100Lf\n",k,e);
		//if(e<=EPSILON) printf("e is <= %.100Lf\n",EPSILON);
		//
		
	}while(k<=n && e>EPSILON);

	return k-1;
}

long double calc_epsilon(int block,long double cdf[])
{
	long double e=0;
	int k;
	struct AC new1,new2,old;
	
	new1.lo=new1.hi=old.lo=0;
	old.hi=UPPER_AC_CODE;
	for(k=0; k<block; k++)
	{
		new1.hi=old.lo+(old.hi-old.lo)*cdf[1];
		new1.lo=old.lo+(old.hi-old.lo)*cdf[0];	
		old=new1;
	}
	new2.lo=new2.hi=old.lo=0;
	
	if(tag(new1)-tag(new2)<=EPSILON){ // problem
		if(FIND_MAX_BLOCK==0) 
			printf("\nProblem 1 with block size: %.9Lf %.9Lf\n\n",tag(new1),tag(new2));
		e=-1;
	}else if(tag(new1)-tag(new2)<EPSILON)
	{
		e=tag(new2)-tag(new1);
	}else e=tag(new1)-tag(new2);
		if(DEBUG>0) 
			if(FIND_MAX_BLOCK==0) 
				printf("\n\nBlock %d, Epsilon %.9Lf; Nearby codes: %.9Lf %.9Lf\n\n",block,e,tag(new1),tag(new2));
	
	if(tag(new1)+e!=tag(new2) && tag(new2)+e!=tag(new1)) // problem
	{
		if(FIND_MAX_BLOCK==0) 
			printf("\nBlock %d, Problem 2 with block size\n\n",block);
		e=-1;
	}
	
	if(FIND_MAX_BLOCK==0) 
		if(e<=EPSILON) printf("*** epsilon value is %.100Lf invalid ***\n\n",e);
	
	return e;
}

int find_recalculate_pAC_window(int mblock,int pv[],int fw[],int pvsize,char typespv[],int max,long double cdf[],int remap[],long double prob)
{
	int start=1,i,q=0;
	struct AC code,code2;
	int psuffixq,removei,addm;
	int num=0,num2=0,failed=0;
	int FIXED_ITERATIONS=3; // for practical speed, should be able to find the window in 3 iterations
	
	for(i=1; i<FIXED_ITERATIONS && i<pvsize/mblock; i++)
	{
		num=0;
		failed=0;
		
		code=pAC(0,mblock,q,pv,typespv,max,pvsize,cdf,remap);
		
		for(psuffixq=1,removei=q,addm=q+mblock; psuffixq<pvsize && removei<pvsize; psuffixq++,removei++,addm++)
		{
			//code=expand_AC(code);
			code=remove_pAC_transition(pv,typespv,fw,pvsize,code,mblock,removei,psuffixq,prob,cdf,max,remap);
			
			code2=pAC(psuffixq,mblock-1,q,pv,typespv,max,pvsize,cdf,remap);
			
			if(equal_epsilon(tag(code),tag(code2))==0) 
			{
				//printf("failed: epsilon=%.100Lf, tag(code)=%.100Lf !~ tag(code2)=%.100Lf\n",epsilon,tag(code),tag(code2));
				failed=1;
				break;
			}
			else num++;
			
			code=add_pAC_transition(pv,typespv,pvsize,code,mblock,addm,psuffixq,cdf,max,remap);
			//code=condense_AC(code);
			
			code2=pAC(psuffixq,mblock,q,pv,typespv,max,pvsize,cdf,remap);
			
			if(equal_epsilon(tag(code),tag(code2))==0) 
			{
				//printf("failed: epsilon=%.100Lf, tag(code)=%.100Lf !~ tag(code2)=%.100Lf\n",epsilon,tag(code),tag(code2));
				failed=1;
				break;
			}
			else num++;
			
		}
		//printf("i=%d, num=%d, num2=%d, failed=%d\n",i,num,num2,failed);
		if((failed!=0 && num<num2) || i==1) num2=num;
		q=q+mblock;
	}
	
	//return num2-1;
	return (num2-1)/2; // since use FIXED_ITERATIONS for speed; we /2 just to be safe
}

struct AC remove_pAC_transition(int pv[],char pvtypes[],int fw[],int pvsize,struct AC mblockAC,int mblock,int removei,int psuffixq,long double prob,long double cdf[],int max,int remap[])
{
	int r=removei;//psuffixq+removei-1;
	int beta=fw[psuffixq-1];
	int j=psuffixq+beta-1;
	int e=(removei+mblock-1<pvsize)?(removei+mblock-1):(pvsize);//(psuffixq+mblock-2<pvsize)?(psuffixq+mblock-2):(pvsize);
	long double lambda=0;
	long double c;
	int pvr=0;
	int i;
	struct AC new1;
	
	if(j<pvsize)
	//if(pv[j]>=0 && pv[j]+max<pvsize)
		//lambda=(map_to_cdf(beta,pvtypes[r],max,remap)-map_to_cdf(0,DISTANCE,max,remap))*pow_int(prob,beta%mblock+1);	
		lambda=(map_to_cdf(pv[j],pvtypes[j],max,remap)-map_to_cdf(0,DISTANCE,max,remap))*pow_int(prob,beta%mblock+1);
	
	if(pvtypes[r]==CONSTANT || (pvtypes[r]==DISTANCE && r-pv[r]>=psuffixq-1)) pvr=pv[r];
	//printf("removing %d %d psuffixq %d mblock %d e %d\n",removei,pvr,psuffixq,mblock,e);
	c=cdf[map_to_cdf(pvr,pvtypes[r],max,remap)-1];
	
	//printf("j=%d, e=%d, r=%d\n",j,e,r);
	
//	if((pvtypes[r]==DISTANCE && (j>e || j<=r)) || (pvtypes[r]==CONSTANT && (j>e || j<=r)))
	if((pvtypes[psuffixq-1]==DISTANCE && (j>e || j<=r)) || (pvtypes[psuffixq-1]==CONSTANT))
	{
		//printf("removing %d only  c=%Lf, prob=%Lf\n",pv[r],c,prob);
		new1.lo=(mblockAC.lo-c)/prob;
		new1.hi=(mblockAC.hi-c)/prob;
	}else{
		//printf("removing %d with adjustment on %d  c=%Lf, prob=%Lf, lambda=%Lf, beta=%d, map_diff_hi=%d, mapp_diff_low=%d, map_diff=%d, pow_num=%Lf, map_diff*pow_num=%Lf\n",pv[r],pv[j],c,prob,lambda,beta,map_to_cdf(pv[beta],pvtypes[r],max,remap),map_to_cdf(0,DISTANCE,max,remap),(map_to_cdf(pv[j],pvtypes[j],max,remap)-map_to_cdf(0,DISTANCE,max,remap)),pow_int(prob,beta%mblock+1),(map_to_cdf(pv[j],pvtypes[j],max,remap)-map_to_cdf(0,DISTANCE,max,remap))*pow_int(prob,beta%mblock+1));
		new1.lo=(mblockAC.lo-lambda-c)/prob;
		new1.hi=(mblockAC.hi-lambda-c)/prob;
	}
	#if DEBUG>0
	{
		printf("%.9Lf: ",tag(new1)); 
		for(i=removei+1; i<=e; i++)
		{
			if(i>=pvsize) printf("[$]");
			else if(pvtypes[i]==CONSTANT) printf("[%d]",pv[i]);
			else if(pvtypes[i]==DISTANCE && i-pv[i]<psuffixq) printf("[0]");
			else if(pvtypes[i]==DISTANCE) printf("[%d]",pv[i]);
		}
		printf("\n");
	}
	#endif
	return new1;
}

struct AC add_pAC_transition(int pv[],char pvtypes[],int pvsize,struct AC mblockAC,int mblock,int addi,int psuffixq,long double cdf[],int max,int remap[])
{
	int a=addi;//psuffixq+addi-1;
	int k=a-pv[a];
	long double delta=mblockAC.hi-mblockAC.lo;
	long double d=0,f=0,v=0,w=0;
	int pva=0;
	int num=addi;//psuffixq+addi-1;
	char pvta=((num>=pvsize || pvtypes[a]==CONSTANT)?(CONSTANT):(DISTANCE));
	struct AC new1 = mblockAC;
	int s;
	int i;
	
	if(num>pvsize) return new1;
	
	if(pvta==CONSTANT && a<pvsize) pva=pv[a];
	
	if(num>=pvsize)//add a terminal
	{
		d=delta*cdf[1];
		f=delta*cdf[0];
	}else{
		d=delta*cdf[map_to_cdf(pva,pvtypes[a],max,remap)];
		f=delta*cdf[map_to_cdf(pva,pvtypes[a],max,remap)-1];
		v=delta*cdf[map_to_cdf(pv[a],pvtypes[a],max,remap)];
		w=delta*cdf[map_to_cdf(pv[a],pvtypes[a],max,remap)-1];
	}
	if((pvtypes[a]==DISTANCE && k<psuffixq) || pvtypes[a]==CONSTANT || num>=pvsize)
	{
		
	//
	//printf("adding pva=%d\n",pva);
	//
		
		new1.lo=(mblockAC.lo+f);
		new1.hi=(mblockAC.lo+d);
	}else{
	
	//
	//printf("adding pva=%d\n",pv[a]);
	//
	
		new1.lo=(mblockAC.lo+w);
		new1.hi=(mblockAC.lo+v);
	}
	#if DEBUG>0
	{
		printf("%.9Lf: ",tag(new1)); 
		s=a-mblock+1;
		//printf("psuffixq=%d,start=%d,end=%d\n",psuffixq,s,a);
		for(i=s; i<=a; i++)
		{
			if(i>=pvsize) printf("[$]");
			else if(pvtypes[i]==CONSTANT) printf("[%d]",pv[i]);
			else if(pvtypes[i]==DISTANCE && i-pv[i]<psuffixq) printf("[0]");
			else if(pvtypes[i]==DISTANCE) printf("[%d]",pv[i]);
		}
		printf("\n");
	}
	#endif
	return new1;
}

struct AC condense_AC(struct AC code)
{
	struct AC val=code;
	val.hi/=FACTOR;
	val.lo/=FACTOR;
	return val;
}

struct AC expand_AC(struct AC code)
{
	struct AC val=code;
	val.hi*=FACTOR;
	val.lo*=FACTOR;
	return val;
}

struct AC pAC(int psuffix,int mblock,int start,int pv[],char pvtypes[],int max,int pvsize,long double cdf[],int remap[])
{
	int end=(psuffix+start+mblock-1>pvsize)?pvsize:(psuffix+start+mblock-1), k, pvk, mcdf;
	struct AC new1,old;
	//printf("psuffix %d start %d mblock %d end is %d\n",psuffix,start,mblock,end);
	new1.lo=new1.hi=old.lo=0;
	old.hi=UPPER_AC_CODE;
	for(k=psuffix+start; k<=end; k++)
	{
		if(pvtypes[k]==DISTANCE && (k-pv[k]<psuffix)) // reset parameter distance
			pvk=0;
		else pvk=pv[k];
		if(k==pvsize) // at end, add a $ terminal (cdf @1)
			mcdf=1;
		else mcdf=map_to_cdf(pvk,pvtypes[k],max,remap);
		//printf("pv[k]=%d, cdf=%Lf, val=%d\n",pv[k],cdf[map_to_cdf(pv[k],pvtypes[k],max,remap)],map_to_cdf(pv[k],pvtypes[k],max,remap));
		
		new1.hi=old.lo+(old.hi-old.lo)*cdf[mcdf];
		new1.lo=old.lo+(old.hi-old.lo)*cdf[mcdf-1];	
		
		old=new1;
	}//printf("new1.hi=%Lf, new1.lo=%Lf\n",new1.hi,new1.lo);
	
	#if DEBUG>1
	{
		printf("%.9Lf: ",tag(new1));
		for(k=psuffix+start; k<=end; k++)
		{
			if(k==pvsize)
				printf("[$]");
			else
			{
				if(pvtypes[k]==DISTANCE && (k-pv[k]<psuffix)) // reset parameter distance
					pvk=0;
				else pvk=pv[k];
				printf("[%d]",pvk);
			}
		}
		printf("\n");
	}
	#endif
	
	return new1;
}

long double tag(struct AC c)
{
	return (c.hi+c.lo)/2.0;
}


#endif

