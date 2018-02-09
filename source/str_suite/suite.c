//
//
// KNOWN PROBLEMS:
//
// (1) when the algorithm is PLRF, in construct() for correctness, we need to either:
//       (a) set p.j=0 and p.k=0 rather than using Omega_pmatch_manager, or, better yet
//       (b) use Omega_pmatch_manager_weak, which avoids before< & before> indexing into pLPF< & pLPF>
//     the problem may be in the use of the extended Rank array (R) and how rev_lt_array and rev_gt_array are adjusted for prevQ2
//
// (2) NOTE: pLneF is invalid when considering pLF from the random generated string from U(1,64); ok when pLneF alone because we use FREE_CONST as the terminator, which constant>$
// (3) Only need to make remap_for_fun of size ASCII_ALPHABET_SIZE; fixes (1) too much memory and (2) error for small strings when the f-bytes-length remap_for_fun wasn't large enough to remap the ASCII symbols;  NEED TO FIX THIS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <sys/time.h>
#include <float.h>

#include "pcode_sorts/mergesort_pcode_d.c"
#include "pcode_sorts/mergesort_pcode_f.c"
#include "../other/radixsort.c"
#include "trad_sarray/ssarray.c" // modified from darmouth.edu
#include "../other/doublyLinkedList.h"  // modified from course.com; Malik textbook
#include "../other/filefuns.c"
#include "../other/utilfuns.c"
#include "pac.c"
#include "pstring.c"
#include "string_ds.c"
#include "pstring_ds.c"
#include "types.c"


/*		
    *** use argv[1-4] for algorithm PLCP_NAIVE,PLPF; 
	*** use argv[1-7] for algorithm AVERAGE_PSA,WORST_CASE_PSA2,WORST_CASE_PSA3,PSA_FOR_PLRF_AND_PLF;
	*** use argv[1-4] for algorithm TRADITIONAL_SA_LCP (argv[3]=""),PLPF,PLCP_TCS,PLCP_TCS_IMP,PLCP_JDA,PERMUTED_PLCP_JDA,PLCP_JDA_IMP,PERMUTED_PLCP_JDA_IMP,PLNEF,PLRF,PSA_FOR_PLRF_AND_PLF,PLF,PBORDER;

	argv[1]: algorithm(0:plcp_given_psa_from_file,1:avgcase,{2,3}:worstcase,4:traditional)
	argv[2]: filename
	argv[3]: parameter_list_or_NO_PARAM_for_none
	argv[4]: compute_pSA_using_only_#bytes
	argv[5]: make_blocks_with_pAC()_y/n
	argv[6]: mblock_size(zero_for_largest_valid)
	argv[7]: validate_pSA_correctness_y/n
*/
int main(int argc, char* argv[])
{
	int start = 0;
	int i,j,k,z,v,mid;
	int block = 3, mblock;
	int psuffixq,removei,addm;
	int fbytes = 0, unique_symbols = 0, temp;
	char* fn=NULL;
	short ALGORITHM,correct,avg;
	long double start_sec, end_sec;
	char* PARAMS;
	char uctmp[BUF_SIZE];
	FILE* f_psa_out=NULL;
	FILE* f_sa_out=NULL;
	struct wcases w;
	
	for(i=0; i<argc; i++) printf("argv[%d]=%s   |   ", i, argv[i]);
	printf("\n\n");
	
	sprintf(PSA_FILE,"%s%c",PSA_FN,'\0');
	sprintf(SA_FILE,"%s%c",SA_FN,'\0');
	
	printf("\n\nin psa.c\n\n");
	fflush(stdout);
	
	if(argc > 2 && strlen(argv[1]) > 0 && strlen(argv[2]) > 0) 
	{
		ALGORITHM=atoi(argv[1]);
		
		fn=argv[2];
		fbytes=get_file_len2(fn);
		
		if(argc > 4 && strlen(argv[4]) > 0)
		{
			temp=strtoul(argv[4],NULL,10);
			if(temp>0 && temp<fbytes) 
				fbytes=temp;
			//printf("temp=%d,fbytes=%d,argv[4]=%s\n",temp,fbytes,argv[4]);
		}
		
		if(fbytes<=RSORT_CONST)
		{	
			printf("\n\n*** Trivial (try inter string) ***\n\n");
			return SUCCESS1;
		}
		
		if(argc > 5 && strlen(argv[5]) > 0)
		{
			if(argv[5][0]=='Y' || argv[5][0]=='y')
				MAKE_BLOCKS=1;
			else MAKE_BLOCKS=0;
		}
		
		if(argc > 6 && strlen(argv[6]) > 0)
		{
			temp=strtol(argv[6],NULL,10);
			if(temp<=0 || temp>=fbytes)
			{
				FIND_MAX_BLOCK=1;
			}
			else
			{
				FIND_MAX_BLOCK=0;
				block=temp;
			}
		}

		if(argc > 7 && strlen(argv[7]) > 0)
		{
			if(argv[7][0]=='Y' || argv[7][0]=='y')
				VERIFY_PSA=1;
			else VERIFY_PSA=0;
		}
		
		if(ALGORITHM != TRADITIONAL_SA_LCP && argc > 3 && strlen(argv[3]) > 0 && argv[3][0]!=NO_PARAM)
		{
			for(i=0; i<strlen(argv[3]); i++)
				uctmp[i]=(char)(argv[3][i]);
			uctmp[i]='\0';
			setAlphaType(ALGORITHM,uctmp,alpha_type);
			PARAMS=argv[3];
		}
		else
		{
			setAlphaType(ALGORITHM,NULL,alpha_type);
			PARAMS=NULL;
		}

		if(fbytes>0)
		{
			printf("fbytes=%d>0\n\n",fbytes); fflush(stdout);
			
			if(ALGORITHM==PSA_FOR_PLRF_AND_PLF || ALGORITHM==PLRF || ALGORITHM==PLF)
			{
				fbytes*=2; // consider T^R
				fbytes++; // consider also ending symbol FREE_CONST
				// now, fbytes=|<T><FREE_CONST><T^R>|
			}
			
			fbuffer=(char *)malloc(fbytes+1);
			
			if(ALGORITHM==LCP_KASAI)
			{
				printf("running KASAI LCP algorithm\n\n");
				
				lcparray=(int *)malloc((fbytes)*(sizeof(int)));
				suffixarray=(int *)malloc((fbytes)*(sizeof(int)));
				Rankarray=(int *)malloc((fbytes)*(sizeof(int)));
				
				if(fbuffer && suffixarray && Rankarray)
				{
					readFile(fn,fbuffer,fbytes);
					fbuffer[fbytes]=0;
						
					printf(": reading length-%d SA from %s\n\n",fbytes,SA_FILE);
									
					if(read_int_arr(SA_FILE,suffixarray,fbytes)==FAILED1)
						return FAILED1;
					construct_rank_array(Rankarray,suffixarray,fbytes);
					
					start_sec=get_current_microseconds();
					compute_LCP_Kasai(fbuffer,lcparray,suffixarray,Rankarray,fbytes);
					end_sec=get_current_microseconds();
					printf("\n\n*** Kasai_LCP executed for a duration of %.0Lf usec or %.10Lf sec (from %.0Lf to %.0Lf) ***\n\n",end_sec-start_sec,(end_sec-start_sec)/1000000.0,start_sec,end_sec);
					
					display_int_arr(stdout,lcparray,fbytes);
				}
			}else if(ALGORITHM==LPF_DIRECT_CROCHEMORE)
			{
				printf("running CROCHEMORE LPF DIRECT algorithm\n\n");
				
				suffixarray=(int *)malloc((fbytes)*(sizeof(int)));
				lpfarray=(int *)malloc((fbytes)*(sizeof(int)));
				before_lt_array=(int *)malloc((fbytes)*(sizeof(int)));
				before_gt_array=(int *)malloc((fbytes)*(sizeof(int)));
				if(fbuffer && suffixarray && lpfarray && before_lt_array && before_gt_array)
				{
					readFile(fn,fbuffer,fbytes);
					fbuffer[fbytes]=0;
						
					printf(": reading length-%d SA from %s\n\n",fbytes,SA_FILE);
									
					if(read_int_arr(SA_FILE,suffixarray,fbytes)==FAILED1)
						return FAILED1;
					
					start_sec=get_current_microseconds();
					compute_LPF_direct_Crochemore(fbuffer,NULL,lpfarray,before_lt_array,before_gt_array,suffixarray,fbytes);
					end_sec=get_current_microseconds();
					printf("\n\n*** CROCHEMORE_LPF_DIRECT executed for a duration of %.0Lf usec or %.10Lf sec (from %.0Lf to %.0Lf) ***\n\n",end_sec-start_sec,(end_sec-start_sec)/1000000.0,start_sec,end_sec);
					
					//display_int_arr(stdout,lpfarray,fbytes);
				}
			}else
			{
				printf("ELSE part BEFORE allocations\n");
				
				prevT=(int *)malloc((fbytes+1)*(sizeof(int)));
				forwT=(int *)malloc((fbytes+1)*(sizeof(int)));
				psuffixarray=(int *)malloc((fbytes)*(sizeof(int)));
				types_prevT=(char *)malloc(fbytes+1);
				remap_prev_size=fbytes+MAP_SIZE;
				remap_prev=(int *)malloc((remap_prev_size)*(sizeof(int)));
				prevT[fbytes]=0;
				forwT[fbytes]=0;
				types_prevT[fbytes]=0;
			
				printf("ELSE part AFTER allocations\n");
				
				if(fbuffer && prevT && forwT && types_prevT && psuffixarray)
				{
					if(ALGORITHM==PSA_FOR_PLRF_AND_PLF || ALGORITHM==PLRF || ALGORITHM==PLF) // construct, fbuffer=<T><FREE_CONST><T^R>
					{
						v=(fbytes-1)/2;
						readFile(fn,fbuffer,v);
						fbuffer[v]=FREE_CONST;
						fbuffer[fbytes]=0;
						z=v+1;
						for(k=v-1; k>=0; k--)
						{
							fbuffer[z++]=fbuffer[k];
							if(k==0) break; /* since no negative in int */
						}
					}
					else
					{
						printf("BEFORE read...\n");fflush(stdout);
						readFile(fn,fbuffer,fbytes);
						printf("AFTER read...\n");fflush(stdout);
						fbuffer[fbytes]=0;
					}
					
					//
					//printf("\nT=");
					//for(k=0; k<fbytes; k++) printf("%c",fbuffer[k]);
					//printf("\n\n");
					//
					
					unique_symbols=getAlphaFreq(fbuffer,fbytes,alpha_freq,remap_alpha,NO);
					max_distance=prev(prevT,types_prevT,fbuffer,alpha_type,fbytes);
					unique_symbols=remapPrevAlpha(prevT,types_prevT,max_distance,fbytes,remap_prev,remap_prev_size);
					prev_cdf_size=unique_symbols+1;
					#if DEBUG>1
						printf("cdf size: %d\n",prev_cdf_size);
					#endif
					prev_cdf=(long double *)malloc(prev_cdf_size*(sizeof(long double)));
					if(prev_cdf)
					{
						printf("if(prev_cdf) A\n");
						make_uniform_cdf(prev_cdf_size,prev_cdf);
						probability=1.0/unique_symbols;
						
						if(FIND_MAX_BLOCK!=0) 
						{
							#if DEBUG>1
								printf("\n\nFinding max block...\n\n");
							#endif
							block=max_block(fbytes,prev_cdf);
						}
						#if DEBUG>1
							printf("block=%d\n\n",block);
						#endif
						
						epsilon=calc_epsilon(block,prev_cdf);
						if(epsilon<=0) return FAILED1;
						
						forw(fbuffer,forwT,prevT,fbytes,alpha_type);
						
						printf("if(prev_cdf) B\n");
						
						if(DEBUG!=0) 
						{
							//print_psuffixes(0,block,prevT,forwT,fbytes,types_prevT,max_distance,prev_cdf,remap_prev,probability);
							//printf("-------------------------\n");
							print_psuffixes(4,block,prevT,forwT,fbytes,types_prevT,max_distance,prev_cdf,remap_prev,probability);
						}
						
						if(MAKE_BLOCKS==0)
						{
							printf("if(prev_cdf) B.1\n");
							
							RECALCULATE_PAC=find_recalculate_pAC_window(block,prevT,forwT,fbytes,types_prevT,max_distance,prev_cdf,remap_prev,probability);
							
							printf("if(prev_cdf) B.2, can use %d transitions before resetting\n", RECALCULATE_PAC);
							
							// a little safety net
							//RECALCULATE_PAC=RECALCULATE_PAC-2;
							//
							
							if(RECALCULATE_PAC<=0) 
							{
								MAKE_BLOCKS=1;
								RECALCULATE_PAC=1;
							}
							
							#if DEBUG>1
								printf("RECALCULATE_PAC=%d\n\n",RECALCULATE_PAC);
							#endif
						}
						
						printf("if(prev_cdf) C\n");
						if(ALGORITHM==AVERAGE_PSA) block=(int)(ceil(log2(fbytes)));
						printf("if(prev_cdf) D\n");
						fflush(stdout);
						printf("\nfilename=\"%s\"\nsize=%d\nblock=%d\nMAKE_BLOCKS=%d\nRECALCULATE_PAC=%d\nPARAMS=\"%s\" where ALL_PARAM=%c and NO_PARAM=%c\nunique_symbols(in prev(T))=%d\nuniform_probability=%.100Lf\nepsilon=%.100Lf\n\n",fn,fbytes,block,(int)MAKE_BLOCKS,(int)RECALCULATE_PAC,PARAMS,ALL_PARAM,NO_PARAM,(int)unique_symbols,probability,epsilon);
						fflush(stdout);
						
						switch(ALGORITHM)
						{
							case PLCP_NAIVE:
							case PLPF:
							case PLNEF:
							case PLCP_TCS:
							case PLCP_TCS_IMP:
							case PLCP_JDA:
							case PERMUTED_PLCP_JDA:
							case PLCP_JDA_IMP:
							case PERMUTED_PLCP_JDA_IMP:
							case PLRF:
							case PLF:
							case PBORDER:
								if(ALGORITHM==PLCP_NAIVE) printf("pLCP computation");
								else if(ALGORITHM==PLPF) printf("pLPF computation");
								else if(ALGORITHM==PLNEF) printf("pLneF computation");
								else if(ALGORITHM==PLCP_TCS || ALGORITHM==PLCP_TCS_IMP) printf("pLCP computation");
								else if(ALGORITHM==PLRF) printf("pLrF computation");
								else if(ALGORITHM==PBORDER) printf("p-border computation");
								else if(ALGORITHM==PLF) printf("pLF computation");
															
								printf(": reading length-%d pSA from %s\n\n",fbytes,PSA_FILE);
								
								if(read_int_arr(PSA_FILE,psuffixarray,fbytes)==FAILED1)
									return FAILED1;
								
								if(ALGORITHM==PLCP_NAIVE)
								{
									plcparray=(int *)malloc((fbytes)*(sizeof(int)));
									if(plcparray)
									{
										start_sec=get_current_microseconds();
										
										compute_pLCP(fbytes,prevT,types_prevT,plcparray,psuffixarray);
										
										end_sec=get_current_microseconds();
										printf("\n\n*** naive_pLCP executed for a duration of %.0Lf usec or %.10Lf sec (from %.0Lf to %.0Lf) ***\n\n",end_sec-start_sec,(end_sec-start_sec)/1000000.0,start_sec,end_sec);
								
										printf("\n\nDisplaying pLCP...\n\n");
										display_int_arr(stdout,plcparray,fbytes);
										printf("\n\nmean(pLCP)=%.10Lf\nmax(pLCP)=%d\n\n",mean(fbytes,plcparray),max(fbytes,plcparray));
									}
								}else if(ALGORITHM==PLPF)
								{
									plpfarray=(int *)malloc((fbytes)*(sizeof(int)));
									Rankarray=(int *)malloc((fbytes)*(sizeof(int)));
									before_lt_array=(int *)malloc((fbytes)*(sizeof(int)));
									before_gt_array=(int *)malloc((fbytes)*(sizeof(int)));
									if(plpfarray && Rankarray && before_lt_array && before_gt_array)
									{
										execute_pLPF_variant(PLPF,psuffixarray,Rankarray,fbytes,plpfarray,before_lt_array,before_gt_array,NULL,NULL,prevT,types_prevT,prevT,types_prevT,prevT,types_prevT);
									}
								}else if(ALGORITHM==PLCP_TCS)
								{
									printf("\nIn pLCP_TCS...\n\n");
									
									plcparray=(int *)malloc((fbytes)*(sizeof(int)));
									Rankarray=(int *)malloc((fbytes)*(sizeof(int)));
									before_lt_array=(int *)malloc((fbytes)*(sizeof(int)));
									after_lt_array=(int *)malloc((fbytes)*(sizeof(int)));
									if(plcparray && Rankarray && before_lt_array && after_lt_array)
									{
										execute_pLPF_variant(PLCP_TCS,psuffixarray,Rankarray,fbytes,plcparray,before_lt_array,after_lt_array,NULL,NULL,prevT,types_prevT,prevT,types_prevT,prevT,types_prevT);
									}
								}else if(ALGORITHM==PLCP_TCS_IMP)
								{
									printf("\nIn pLCP_TCS_IMP...\n\n");
									
									plcparray=(int *)malloc((fbytes)*(sizeof(int)));
									Rankarray=(int *)malloc((fbytes)*(sizeof(int)));
									neighbor_array=(int *)malloc((fbytes)*(sizeof(int)));
									if(plcparray && Rankarray && neighbor_array)
									{
										execute_pLPF_variant(PLCP_TCS_IMP,psuffixarray,Rankarray,fbytes,plcparray,neighbor_array,NULL,NULL,NULL,prevT,types_prevT,prevT,types_prevT,prevT,types_prevT);
									}
								}else if(ALGORITHM==PLCP_JDA || ALGORITHM==PERMUTED_PLCP_JDA)
								{
									printf("\nIn pLCP_JDA...\n\n");
									
									plcparray=(int *)malloc((fbytes)*(sizeof(int)));
									Rankarray=(int *)malloc((fbytes)*(sizeof(int)));
									before_lt_array=(int *)malloc((fbytes)*(sizeof(int)));
									before_gt_array=(int *)malloc((fbytes)*(sizeof(int)));
									if(plcparray && Rankarray && before_lt_array && before_gt_array)
									{
										execute_pLPF_variant(ALGORITHM,psuffixarray,Rankarray,fbytes,plcparray,before_lt_array,before_gt_array,NULL,NULL,prevT,types_prevT,prevT,types_prevT,prevT,types_prevT);
									}
								}else if(ALGORITHM==PLCP_JDA_IMP  || ALGORITHM==PERMUTED_PLCP_JDA_IMP)
								{
									printf("\nIn pLCP_JDA_IMP...\n\n");
									
									plcparray=(int *)malloc((fbytes)*(sizeof(int)));
									Rankarray=(int *)malloc((fbytes)*(sizeof(int)));
									neighbor_array=(int *)malloc((fbytes)*(sizeof(int)));
									if(plcparray && Rankarray && neighbor_array)
									{
										execute_pLPF_variant(ALGORITHM,psuffixarray,Rankarray,fbytes,plcparray,neighbor_array,NULL,NULL,NULL,prevT,types_prevT,prevT,types_prevT,prevT,types_prevT);
									}
								}else if(ALGORITHM==PLNEF || ALGORITHM==PLRF || ALGORITHM==PLF)
								{
									if(ALGORITHM==PLNEF)
									{
										plnefarray=(int *)malloc((fbytes)*(sizeof(int)));
										Rankarray=(int *)malloc((fbytes)*(sizeof(int)));
										neq_lt_array=(int *)malloc((fbytes)*(sizeof(int)));
										neq_gt_array=(int *)malloc((fbytes)*(sizeof(int)));
										if(plnefarray && Rankarray && neq_lt_array && neq_gt_array)
										{
											if(ALGORITHM!=PLF)
												execute_pLPF_variant(PLNEF,psuffixarray,Rankarray,fbytes,plnefarray,neq_lt_array,neq_gt_array,NULL,NULL,prevT,types_prevT,prevT,types_prevT,prevT,types_prevT);
										}
									}
									if(ALGORITHM==PLRF || ALGORITHM==PLF)
									{
										v=(fbytes-1)/2;
										mid=v;
										
										//printf("v=%d\n\n",v);
										
										prevQ1=(int *)malloc((v)*(sizeof(int)));
										types_prevQ1=(char *)malloc((v)*(sizeof(char)));
										prevQ2=(int *)malloc((v)*(sizeof(int)));
										types_prevQ2=(char *)malloc((v)*(sizeof(char)));
										if(ALGORITHM!=PLF) 
										{
											plrfarray=(int *)malloc((v)*(sizeof(int)));
										}
										Rankarray=(int *)malloc((fbytes)*(sizeof(int)));
										rev_lt_array=(int *)malloc((v)*(sizeof(int)));
										rev_gt_array=(int *)malloc((v)*(sizeof(int)));
										
										if(((plrfarray && ALGORITHM==PLRF) || ALGORITHM==PLF) && prevQ1 && types_prevQ1 && prevQ2 && types_prevQ2 && Rankarray && rev_lt_array && rev_gt_array)
										{
											// make prevQ1 and types_prevQ1 from prevT
											for(k=0; k<v; k++)
											{
												prevQ1[k]=prevT[k];
												types_prevQ1[k]=types_prevT[k];
											}
											
											//printf("prevQ1=");for(k=0; k<v; k++) printf("[%d]",prevQ1[k]); printf("\n\n");
											//printf("types_prevQ1=");for(k=0; k<v; k++) printf("[%d]",types_prevQ1[k]); printf("\n\n");

											// make prevQ2 and types_prevQ2 from prevT
											for(k=0; k<v; k++)
											{
												prevQ2[k]=prevT[k+v+1];
												types_prevQ2[k]=types_prevT[k+v+1];
												if(types_prevQ2[k]==DISTANCE && prevQ2[k]>k) // reset distance
													prevQ2[k]=0;
											}									

											//printf("prevQ2=");for(k=0; k<v; k++) printf("[%d]",prevQ2[k]); printf("\n\n");
											//printf("types_prevQ2=");for(k=0; k<v; k++) printf("[%d]",types_prevQ2[k]); printf("\n\n");
																			
											if(ALGORITHM==PLRF)
												execute_pLPF_variant(PLRF,psuffixarray,Rankarray,v,plrfarray,rev_lt_array,rev_gt_array,NULL,NULL,prevT,types_prevT,prevQ1,types_prevQ1,prevQ2,types_prevQ2);
											if(ALGORITHM==PLF)
											{
												pLF_array=(int *)malloc((v)*(sizeof(int)));
												neq_lt_array=(int *)malloc((v)*(sizeof(int)));
												neq_gt_array=(int *)malloc((v)*(sizeof(int)));
										
												if(pLF_array && Rankarray && neq_lt_array && neq_gt_array && rev_lt_array && rev_gt_array && prevQ1 && types_prevQ1 && prevQ2 && types_prevQ2)
												{
													execute_pLPF_variant(PLF,psuffixarray,Rankarray,v,pLF_array,neq_lt_array,neq_gt_array,rev_lt_array,rev_gt_array,prevT,types_prevT,prevQ1,types_prevQ1,prevQ2,types_prevQ2);	
												}
											}
										}
									}
								}else if(ALGORITHM==PBORDER)
								{
									pborderarray=(int *)malloc((fbytes)*(sizeof(int)));
									Rankarray=(int *)malloc((fbytes)*(sizeof(int)));
									prefix_array=(int *)malloc((fbytes)*(sizeof(int)));
									suffix_array=(int *)malloc((fbytes)*(sizeof(int)));
									if(pborderarray && Rankarray && prefix_array && suffix_array)
									{
										execute_pLPF_variant(ALGORITHM,psuffixarray,Rankarray,fbytes,pborderarray,prefix_array,suffix_array,NULL,NULL,prevT,types_prevT,prevT,types_prevT,prevT,types_prevT);
									}
								}							
								
								break;

							case NAIVE_PBORDER:
								pborderarray=(int *)malloc((fbytes)*(sizeof(int)));
								
								start_sec=get_current_microseconds();
								correct_pborder(pborderarray,prevT,types_prevT,fbytes);
								end_sec=get_current_microseconds();
								printf("\n\n*** naive_pborder executed for a duration of %.0Lf usec or %.10Lf sec (from %.0Lf to %.0Lf) ***\n\n",end_sec-start_sec,(end_sec-start_sec)/1000000.0,start_sec,end_sec);
								break;
								
							case AVERAGE_PSA: 
								printf("running p_suffix_sort_pAC1 (average case)\n\n");
								
								start_sec=get_current_microseconds();
								
								avg_case=p_suffix_sort_pAC1(block,psuffixarray,prevT,forwT,fbytes,types_prevT,max_distance,prev_cdf,remap_prev,probability);
								
								end_sec=get_current_microseconds();
								printf("\n\n*** p_suffix_sort_pAC1 executed for a duration of %.0Lf usec or %.10Lf sec (from %.0Lf to %.0Lf) ***\n\n",end_sec-start_sec,(end_sec-start_sec)/1000000.0,start_sec,end_sec);
								
								if(avg_case==0) printf("\n*** FYI: not completely sorted ***\n\n");
								break;
								
							case PSA_FOR_PLRF_AND_PLF:
							case WORST_CASE_PSA2:
								printf("running p_suffix_sort_pAC2 (worst case)\n\n");
								
								if(ALGORITHM==PSA_FOR_PLRF_AND_PLF) printf("... pSA for pLrF data structure ... \n\n");

												
								start_sec=get_current_microseconds();
								
								p_suffix_sort_pAC2(block,psuffixarray,prevT,forwT,fbytes,types_prevT,max_distance,prev_cdf,remap_prev,probability);
								
								end_sec=get_current_microseconds();
								printf("\n\n*** p_suffix_sort_pAC2 executed for a duration of %.0Lf usec or %.10Lf sec (from %.0Lf to %.0Lf) ***\n\n",end_sec-start_sec,(end_sec-start_sec)/1000000.0,start_sec,end_sec);
								break;
								
							case WORST_CASE_PSA3:
								printf("running p_suffix_sort_pAC3 (improved worst case)\n\n");
								
								start_sec=get_current_microseconds();
								
								avg=p_suffix_sort_pAC3(block,psuffixarray,prevT,forwT,fbytes,types_prevT,max_distance,prev_cdf,remap_prev,probability);
								
								end_sec=get_current_microseconds();
								printf("\n\n*** p_suffix_sort_pAC3 executed for a duration of %.0Lf usec or %.10Lf sec (from %.0Lf to %.0Lf) ***\n\n",end_sec-start_sec,(end_sec-start_sec)/1000000.0,start_sec,end_sec);
								
								if(avg!=0) printf("\n\n*** Solved with average case algorithm ***\n\n");
								break;
							
							case TRADITIONAL_SA_LCP:
							case TRADITIONAL_SA:
								printf("running traditional suffix sort ssarray\n\n");
								fflush(stdout);
								start_sec=get_current_microseconds();
								
								intfbuffer=(int *)malloc(((fbytes+1)*(sizeof(int))));
								remap_for_fun=(int *)malloc((fbytes+1)*sizeof(int));
								
								if(intfbuffer && remap_for_fun)
								{
									// symbols before
									//for(i=0; i<fbytes; i++) printf("[%d]",fbuffer[i]);
									//printf("\n");
									//
									
									for(i=0; i<fbytes; i++)
									{
										intfbuffer[i]=(int)(fbuffer[i]);
										remap_for_fun[i]=0;
									}
									intfbuffer[i]=0; // the $
									
									
									uintfbuffer=(int *)malloc(((fbytes+1)*(sizeof(int))));
									
									if(uintfbuffer)
									{
										for(i=0; i<fbytes; i++)
											uintfbuffer[i]=(int)(intfbuffer[i]);
										radix_sort_uint(uintfbuffer,fbytes,RSORT_CONST);
										for(i=0; i<fbytes; i++)
											intfbuffer[i]=(int)(uintfbuffer[i]);
										
										i=0;
										j=1;
										do
										{
											remap_for_fun[intfbuffer[i]]=j++;
											while(++i<fbytes && intfbuffer[i]==intfbuffer[i-1]);
										}while(intfbuffer[i]!=0);
										
										for(i=0; i<fbytes; i++)
											intfbuffer[i]=remap_for_fun[(int)(fbuffer[i])];
										
										// remapped symbols after
										//for(i=0; i<fbytes; i++) printf("[%d]",intfbuffer[i]);
										//printf("\n");
										//
										
										if(ssarray(intfbuffer)<0)
											printf("*** Problem with ssarray sort ***\n\n");
										else
										{
											for(i=1; i<=fbytes; i++) // leave out [$] suffix
											{
												psuffixarray[i-1]=intfbuffer[i];
											}
											
											if(ALGORITHM==TRADITIONAL_SA)
											{
												f_sa_out=fopen(SA_FILE,"wb");
												if(f_sa_out)
													display_int_arr(f_sa_out,psuffixarray,fbytes);
												fclose(f_sa_out);
											}
											else if(ALGORITHM==TRADITIONAL_SA_LCP)
											{
												plcparray=(int *)malloc((fbytes)*(sizeof(int)));
												if(plcparray)
												{
													compute_pLCP(fbytes,prevT,types_prevT,plcparray,psuffixarray);
													printf("\n\nmean(LCP)=%.10Lf\nmax(LCP)=%d\n\n",mean(fbytes,plcparray),max(fbytes,plcparray));
												}
											}
										}
									}
								}
									
								end_sec=get_current_microseconds();
								printf("\n\n*** traditional ssarray executed for a duration of %.0Lf usec or %.10Lf sec (from %.0Lf to %.0Lf) ***\n\n",end_sec-start_sec,(end_sec-start_sec)/1000000.0,start_sec,end_sec);
								break;
														
							default: return SUCCESS1;
						}
						
						if(ALGORITHM==AVERAGE_PSA || ALGORITHM==WORST_CASE_PSA2 || ALGORITHM==WORST_CASE_PSA3 || ALGORITHM==TRADITIONAL_SA_LCP || ALGORITHM==TRADITIONAL_SA || ALGORITHM==PSA_FOR_PLRF_AND_PLF){
							printf("\n\nDisplaying pSA...\n\n");
							//display_pSA(psuffixarray,prevT,fbytes,types_prevT);
							
							if(ALGORITHM==TRADITIONAL_SA_LCP || ALGORITHM==TRADITIONAL_SA) f_psa_out=fopen(SA_FILE,"wb");
							else f_psa_out=fopen(PSA_FILE,"wb");
							if(f_psa_out)
								display_int_arr(f_psa_out,psuffixarray,fbytes);
							fclose(f_psa_out);
						}
						if(VERIFY_PSA!=0)
						{
							printf("\n\nVerifying correct pSA...\n\n");
							correct=correct_pSA(psuffixarray,prevT,fbytes,types_prevT);
							if(correct==0) printf("\n*** Invalid pSA (consider cascading standard floating-point arithmetic error) ***\n\n");
							else printf("\n*** Correct pSA ***\n\n");
						}
					}
				}
			}
		}
	}
	
	free(fbuffer);free(prevT);free(forwT);free(psuffixarray);free(types_prevT);free(remap_prev);free(prev_cdf);free(plcparray);free(intfbuffer);free(remap_for_fun);free(plpfarray);free(Rankarray);free(before_lt_array);free(before_gt_array);free(plnefarray);free(neq_lt_array);free(neq_gt_array);free(after_lt_array);free(after_gt_array);free(rev_lt_array);free(rev_gt_array);free(neighbor_array);free(plrfarray);free(prevQ1);free(types_prevQ1);free(prevQ2);free(types_prevQ2);free(pborderarray);free(prefix_array);free(suffix_array);free(pLF_array);free(lcparray);free(suffixarray);free(lpfarray);
	
	return SUCCESS1;
}
