#include "types.c"
#include "string_ds.c"

#ifndef PSTRINGDS_C
#define PSTRINGDS_C

char PSA2_FILE[BUF_SIZE];

void display_pSA(int pSA[],int pv[],int pvsize,char pvtypes[]);
short correct_pSA(int pSA[],int pv[],int pvsize,char pvtypes[]);
void p_suffix_sort_pAC2(int mblock,int pSA[],int pv[],int fw[],int pvsize,char typespv[],int max,long double cdf[],int remap[],long double prob);
short p_suffix_sort_pAC3(int mblock,int pSA[],int pv[],int fw[],int pvsize,char typespv[],int max,long double cdf[],int remap[],long double prob);
void compute_pLCP(int pvsize,int pv[],char pvtypes[],int pLCP[],int pSA[]);
int Lambda_pmatcher(int a,int b,int q,int pv1[],char typespv1[],int pv2[],char typespv2[],int pvsize);
struct wtuple Omega_pmatch_manager(short alg_type,int pvsize,int i,int pLPF_lt[],int pLPF_gt[],int before_lt[],int before_gt[],int R[]);
struct wtuple Omega_pmatch_manager_weak(short alg_type,int pvsize,int i,int pLPF_lt[],int pLPF_gt[],int before_lt[],int before_gt[],int R[]);
struct wcases construct(short type,int out[],int arr1[],int arr2[],int R[],int pv1[],char typespv1[],int pv2[],char typespv2[],int pvsize);
struct wcases compute_pLPF(int pLPF[],int before_lt[],int before_gt[],int pSA[],int R[],int pv[],char typespv[],int pvsize);
struct wcases compute_pLneF(int pLneF[],int neq_lt[],int neq_gt[],int pSA[],int R[],int pv[],char typespv[],int pvsize);
struct wcases compute_pLrF(int pLrF[],int rev_lt[],int rev_gt[],int pSA[],int R[],int pvQ1[],char typespvQ1[],int pvQ2[],char typespvQ2[],int pvsize);
struct wcases compute_pborder(int pborder[],int prefix[],int suffix[],int pSA[],int R[],int pv[],char typespv[],int pvsize);
short correct_pborder(int pborder[],int pv[],char pvtypes[],int pvsize);
struct wcases_pair compute_pLCP_TCS(int pLCP[],int before_lt[],int after_lt[],int pSA[],int R[],int pv[],char typespv[],int pvsize);
struct wcases compute_pLCP_TCS_IMP(int pLCP[],int M[],int pSA[],int R[],int pv[],char typespv[],int pvsize);
struct wcases_pair compute_pLCP_JDA(short type,int pLCP[],int before_lt[],int after_lt[],int pSA[],int R[],int pv[],char typespv[],int pvsize);
short correct_pLPF_variant(short type,int R[],int answer[],int cand_arr1[],int cand_arr2[],int pv1[],char typespv1[],int pv2[],char typespv2[],int pvsize);
void disp_cases_summary(FILE* out,struct wcases w);
void execute_pLPF_variant(short type,int pSA[],int R[],int pvsize,int out[],int arr1[],int arr2[],int arr3[],int arr4[],int pv[],char pvtypes[],int pv1[],char pvtypes1[],int pv2[],char pvtypes2[]);
struct wcases compute_pLCP_JDA_IMP(short type,int pLCP[],int M[],int pSA[],int R[],int pv[],char typespv[],int pvsize);
struct wcases compute_pLF(int pLF[],int pLneF[],int neq_lt[],int neq_gt[],int pLrF[],int rev_lt[],int rev_gt[],int pSA_T[],int R_T[],int pSA_Q[],int R_Q[],int pvQ1[],char typespvQ1[],int pvQ2[],char typespvQ2[],int pvsize,long double *t_plnef,long double *t_plrf);


void execute_pLPF_variant(short type,int pSA[],int R[],int pvsize,int out[],int arr1[],int arr2[],int arr3[],int arr4[],int pv[],char pvtypes[],int pv1[],char pvtypes1[],int pv2[],char pvtypes2[])
{
	long double start_sec,end_sec,t_plnef=0,t_plrf=0;
	struct wcases w;
	struct wcases_pair wp;
	char ctype[BUF_SIZE];
	short correct,correct2;
	int i,k;
	int *temp_pLneF=NULL;
	int *temp_pLrF=NULL;
	int *pSA_T=NULL;
	int *R_T=NULL;
	
	sprintf(ctype,"%c",'\0');
	
	for(i=0; i<pvsize; i++)
	{
		if(arr1!=NULL) arr1[i]=0;
		if(arr2!=NULL) arr2[i]=0;
		if(arr3!=NULL) arr3[i]=0;
		if(arr4!=NULL) arr4[i]=0;
	}
	
	if(type==PLRF || type==PLF)
	{
		construct_rank_array(R,pSA,2*pvsize+1);
		//printf("\n\nR="); display_int_arr(stdout,R,2*pvsize+1); printf("\n\n");
	}
	else
		construct_rank_array(R,pSA,pvsize);
	
	if(type==PLF)
	{
		temp_pLneF=(int *)malloc((pvsize)*(sizeof(int)));
		temp_pLrF=(int *)malloc((pvsize)*(sizeof(int)));
		pSA_T=(int *)malloc((pvsize)*(sizeof(int)));
		R_T=(int *)malloc((pvsize)*(sizeof(int)));
		
		// make other pSA and R for original T
		/*k=0;
		for(i=0;i<2*pvsize+1; i++)
		{
			if(pSA[i]<pvsize)
				pSA_T[k++]=pSA[i];
		}*/
		sprintf(PSA2_FILE,"%s%c",PSA_T_FN,'\0');
		read_int_arr(PSA2_FILE,pSA_T,pvsize);
		
		//
		//correct=correct_pSA(pSA_T,pv1,pvsize,pvtypes1);
		//printf("correct=%d",correct);
		//
		
		construct_rank_array(R_T,pSA_T,pvsize);
	
		start_sec=get_current_microseconds();

		w=compute_pLF(out,temp_pLneF,arr1,arr2,temp_pLrF,arr3,arr4,pSA_T,R_T,pSA,R,pv1,pvtypes1,pv2,pvtypes2,pvsize,&t_plnef,&t_plrf);
		
		end_sec=get_current_microseconds();
	
		sprintf(ctype,"pLF%c",'\0');
			
	}else{
		start_sec=get_current_microseconds();
									
		switch(type)
		{
			case PLNEF: 
				w=compute_pLneF(out,arr1,arr2,pSA,R,pv1,pvtypes1,pvsize); sprintf(ctype,"pLneF%c",'\0'); break;
			case PLPF: 
				w=compute_pLPF(out,arr1,arr2,pSA,R,pv1,pvtypes1,pvsize); sprintf(ctype,"pLPF%c",'\0'); break;
			case PLCP_TCS:
				wp=compute_pLCP_TCS(out,arr1,arr2,pSA,R,pv1,pvtypes1,pvsize); sprintf(ctype,"pLCP%c",'\0'); break;
			case PLCP_TCS_IMP:
				w=compute_pLCP_TCS_IMP(out,arr1,pSA,R,pv1,pvtypes1,pvsize); sprintf(ctype,"pLCP%c",'\0'); break;
			case PLCP_JDA: sprintf(ctype,"pLCP%c",'\0');
			case PERMUTED_PLCP_JDA:
				if(type==PERMUTED_PLCP_JDA) sprintf(ctype,"permuted-pLCP%c",'\0');
				wp=compute_pLCP_JDA(type,out,arr1,arr2,pSA,R,pv1,pvtypes1,pvsize); break;
			case PLCP_JDA_IMP: sprintf(ctype,"pLCP%c",'\0');
			case PERMUTED_PLCP_JDA_IMP:
				if(type==PERMUTED_PLCP_JDA_IMP) sprintf(ctype,"permuted-pLCP%c",'\0');
				w=compute_pLCP_JDA_IMP(type,out,arr1,pSA,R,pv1,pvtypes1,pvsize); break;
			case PLRF:
				w=compute_pLrF(out,arr1,arr2,pSA,R,pv1,pvtypes1,pv2,pvtypes2,pvsize); sprintf(ctype,"pLrF%c",'\0'); break;
			case PBORDER:
				w=compute_pborder(out,arr1,arr2,pSA,R,pv1,pvtypes1,pvsize); sprintf(ctype,"p-border%c",'\0'); break;
		}
		end_sec=get_current_microseconds();
	}
	if(type==PLCP_TCS || type==PLCP_JDA || type==PERMUTED_PLCP_JDA)
	{
		for(i=0; i<NUM_WCASES; i++)
			w.num[i]=wp.w1.num[i]+wp.w2.num[i];
	}
	
	if(type==PLF)
	{
		printf("\n\n*** compute_pLneF executed for a duration of %.0Lf usec or %.10Lf sec ***\n\n",t_plnef,(t_plnef)/1000000.0);
		printf("\n\n*** compute_pLrF executed for a duration of %.0Lf usec or %.10Lf sec ***\n\n",t_plrf,(t_plrf)/1000000.0);
	}
	
	printf("\n\n*** compute_%s executed for a duration of %.0Lf usec or %.10Lf sec (from %.0Lf to %.0Lf) ***\n\n",ctype,end_sec-start_sec,(end_sec-start_sec)/1000000.0,start_sec,end_sec);

	printf("\n\nDisplaying %s...\n\n",ctype);
	display_int_arr(stdout,out,pvsize);
	
	if(VERIFY_PLPF_VARIANTS!=0)
	{
		printf("\n\nVerifying %s...\n\n",ctype);
		
		system("date;");
		start_sec=get_current_microseconds();
		if(type==PLF)
		{
			correct=correct_pLPF_variant(PLNEF,R_T,temp_pLneF,arr1,arr2,pv1,pvtypes1,pv1,pvtypes1,pvsize);
			
			if(correct!=0) printf("\n*** Correct pLneF ***\n\n");
			else printf("\n*** Invalid pLneF ***\n\n");
			
			correct2=correct_pLPF_variant(PLRF,R,temp_pLrF,arr3,arr4,pv1,pvtypes1,pv2,pvtypes2,pvsize);
			if(correct2!=0) printf("\n*** Correct pLrF ***\n\n");
			else printf("\n*** Invalid pLrF ***\n\n");
			
			// max is trivial correctness
			if(correct>0 && correct2>0) correct=1;
			else correct=0;
		}
		else if(type==PLRF)
			correct=correct_pLPF_variant(type,R,out,arr1,arr2,pv1,pvtypes1,pv2,pvtypes2,pvsize);
		else if(type==PBORDER)
		{
			if(RUN_NAIVE_PBORDER_CHECK>0)
				correct=correct_pborder(out,pv1,pvtypes1,pvsize);
		}else
			correct=correct_pLPF_variant(type,R,out,arr1,arr2,pv,pvtypes,pv,pvtypes,pvsize);
			
		end_sec=get_current_microseconds();
		
		system("date;");
		
		if(type==PBORDER && RUN_NAIVE_PBORDER_CHECK==0)
			printf("\n\n*** naive_pborder validation not run (too much time required) ***\n\n");
		else
		{
			printf("\n\n*** naive_verification executed for a duration of %.0Lf usec or %.10Lf sec (from %.0Lf to %.0Lf) ***\n\n",end_sec-start_sec,(end_sec-start_sec)/1000000.0,start_sec,end_sec);
			
			if(correct==0) printf("\n*** Invalid %s ***\n\n",ctype);
			else printf("\n*** Correct %s ***\n\n",ctype);
		}
	}
	disp_cases_summary(stdout,w);
	printf("\n\nmean(%s)=%.10Lf\nmax(%s)=%d\n\n",ctype,mean(pvsize,out),ctype,max(pvsize,out));
	
	if(type==PLF)
	{
		printf("\n\nmean(%s)=%.10Lf\nmax(%s)=%d\n\n","pLneF\0",mean(pvsize,temp_pLneF),"pLneF\0",max(pvsize,temp_pLneF));
		printf("\n\nmean(%s)=%.10Lf\nmax(%s)=%d\n\n","pLrF\0",mean(pvsize,temp_pLrF),"pLrF\0",max(pvsize,temp_pLrF));
	}
	
	free(temp_pLneF);free(temp_pLrF);free(pSA_T);free(R_T);
}

short correct_pborder(int pborder[],int pv[],char pvtypes[],int pvsize)
{
	short correct=1;
	int i,j,q,m,p1,p2;
	
	if(DEBUG>0) printf("\n\np-border=");
	
	for(i=0; i<pvsize /*&& correct!=0*/; i++)
	{
		q=0;
		m=0;
		for(j=1; j<=i && m==0; j++)
		{
			q=0;
			do
			{
				if(pvtypes[q]==CONSTANT && pvtypes[j+q]==CONSTANT && pv[q]==pv[j+q]) q++;
				else if(pvtypes[q]==DISTANCE && pvtypes[j+q]==DISTANCE)
				{
					p1=pv[q];
					if(pv[j+q]>q) p2=0;
					else p2=pv[j+q];
					if(p1==p2) q++;
					else break;
				}
				else break;
			}while(j+q<=i);
			if(j+q==i+1) // successful match!
				m=1;
			else q=0;
		}
		if(pborder[i]!=q) correct=0;
		if(DEBUG>0) printf("(%d:%d)",i,q);
	}
	
	if(DEBUG>0) printf("\n\n");
	
	return correct;
}

void disp_cases_summary(FILE* out,struct wcases w)
{
	int k;
	fprintf(out,"\nSummary of Cases...\n\n");
	for(k=0; k<NUM_WCASES; k++)
	{
		switch(k)
		{
			case TRIVIAL: fprintf(out,"%-10s: %-d\n","TRIVIAL",w.num[k]); break;
			case FIG_1A: fprintf(out,"%-10s: %-d\n","FIG_1A",w.num[k]); break;
			case FIG_1B: fprintf(out,"%-10s: %-d\n","FIG_1B",w.num[k]); break;
			case FIG_1C: fprintf(out,"%-10s: %-d\n","FIG_1C",w.num[k]); break;
			case FIG_1D: fprintf(out,"%-10s: %-d\n","FIG_1D",w.num[k]); break;
			case FIG_1E: fprintf(out,"%-10s: %-d\n","FIG_1E",w.num[k]); break;
			case FIG_1F: fprintf(out,"%-10s: %-d\n","FIG_1F",w.num[k]); break;
		}
	}
}

void compute_pLCP(int pvsize,int pv[],char pvtypes[],int pLCP[],int pSA[])
{
	int i,j,k,ri,rj,a,b;
	
	if(pvsize<=0) return;
	
	pLCP[0]=0;
	
	if(pvsize>1)
	{
		for(j=1; j<pvsize; j++)
		{
			i=j-1;
			k=0;
			do
			{
				a=pSA[i]+k;
				b=pSA[j]+k;
				
				if(pvtypes[a]==pvtypes[b] && pvtypes[a]==CONSTANT)
				{
					if(pv[a]==pv[b]) k++;
					else break;
				}
				else if(pvtypes[a]==pvtypes[b] && pvtypes[a]==DISTANCE)
				{
					ri=pv[a];
					rj=pv[b];
					if(a-pv[a]<pSA[i]) ri=0;
					if(b-pv[b]<pSA[j]) rj=0;
					if(ri==rj) k++;
					else break;
				}				
				else break;
			}while(pSA[i]+k<pvsize && pSA[j]+k<pvsize);
			pLCP[j]=k;
		}
	}
	
	#if DEBUG>1
		for(i=0; i<pvsize; i++)
			printf("(pLCP[%d]=%d)",i,pLCP[i]);
		printf("\n\n");
	#endif

}

void display_pSA(int pSA[],int pv[],int pvsize,char pvtypes[])
{
	int j,k,pvk;
		
	for(j=0; j<pvsize; j++)
	{
		if(DEBUG>0)
		{
			printf("%-9d: ",pSA[j]);
			if(DEBUG>0)
			{
				for(k=pSA[j]; k<=pvsize; k++)
				{
					if(k==pvsize)
						printf("[$]");
					else
					{
						if(pvtypes[k]==DISTANCE && (k-pv[k]<pSA[j])) // reset parameter distance
							pvk=0;
						else pvk=pv[k];
						printf("[%d]",pvk);
					}
				}
			}
			printf("\n");
		}else printf("(%d:%d)",j,pSA[j]);
	}printf("\n");
}

short correct_pSA(int pSA[],int pv[],int pvsize,char pvtypes[])
{
	short OK=0;
	int i,j,k,pvk,correct=1,ri,rj;
	
	for(i=0; i<pvsize-1; i++)
	{
		OK=0;
		j=i+1;
		k=0;
		while(pSA[i]+k<pvsize && pSA[j]+k<pvsize)
		{
			if(pvtypes[pSA[i]+k]==CONSTANT && pvtypes[pSA[j]+k]==DISTANCE)
			{
				correct=0;
				if(DISPLAY_PSA_ERRORS != 0)
				{
					printf("1: incorrect order of p-suffix %d and %d; types[%d]=%d types[%d]=%d\n",pSA[i],pSA[j],pSA[i]+k,(int)pvtypes[pSA[i]+k],pSA[j]+k,(int)pvtypes[pSA[j]+k]);
				}else return correct;
			}else if(pvtypes[pSA[i]+k]==DISTANCE && pvtypes[pSA[j]+k]==CONSTANT)//still valid
			{
				OK=1;
				break;
			}// at this point, same pvtypes
			ri=pv[pSA[i]+k];
			rj=pv[pSA[j]+k];
			if(pvtypes[pSA[i]+k]==CONSTANT)
			{
				if(ri>rj)
				{
					correct=0;
					if(DISPLAY_PSA_ERRORS != 0)
					{
						printf("2: incorrect order of p-suffix %d and %d\n",pSA[i],pSA[j]);
					}else return correct;
				}else if(ri<rj) //still valid
				{		
						OK=1;
						break;
				}
				// otherwise, equal and still valid
			}else
			{
				if(pSA[i]+k-ri<pSA[i]) ri=0;
				if(pSA[j]+k-rj<pSA[j]) rj=0;
				if(ri<rj) //still valid
				{
					OK=1;
					break;
				}
				else if(ri>rj)
				{
					correct=0;
					if(DISPLAY_PSA_ERRORS != 0)
					{
						printf("3: incorrect order of p-suffix %d and %d\n",pSA[i],pSA[j]);
					}else return correct;
				}
				// otherwise, equal and still valid
			}
			k++;
		}
		if(OK!=1 && pSA[j]>pSA[i]) // at this point, p-suffixes are equal, and no more symbols on one p-suffix, so shorter must be <
		{
			correct=0;
			if(DISPLAY_PSA_ERRORS != 0)
			{
				printf("4: incorrect order of p-suffix %d and %d\n",pSA[i],pSA[j]);
			}else return correct;
		}
	}
	return correct;
}

short p_suffix_sort_pAC1(int mblock,int pSA[],int pv[],int fw[],int pvsize,char typespv[],int max,long double cdf[],int remap[],long double prob)
{
	int start=0, recalc=0;
	short average_case=1;
	struct AC current;
	int psuffixq,removei=0,addm=mblock,k;
	long double tagged_pac_min=UPPER_AC_CODE, tagged_pac_max=0;
	struct pcode_d *code=(struct pcode_d *)malloc(pvsize*(sizeof(struct pcode_d)));
	long double *tagged=(long double *)malloc(pvsize*(sizeof(long double)));
	
	#if DEBUG>1 
		printf("\nmblock=%d\n\n",mblock);
	#endif
	
	if(!code || !tagged)
	{

		fprintf(stderr,"*** p_suffix_sort_pAC1: error allocating memory ***\n");
		free(code);free(tagged);
		return 0;
	}
	
	if(pvsize<mblock) mblock=pvsize;
	
	current=pAC(0,mblock,start,pv,typespv,max,pvsize,cdf,remap);
	tagged[0]=tag(current);
	if(tag(current)>=0 && tag(current)<tagged_pac_min) tagged_pac_min=tag(current);
	if(tag(current)<=UPPER_AC_CODE && tag(current)>tagged_pac_max) tagged_pac_max=tag(current);
	
	for(psuffixq=1,recalc=0; psuffixq<pvsize; psuffixq++,removei++,addm++)
	{
		if(MAKE_BLOCKS!=0 || RECALCULATE_PAC==1)
			current=pAC(psuffixq,mblock,start,pv,typespv,max,pvsize,cdf,remap);
		else
		{
			if(0<=removei && removei<pvsize)
			{
				if(recalc==RECALCULATE_PAC)
				{
					current=pAC(psuffixq,mblock-1,start,pv,typespv,max,pvsize,cdf,remap);
					recalc=0;
				}
				else
				{
					current=remove_pAC_transition(pv,typespv,fw,pvsize,current,mblock,removei,psuffixq,prob,cdf,max,remap);
					recalc++;
				}
			}
			if(0<=addm && addm<pvsize)
			{
				if(recalc==RECALCULATE_PAC)
				{
					current=pAC(psuffixq,mblock,start,pv,typespv,max,pvsize,cdf,remap);
					recalc=0;
				}
				else
				{
					current=add_pAC_transition(pv,typespv,pvsize,current,mblock,addm,psuffixq,cdf,max,remap);	
					recalc++;	
				}
			}
		}
		if(tag(current)>0 && tag(current)<tagged_pac_min) tagged_pac_min=tag(current);
		if(tag(current)<=UPPER_AC_CODE && tag(current)>tagged_pac_max) tagged_pac_max=tag(current);
		tagged[psuffixq]=tag(current);
	}	
	
	#if DEBUG>1
		printf("\nAvg case codes:\n\nmblock=%d,pac_min=%Lf,pac_max=%Lf\n\n",mblock,tagged_pac_min,tagged_pac_max);
	#endif
	
	for(psuffixq=0; psuffixq<pvsize; psuffixq++)
	{
		code[psuffixq].i=psuffixq;
		code[psuffixq].di=(int)(floor(MAKE_D_CONSTANT*(pvsize-1)*((tagged[psuffixq]-tagged_pac_min)/(tagged_pac_max-tagged_pac_min))));
		#if DEBUG>1
			printf("%-9ld: %d (%Lf)\n",code[psuffixq].i,code[psuffixq].di,tagged[psuffixq]);
		#endif
	}
	sort(pvsize,code,0,pvsize);
	for(k=0; k<pvsize; k++)
	{
		pSA[k]=code[k].i;
		if(k<pvsize && code[k].di==code[k+1].di) 
			average_case=0;
	}
	
	free(code);free(tagged);
	return average_case;
}

void p_suffix_sort_pAC2(int mblock,int pSA[],int pv[],int fw[],int pvsize,char typespv[],int max,long double cdf[],int remap[],long double prob)
{
	struct AC current;
	int q=0,removei=0,addm=mblock,k,b=0,s=1,recalc=0;
	struct pcode_f *code=(struct pcode_f *)malloc(pvsize*(sizeof(struct pcode_f)));
	struct pcode_f *code_tmp=(struct pcode_f *)malloc(pvsize*(sizeof(struct pcode_f)));
	int *R=(int *)malloc(pvsize*(sizeof(int)));
	
	#if DEBUG>1 
		printf("\nmblock=%d\n\n",mblock);
	#endif
	
	if(!code || !R || !code_tmp)
	{
		fprintf(stderr,"*** p_suffix_sort_pAC2: error allocating memory ***\n");
		free(code);free(R);free(code_tmp);return;
	}
	
	if(pvsize<mblock) mblock=pvsize;
	
	// A) -- initialize code pairs
	for(k=0; k<pvsize; k++)
	{
		code[k].i=code_tmp[k].i=k;
		code[k].fi=code_tmp[k].fi=0.0;
		R[k]=k;
	}
	
	// B) -- sort m-blocks of p-suffixes
	while(b<pvsize)
	{	
		// B1) -- generate m-block codes
		current=pAC(0,mblock,q,pv,typespv,max,pvsize,cdf,remap);
		code_tmp[R[0]].i=0;
		code_tmp[R[0]].fi=tag(current);
		
		if(MAKE_BLOCKS || RECALCULATE_PAC<=1)
		{
			for(k=1; k<pvsize; k++)
			{
				//printf("\n... making codes with pAC...\n\n");
				current=pAC(k,mblock,q,pv,typespv,max,pvsize,cdf,remap);
				code_tmp[R[k]].i=k;
				code_tmp[R[k]].fi=tag(current);
			}
		}
		else
		{
			for(k=1,removei=q,addm=q+mblock,recalc=0; k<=pvsize; k++,removei++,addm++)
			{
				if(0<=removei && removei<pvsize)
				{
					if(recalc==RECALCULATE_PAC)
					{
						current=pAC(k,mblock-1,q,pv,typespv,max,pvsize,cdf,remap);
						recalc=0;
					}
					else
					{
						current=remove_pAC_transition(pv,typespv,fw,pvsize,current,mblock,removei,k,prob,cdf,max,remap);
						recalc++;
					}	
				}
				if(0<=addm && addm<=pvsize)
				{
					if(recalc==RECALCULATE_PAC)
					{
						current=pAC(k,mblock,q,pv,typespv,max,pvsize,cdf,remap);
						recalc=0;
					}
					else
					{
						current=add_pAC_transition(pv,typespv,pvsize,current,mblock,addm,k,cdf,max,remap);	
						recalc++;	
					}
				}
				if(k<pvsize)
				{
					code_tmp[R[k]].i=k;
					code_tmp[R[k]].fi=tag(current);
				}
			}
		}		
				
		// B2) -- identify bucket
		for(k=0; k<pvsize; k++)
		{
			if(k==0) b=1;
			else
			{
				if(!((equal_epsilon(code[R[code_tmp[k].i]].fi,code[R[code_tmp[k-1].i]].fi))
					// the following condition is an implementation detail in case the bucket #s are different but epsilon causes codes to be falsely equal
					&& floor(code[R[code_tmp[k].i]].fi)==floor(code[R[code_tmp[k-1].i]].fi)
				   ))
				 b++;
			} 
			code_tmp[k].fi=b+code_tmp[k].fi-floor(code_tmp[k].fi);
		}
	
		/*
		printf("\n\nbefore sort... epsilon=%.100Lf\n",epsilon);
		for(k=0; k<pvsize; k++)
			printf("code[%d]=(i=%d,fi=%.100Lf)\n",k,code[k].i,code[k].fi);
		for(k=0; k<pvsize; k++)
			printf("code_tmp[%d]=(i=%d,fi=%.100Lf)\n",k,code_tmp[k].i,code_tmp[k].fi);
		*/
		
		// B3) -- sort p-suffixes
		sort_f(pvsize,code_tmp,0,pvsize);
		
		// B4) -- rank the p-suffixes
		for(k=0; k<pvsize; k++)
			R[code_tmp[k].i]=k;		

		// code=code_tmp
		for(k=0; k<pvsize; k++)
		{
			code[k].i=code_tmp[k].i;
			code[k].fi=code_tmp[k].fi;
		}
			
		q=q+mblock;
		s++;
	}
	
	// C) -- retain p-suffix array
	for(k=0; k<pvsize; k++)
		pSA[k]=code[k].i;
		
	free(code);free(R);free(code_tmp);
}
	
short p_suffix_sort_pAC3(int mblock,int pSA[],int pv[],int fw[],int pvsize,char typespv[],int max,long double cdf[],int remap[],long double prob)
{
	struct AC current;
	long double temp;
	short average_case=0;
	int q=0,removei=0,addm=mblock,k,b=0,s=1,i=0,f=0,x=pvsize,y=0,v,recalc=0;
	struct pcode_f *code=(struct pcode_f *)malloc(pvsize*(sizeof(struct pcode_f)));
	struct pcode_f *code_tmp=(struct pcode_f *)malloc(pvsize*(sizeof(struct pcode_f)));
	struct pcode_f *code_tmp2=(struct pcode_f *)malloc(pvsize*(sizeof(struct pcode_f)));
	int *R=(int *)malloc(pvsize*(sizeof(int)));
	int *R2=(int *)malloc(pvsize*(sizeof(int)));
	
	#if DEBUG>1
		printf("\nmblock=%d\n\n",mblock);
	#endif
	
	if(!code || !R || !R2 || !code_tmp || !code_tmp2)
	{
		fprintf(stderr,"*** p_suffix_sort_pAC3: error allocating memory ***\n");
		free(code);free(R);free(R2);free(code_tmp);free(code_tmp2);return average_case;
	}
	
	if(pvsize<mblock) mblock=pvsize;
	
	// A) -- initialize code pairs
	for(k=0; k<pvsize; k++)
	{
		code[k].i=code_tmp[k].i=k;
		code[k].fi=code_tmp[k].fi=0.0;
		R[k]=R2[k]=k;
	}
	
	// try average case algorithm
	average_case=p_suffix_sort_pAC1(mblock,pSA,pv,fw,pvsize,typespv,max,cdf,remap,prob);
	if(average_case!=0)
		return average_case;
	
	// B) -- sort m-blocks of p-suffixes
	while(f<x)
	{	
		f=0;
		
		// B1) -- generate m-block codes
		if(q+mblock-1<pvsize) y=q+mblock-1;
		else y=pvsize;
		
		current=pAC(0,y-q+1,q,pv,typespv,max,pvsize,cdf,remap);
		code_tmp[R[0]].i=0;
		code_tmp[R[0]].fi=tag(current);
				
		if(MAKE_BLOCKS || RECALCULATE_PAC<=1)
		{
			for(k=1; k<x; k++)
			{
				current=pAC(k,mblock,q,pv,typespv,max,pvsize,cdf,remap);
				code_tmp[R2[k]].i=k;
				code_tmp[R2[k]].fi=tag(current);
			}
		}
		else
		{
			for(k=1,removei=q,addm=q+mblock,recalc=0; k<x; k++,removei++,addm++)
			{
				if(0<=removei && removei<pvsize)
				{
					if(recalc==RECALCULATE_PAC)
					{
						current=pAC(k,mblock-1,q,pv,typespv,max,pvsize,cdf,remap);
						recalc=0;
					}
					else
					{
						current=remove_pAC_transition(pv,typespv,fw,pvsize,current,mblock,removei,k,prob,cdf,max,remap);
						recalc++;
					}	
				}
				if(0<=addm && addm<=pvsize)
				{
					if(recalc==RECALCULATE_PAC)
					{
						current=pAC(k,mblock,q,pv,typespv,max,pvsize,cdf,remap);
						recalc=0;
					}
					else
					{
						current=add_pAC_transition(pv,typespv,pvsize,current,mblock,addm,k,cdf,max,remap);	
						recalc++;	
					}	
				}
				if(k<pvsize)
				{
					code_tmp[R[k]].i=k;
					code_tmp[R[k]].fi=tag(current);
				}
			}
		}		
				
		// B2) -- identify bucket
		radix_sort_uint(R2,x,RSORT_CONST);

		for(k=0; k<x; k++)
		{
			if(R2[k]==0)
			{
				b=1;
				f++;
			}
			else
			{
				if(!(equal_epsilon(code[R2[k]].fi,code[R2[k]-1].fi)))
				{
					b=R2[k]+1;
					f++;
				}
			} 
			code_tmp[R2[k]].fi=b+code_tmp[R2[k]].fi-floor(code_tmp[R2[k]].fi);
		}

		// !! make new tmp to sort	
		for(v=0; v<x; v++)
		{
			code_tmp2[v].fi=code_tmp[R[v]].fi;
			code_tmp2[v].i=code_tmp[R[v]].i;
		}
		
		// B3) -- sort p-suffixes
		sort_f(pvsize,code_tmp2,0,x);

		// !! restore tmp into code_tmp
		for(v=0; v<x; v++)
		{
			code_tmp[R2[v]].fi=code_tmp2[v].fi;
			code_tmp[R2[v]].i=code_tmp2[v].i;
		}				

		// B4) -- rank the p-suffixes
		for(k=0; k<x; k++)
		{
			R[code_tmp[R2[k]].i]=R2[k];
		}
		for(k=0; k<x; k++)
		{
			R2[k]=R[k];
		}

		for(k=0; k<x; k++)
		{
			code[R2[k]].i=code_tmp[R2[k]].i;
			code[R2[k]].fi=code_tmp[R2[k]].fi;
		}
					
		q=q+mblock;
		i++;
		x=pvsize-i*mblock+1;
		s++;
	}
	
	// C) -- retain p-suffix array
	for(k=0; k<pvsize; k++)
		pSA[k]=code[k].i;
		
	free(code);free(R);free(R2);free(code_tmp);free(code_tmp2);
	
	return average_case;
}

int Lambda_pmatcher(int a,int b,int q,int pv1[],char typespv1[],int pv2[],char typespv2[],int pvsize)
{
	short c=1;
	int x,y;
	
	if(a==NOT_EXIST || b==NOT_EXIST) return 0;
	while(c!=0 && (a+q<pvsize) && (b+q<pvsize))
	{
		x=pv1[a+q];
		y=pv2[b+q];

		if(typespv1[a+q]==CONSTANT && typespv2[b+q]==CONSTANT)
		{
			if(x==y) q++;
			else c=0;
		}else if(typespv1[a+q]==DISTANCE && typespv2[b+q]==DISTANCE)
		{
			if(q<x) x=0;
			if(q<y) y=0;
			if(x==y) q++;
			else c=0;
		}else c=0;
	}
	return q;
}

struct wtuple Omega_pmatch_manager(short alg_type,int pvsize,int i,int pLPF_lt[],int pLPF_gt[],int before_lt[],int before_gt[],int R[])
{
	struct wtuple p;
	int a=0, b=0, c=0, d=0;
	int j=0, k=0; // implementation issue: needed to treat as int type, since -1 can be detected as invalid and reset
	short type=TRIVIAL;
	
	if(i>0 && before_lt!=NULL)
	{
		/*if(alg_type==PLRF) a=before_lt[i-1]+pvsize+2;
		else*/ 
		a=before_lt[i-1]+1; 
		
		c=pLPF_lt[i-1]-1;
		if(before_gt!=NULL)
		{
			/*if(alg_type==PLRF) b=before_gt[i-1]+pvsize+2;
			else*/ 
			b=before_gt[i-1]+1; 
			
			d=pLPF_gt[i-1]-1;
		}
		
		if(before_gt!=NULL && before_lt[i]!=NOT_EXIST && before_gt[i]!=NOT_EXIST)
		{
			if(a==0 && b==0){
				j=(int)pLPF_gt[before_lt[i]]; k=(int)pLPF_lt[before_gt[i]];
			}else if(a==0 && R[b]<R[i]){
				j=(int)d; k=(int)pLPF_lt[before_gt[i]];
			}else if(a==0 && R[b]>R[i]){
				j=(int)pLPF_gt[before_lt[i]]; k=(int)d;
			}else if(b==0 && R[a]<R[i]){
				j=(int)c; k=(int)pLPF_lt[before_gt[i]];
			}else if(b==0 && R[a]>R[i]){
				j=(int)pLPF_gt[before_lt[i]]; k=(int)c;
			}/* Fig. 1(a) */else if(R[a]<R[i] && R[i]<R[b]){
				j=(int)c; k=(int)d; type=FIG_1A;
			}/* Fig. 1(b) */else if(R[b]<R[i] && R[i]<R[a]){
				j=(int)d; k=(int)c; type=FIG_1B;
			}/* Fig. 1(c) */else if(R[a]<R[b] && R[b]<R[i]){
				j=(int)d; k=(int)pLPF_lt[before_gt[i]]; type=FIG_1C;
			}/* Fig. 1(d) */else if(R[b]<R[a] && R[a]<R[i]){
				j=(int)c; k=(int)pLPF_lt[before_gt[i]]; type=FIG_1D;
			}/* Fig. 1(e) */else if(R[i]<R[a] && R[a]<R[b]){
				j=(int)pLPF_gt[before_lt[i]]; k=(int)c; type=FIG_1E;
			}/* Fig. 1(f) */else if(R[i]<R[b] && R[b]<R[a]){
				j=(int)pLPF_gt[before_lt[i]]; k=(int)d; type=FIG_1F;
			}
		}else if(a>0 && b>0 && (before_lt[i]==NOT_EXIST || before_gt[i]==NOT_EXIST)){
			if(R[a]<R[b] && R[b]<R[i]) j=(int)d;
			else if(R[b]<R[a] && R[a]<R[i]) j=(int)c;
			else if(R[i]<R[a] && R[a]<R[b]) k=(int)c;
			else if(R[i]<R[b] && R[b]<R[a]) k=(int)d;
		}else if(a>0 && (before_gt==NULL || before_lt[i]==NOT_EXIST || before_gt[i]==NOT_EXIST))
		{
			if(R[a]<R[i]) j=(int)c;
			else k=(int)c;
		}else if(b>0 && (before_lt[i]==NOT_EXIST || before_gt[i]==NOT_EXIST))
		{
			if(R[b]<R[i]) j=(int)d;
			else k=(int)d;
		}
		if(j<0) j=0;
		if(k<0) k=0;
	}
	p.j=(int)j; p.k=(int)k; p.type=type;
	return p;
}

struct wtuple Omega_pmatch_manager_weak(short alg_type,int pvsize,int i,int pLPF_lt[],int pLPF_gt[],int before_lt[],int before_gt[],int R[])
{
	struct wtuple p;
	int a=0, b=0, c=0, d=0;
	int j=0, k=0; // implementation issue: needed to treat as int type, since -1 can be detected as invalid and reset
	short type=TRIVIAL;
	
	if(i>0 && before_lt!=NULL)
	{
		if(alg_type==PLRF) a=before_lt[i-1]+pvsize+2;
		else a=before_lt[i-1]+1; 
		
		c=pLPF_lt[i-1]-1;
		if(before_gt!=NULL)
		{
			if(alg_type==PLRF) b=before_gt[i-1]+pvsize+2;
			else b=before_gt[i-1]+1; 
			
			d=pLPF_gt[i-1]-1;
		}
		
		if(before_gt!=NULL && before_lt[i]!=NOT_EXIST && before_gt[i]!=NOT_EXIST)
		{
			if(a==0 && b==0){
				j=0; k=0;
			}else if(a==0 && R[b]<R[i]){
				j=(int)d; k=0;
			}else if(a==0 && R[b]>R[i]){
				j=0; k=(int)d;
			}else if(b==0 && R[a]<R[i]){
				j=(int)c; k=0;
			}else if(b==0 && R[a]>R[i]){
				j=0; k=(int)c;
			}/* Fig. 1(a) */else if(R[a]<R[i] && R[i]<R[b]){
				j=(int)c; k=(int)d; type=FIG_1A;
			}/* Fig. 1(b) */else if(R[b]<R[i] && R[i]<R[a]){
				j=(int)d; k=(int)c; type=FIG_1B;
			}/* Fig. 1(c) */else if(R[a]<R[b] && R[b]<R[i]){
				j=(int)d; k=0; type=FIG_1C;
			}/* Fig. 1(d) */else if(R[b]<R[a] && R[a]<R[i]){
				j=(int)c; k=0; type=FIG_1D;
			}/* Fig. 1(e) */else if(R[i]<R[a] && R[a]<R[b]){
				j=0; k=(int)c; type=FIG_1E;
			}/* Fig. 1(f) */else if(R[i]<R[b] && R[b]<R[a]){
				j=0; k=(int)d; type=FIG_1F;
			}
		}else if(a>0 && b>0 && (before_lt[i]==NOT_EXIST || before_gt[i]==NOT_EXIST)){
			if(R[a]<R[b] && R[b]<R[i]) j=(int)d;
			else if(R[b]<R[a] && R[a]<R[i]) j=(int)c;
			else if(R[i]<R[a] && R[a]<R[b]) k=(int)c;
			else if(R[i]<R[b] && R[b]<R[a]) k=(int)d;
		}else if(a>0 && (before_gt==NULL || before_lt[i]==NOT_EXIST || before_gt[i]==NOT_EXIST))
		{
			if(R[a]<R[i]) j=(int)c;
			else k=(int)c;
		}else if(b>0 && (before_lt[i]==NOT_EXIST || before_gt[i]==NOT_EXIST))
		{
			if(R[b]<R[i]) j=(int)d;
			else k=(int)d;
		}
		if(j<0) j=0;
		if(k<0) k=0;
	}
	p.j=(int)j; p.k=(int)k; p.type=type;
	return p;
}

struct wcases construct(short type,int out[],int arr1[],int arr2[],int R[],int pv1[],char typespv1[],int pv2[],char typespv2[],int pvsize)
{
	FILE* deciding_arr_out=fopen(DECIDING_ARR_FN,"wb");
	int i;
	int* z1=(int *)malloc((pvsize+1)*(sizeof(int)));
	int* z2=(int *)malloc((pvsize+1)*(sizeof(int)));
	struct wtuple p;
	struct wcases w;

	for(i=0; i<NUM_WCASES; i++)
		w.num[i]=0;
	if(z1 && z2)
	{
		for(i=0; i<pvsize; i++) z1[i]=z2[i]=0;
		
		for(i=0; i<pvsize; i++)
		{
			if(type==PBORDER)
			{
				p.j=0; p.k=0; p.type=TRIVIAL;
			}else if(type==PLRF)
			{
				p=Omega_pmatch_manager_weak(type,pvsize,i,z1,z2,arr1,arr2,R);
			}else
			{
				p=Omega_pmatch_manager(type,pvsize,i,z1,z2,arr1,arr2,R);
			}
			
			w.num[p.type]++;
						
			z1[i]=Lambda_pmatcher(i,arr1[i],p.j,pv1,typespv1,pv2,typespv2,pvsize);
			if(arr2!=NULL) z2[i]=Lambda_pmatcher(i,arr2[i],p.k,pv1,typespv1,pv2,typespv2,pvsize);
						
			if(z2[i]>z1[i]) {
				out[i]=z2[i];
				if(out[i]==0) write_int_or_int(deciding_arr_out,(int)NOT_EXIST,1);
				else write_int_or_int(deciding_arr_out,(int)arr2[i],1);
			}else{
				out[i]=z1[i];
				if(out[i]==0) write_int_or_int(deciding_arr_out,(int)NOT_EXIST,1);
				else write_int_or_int(deciding_arr_out,(int)arr1[i],1);
			}
		}
	}
	free(z1);free(z2);
	fclose(deciding_arr_out);
	return w;
}

struct wcases compute_pLPF(int pLPF[],int before_lt[],int before_gt[],int pSA[],int R[],int pv[],char typespv[],int pvsize)
{
	preprocess_SA_arrays(before_lt,BEFORE_LT,pSA,pvsize);
	preprocess_SA_arrays(before_gt,BEFORE_GT,pSA,pvsize);
	return construct(PLPF,pLPF,before_lt,before_gt,R,pv,typespv,pv,typespv,pvsize);
}

struct wcases compute_pLF(int pLF[],int pLneF[],int neq_lt[],int neq_gt[],int pLrF[],int rev_lt[],int rev_gt[],int pSA_T[],int R_T[],int pSA_Q[],int R_Q[],int pvQ1[],char typespvQ1[],int pvQ2[],char typespvQ2[],int pvsize,long double *t_plnef,long double *t_plrf)
{
	struct wcases w1,w2;
	long double start,end;
	int i;
	
	start=get_current_microseconds();
	w1=compute_pLneF(pLneF,neq_lt,neq_gt,pSA_T,R_T,pvQ1,typespvQ1,pvsize);	
	end=get_current_microseconds();
	(*t_plnef)=end-start;
	
	//printf("\n\npLneF="); display_int_arr(stdout,pLneF,pvsize);
	
	start=get_current_microseconds();
	w2=compute_pLrF(pLrF,rev_lt,rev_gt,pSA_Q,R_Q,pvQ1,typespvQ1,pvQ2,typespvQ2,pvsize);
	end=get_current_microseconds();
	(*t_plrf)=end-start;
	
	//printf("\n\pLrF="); display_int_arr(stdout,pLrF,pvsize);
	
	for(i=0; i<pvsize; i++)
	{
		if(pLneF[i]>pLrF[i]) pLF[i]=pLneF[i];
		else pLF[i]=pLrF[i];
	}
	
	for(i=0; i<NUM_WCASES; i++)
		w1.num[i]=w1.num[i]+w2.num[i];
	
	return w1;
}

struct wcases compute_pLneF(int pLneF[],int neq_lt[],int neq_gt[],int pSA[],int R[],int pv[],char typespv[],int pvsize)
{
	preprocess_SA_arrays(neq_lt,NEQ_LT,pSA,pvsize);
	preprocess_SA_arrays(neq_gt,NEQ_GT,pSA,pvsize);
	return construct(PLNEF,pLneF,neq_lt,neq_gt,R,pv,typespv,pv,typespv,pvsize);
}

struct wcases compute_pLrF(int pLrF[],int rev_lt[],int rev_gt[],int pSA[],int R[],int pvQ1[],char typespvQ1[],int pvQ2[],char typespvQ2[],int pvsize)
{
	preprocess_SA_arrays(rev_lt,REV_LT,pSA,pvsize);
	preprocess_SA_arrays(rev_gt,REV_GT,pSA,pvsize);
	return construct(PLRF,pLrF,rev_lt,rev_gt,R,pvQ1,typespvQ1,pvQ2,typespvQ2,pvsize);
}

struct wcases compute_pborder(int pborder[],int prefix[],int suffix[],int pSA[],int R[],int pv[],char typespv[],int pvsize)
{
	int i,j,c=0;
	struct wcases w;
	int *tmp=(int *)malloc((pvsize+1)*(sizeof(int)));
	
	for(i=0; i<pvsize; i++)
		pborder[i]=0;
		
	for(i=0; i<pvsize; i++)
	{
		if(i==0) 
			prefix[i]=suffix[i]=NOT_EXIST;
		else
		{
			if(R[0]<R[i]) 
			{
				prefix[i]=0;
				suffix[i]=NOT_EXIST;
			}else
			{
				prefix[i]=NOT_EXIST;
				suffix[i]=0;
			}
		}
	}
	w=construct(PBORDER,tmp,prefix,suffix,R,pv,typespv,pv,typespv,pvsize);
		
	for(i=1; i<pvsize; i+=j)
	{
		j=c=tmp[i];
		while(c>0 && c>pborder[i+c-1]) 
		{
			pborder[i+c-1]=c;
			c--;
		}
		j=1;
	}
	free(tmp);
	
	return w;
}

struct wcases_pair compute_pLCP_TCS(int pLCP[],int before_lt[],int after_lt[],int pSA[],int R[],int pv[],char typespv[],int pvsize)
{
	int * X=(int *)malloc((pvsize)*(sizeof(int)));
	int * Y=(int *)malloc((pvsize)*(sizeof(int)));
	int i;
	struct wcases_pair w;
	
	preprocess_SA_arrays(before_lt,BEFORE_LT,pSA,pvsize);
	w.w1=construct(PLCP_TCS,X,before_lt,NULL,R,pv,typespv,pv,typespv,pvsize);
	preprocess_SA_arrays(after_lt,AFTER_LT,pSA,pvsize);
	w.w2=construct(PLCP_TCS,Y,after_lt,NULL,R,pv,typespv,pv,typespv,pvsize);
	
	for(i=0; i<pvsize; i++)
	{
		if(X[i]>Y[i])
			pLCP[R[i]]=X[i];
		else
			pLCP[R[i]]=Y[i];
	}
	free(X);free(Y);
	return w;
}

struct wcases_pair compute_pLCP_JDA(short type,int pLCP[],int before_lt[],int after_lt[],int pSA[],int R[],int pv[],char typespv[],int pvsize)
{
	int * X=(int *)malloc((pvsize)*(sizeof(int)));
	int * Y=(int *)malloc((pvsize)*(sizeof(int)));
	int i;
	struct wcases_pair w;
	
	preprocess_SA_arrays(before_lt,BEFORE_LT,pSA,pvsize);
	w.w1=construct(PLCP_JDA,X,before_lt,NULL,R,pv,typespv,pv,typespv,pvsize);
	preprocess_SA_arrays(after_lt,AFTER_LT,pSA,pvsize);
	w.w2=construct(PLCP_JDA,Y,after_lt,NULL,R,pv,typespv,pv,typespv,pvsize);
	
	for(i=0; i<pvsize; i++)
	{
		if(type==PERMUTED_PLCP_JDA)
		{
			if(X[i]>Y[i])
				pLCP[i]=X[i];
			else
				pLCP[i]=Y[i];
		}else if(type==PLCP_JDA)
		{
			if(X[i]>Y[i])
				pLCP[R[i]]=X[i];
			else
				pLCP[R[i]]=Y[i];
		}
	}
	free(X);free(Y);
	return w;
}

struct wcases compute_pLCP_JDA_IMP(short type,int pLCP[],int M[],int pSA[],int R[],int pv[],char typespv[],int pvsize)
{
	int * X=(int *)malloc((pvsize)*(sizeof(int)));
	int i;
	struct wcases w;
	
	if(pvsize>0) M[pSA[0]]=NOT_EXIST;
	
	for(i=1; i<pvsize; i++)
		M[pSA[i]]=pSA[i-1];
	w=construct(PLCP_JDA_IMP,X,M,NULL,R,pv,typespv,pv,typespv,pvsize);
	
	switch(type)
	{
		case PERMUTED_PLCP_JDA_IMP: 
			for(i=0; i<pvsize; i++)
				pLCP[i]=X[i];
			break;
		case PLCP_JDA_IMP:
			for(i=0; i<pvsize; i++)
				pLCP[i]=X[pSA[i]];
			break;
	}return w;
}

struct wcases compute_pLCP_TCS_IMP(int pLCP[],int M[],int pSA[],int R[],int pv[],char typespv[],int pvsize)
{
	int i;
	struct wcases w;
	int * tmp=(int *)malloc((pvsize)*(sizeof(int)));

	for(i=0; i<pvsize; i++)
	{
		M[i]=NOT_EXIST;
		pLCP[R[i]]=i;
	}
	for(i=1; i<pvsize; i++)
		M[pLCP[i]]=pLCP[i-1];
	w=construct(PLCP_TCS_IMP,tmp,M,NULL,R,pv,typespv,pv,typespv,pvsize);
	for(i=0; i<pvsize; i++)
		pLCP[R[i]]=tmp[i];
	free(tmp);
	
	return w;
}

short correct_pLPF_variant(short type,int R[],int answer[],int cand_arr1[],int cand_arr2[],int pv1[],char typespv1[],int pv2[],char typespv2[],int pvsize)
{
	short correct=1;
	int i, a, b, q1, q2, q, x, y;
	
	if(DEBUG>0) printf("\n\npLPF_variant=");
	
	for(i=0; i<pvsize /*&& correct!=0*/; i++)
	{
		a=cand_arr1[i];
		if(cand_arr2!=NULL) b=cand_arr2[i];
		q1=q2=0;
		if(a!=NOT_EXIST){
			while(a+q1<pvsize && i+q1<pvsize){
				if(typespv2[a+q1]==CONSTANT && typespv1[i+q1]==CONSTANT){
					if(pv2[a+q1]!=pv1[i+q1]) break;
				}else if(typespv2[a+q1]==DISTANCE && typespv1[i+q1]==DISTANCE){
					if(a+q1-pv2[a+q1]<a) x=0;
					else x=pv2[a+q1];
					if(i+q1-pv1[i+q1]<i) y=0;
					else y=pv1[i+q1];
					if(x!=y) break;
				}else break;
				q1++;
			}
		}
		if(cand_arr2!=NULL && b!=NOT_EXIST){
			while(b+q2<pvsize && i+q2<pvsize){
				if(typespv2[b+q2]==CONSTANT && typespv1[i+q2]==CONSTANT){
					if(pv2[b+q2]!=pv1[i+q2]) break;
				}else if(typespv2[b+q2]==DISTANCE && typespv1[i+q2]==DISTANCE){
					if(b+q2-pv2[b+q2]<b) x=0;
					else x=pv2[b+q2];
					if(i+q2-pv1[i+q2]<i) y=0;
					else y=pv1[i+q2];
					if(x!=y) break;
				}else break;
				q2++;
			}
		}
		if(q1>q2) q=q1;
		else q=q2;
		
		if(type==PLCP_TCS || type==PLCP_TCS_IMP || type==PLCP_JDA || type==PLCP_JDA_IMP)
		{
			if(answer[R[i]]!=q) correct=0;
		}
		else if(answer[i]!=q) correct=0;
		
		if(DEBUG>0) printf("[%d]",q);
	}
	
	if(DEBUG>0) printf("\n\n");
	
	//printf("\n\ncand_arr1="); display_int_arr(stdout,cand_arr1,pvsize); printf("\n");
	//printf("\n\ncand_arr2="); display_int_arr(stdout,cand_arr2,pvsize); printf("\n");	
	//printf("\n\nR="); display_int_arr(stdout,R,pvsize);
	return correct;
}


#endif
