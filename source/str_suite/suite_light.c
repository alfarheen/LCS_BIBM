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
#include "trad_sarray/KS_suf_sort.c"  // modified from http://people.mpi-inf.mpg.de/~sanders/programs/suffix/
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
	*** use argv[1-9] for algorithm AVERAGE_PSA,WORST_CASE_PSA2,WORST_CASE_PSA3,PSA_FOR_PLRF_AND_PLF;
	*** use argv[1-4] for algorithm TRADITIONAL_SA_LCP (argv[3]=""),PLPF,PLCP_TCS,PLCP_TCS_IMP,PLCP_JDA,PERMUTED_PLCP_JDA,PLCP_JDA_IMP,PERMUTED_PLCP_JDA_IMP,PLNEF,PLRF,PSA_FOR_PLRF_AND_PLF,PLF,PBORDER;

	argv[1]: algorithm(0:plcp_given_psa_from_file,1:avgcase,{2,3}:worstcase,4:traditional)
	argv[2]: filename
	argv[3]: parameter_list_or_NO_PARAM_for_none
	argv[4]: compute_pSA_using_only_#bytes
	argv[5]: make_blocks_with_pAC()_y/n
	argv[6]: mblock_size(zero_for_largest_valid)
	argv[7]: validate_pSA_correctness_y/n
	argv[8]: filename_type(char,int)
	argv[9]: path_for_output_arrays_data
*/
int main(int argc, char* argv[])
{
	int start = 0;
	int i,j,k,z,v,mid;
	int block = 3, mblock;
	int psuffixq,removei,addm;
	int strsize = 0, unique_symbols = 0, temp;
	int psuffixarraybytes = 0;
	int filename_type_indicator;
	char init_filename_type[BUF_SIZE];
	char* fn=NULL;
	short ALGORITHM,correct,avg;
	long double start_sec, end_sec;
	char* PARAMS;
	char uctmp[BUF_SIZE];
	char path_for_output_arrays_data[MED_BUF_SIZE];
	char output_filename[MED_BUF_SIZE], output_filename2[MED_BUF_SIZE];
	FILE* f_psa_out=NULL;
	FILE* f_sa_out=NULL;
	struct wcases w;
	
	sprintf(path_for_output_arrays_data, "../arrays");

	strcpy(init_filename_type,FILENAME_TYPE_CHAR);
	filename_type_indicator=from_FILENAME_TYPE_str2num(init_filename_type);

	for(i=0; i<argc; i++) printf("argv[%d]=%s   |   ", i, argv[i]);
	printf("\n\n");
	
	printf("\n\n\n\n\n\n\n\n\n\n*************************************************************************\n\n***suite_light.c***\n\n\n\n\n\n");
	fflush(stdout);
	
	if(argc > 2 && strlen(argv[1]) > 0 && strlen(argv[2]) > 0) 
	{
		ALGORITHM=atoi(argv[1]);
		
		fn=argv[2];
		strsize=get_file_len2(fn);
		
		if(argc > 4 && strlen(argv[4]) > 0)
		{
			temp=strtoul(argv[4],NULL,10);
			if(temp>0 && temp<strsize) 
				strsize=temp;
			//printf("temp=%d,strsize=%d,argv[4]=%s\n",temp,strsize,argv[4]);
		}
		
		if(strsize<=RSORT_CONST)
		{	
			printf("\n\n*** Trivial (try inter string) strsize = %d, RSORT_CONST = %d ***\n\n", strsize, RSORT_CONST);
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
			if(temp<=0 || temp>=strsize)
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

		if(argc > 8 && strlen(argv[8]) > 0){

//printf("$$$$$$$$$$$$$$$$$$$$$$ argv[]%s %d\n\n",argv[8],strsize);



			filename_type_indicator = from_FILENAME_TYPE_str2num(argv[8]);
			if(filename_type_indicator == FILENAME_TYPE_INT_NUM) strsize /= sizeof(int);

			//printf("$$$$$$$$$$$$$$$$$$$$$$ argv[]%s %d\n\n",argv[8],strsize);
		}


		if(argc > 9 && strlen(argv[9]) > 0){
			sprintf(path_for_output_arrays_data,"%s",argv[9]);
		}
		

		make_fn(path_for_output_arrays_data,(char*)PSA_FN,PSA_FILE);
		make_fn(path_for_output_arrays_data,(char*)SA_FN,SA_FILE);

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

		short JUST_TRADITIONAL=0;

		if(ALGORITHM==LCP_KASAI || ALGORITHM==TRADITIONAL_SA_LCP || ALGORITHM==TRADITIONAL_SA || ALGORITHM==KS_INT_TRADITIONAL_SA || ALGORITHM==LPF_DIRECT_CROCHEMORE || ALGORITHM==LPF_DIRECT_CROCHEMORE__LIGHT){
			JUST_TRADITIONAL=1;
			printf("\n\nJust Traditional function, ALGORITHM=%d\n\n",ALGORITHM);
		}else JUST_TRADITIONAL=0;

		if(strsize>0)
		{
			//printf("strsize=%d>0\n\n",strsize); fflush(stdout);
			
			if(ALGORITHM==PSA_FOR_PLRF_AND_PLF || ALGORITHM==PLRF || ALGORITHM==PLF)
			{
				strsize*=2; // consider T^R
				strsize++; // consider also ending symbol FREE_CONST
				// now, strsize=|<T><FREE_CONST><T^R>|
			}
			

			if(filename_type_indicator == FILENAME_TYPE_CHAR_NUM)
			{
				free(intfbuffer);
				intfbuffer=NULL;
				fbuffer=(char *)malloc(strsize+5);
				printf("BEFORE read...\n");fflush(stdout);
				readFile(fn,fbuffer,strsize);
				printf("AFTER read...\n");fflush(stdout);
				fbuffer[strsize]=0;
			}
			else if(filename_type_indicator == FILENAME_TYPE_INT_NUM)
			{
				free(fbuffer);
				fbuffer=NULL;
				intfbuffer=(int *)malloc(strsize*sizeof(int)+5*sizeof(int));						
				printf(": reading length-%d INTEGER text from: %s\n\n",strsize,fn);
				if(read_int_arr(fn,intfbuffer,strsize)==FAILED1){
					printf("**** ERROR: READING FILE\n\n");
					return FAILED1;
				}
				printf("READ FILE.\n");
			}


			if(ALGORITHM==LCP_KASAI)
			{
				printf("running KASAI LCP algorithm\n\n");
				
				lcparray=(int *)malloc((strsize)*(sizeof(int)));
				suffixarray=(int *)malloc((strsize)*(sizeof(int)));
				Rankarray=(int *)malloc((strsize)*(sizeof(int)));
				
				if(fbuffer && suffixarray && Rankarray)
				{
					readFile(fn,fbuffer,strsize);
					fbuffer[strsize]=0;
						
					printf(": reading length-%d SA from %s\n\n",strsize,SA_FILE);
									
					if(read_int_arr(SA_FILE,suffixarray,strsize)==FAILED1)
						return FAILED1;
					construct_rank_array(Rankarray,suffixarray,strsize);
					
					start_sec=get_current_microseconds();
					compute_LCP_Kasai(fbuffer,lcparray,suffixarray,Rankarray,strsize);
					end_sec=get_current_microseconds();
					printf("\n\n*** Kasai_LCP executed for a duration of %.0Lf usec or %.10Lf sec (from %.0Lf to %.0Lf) ***\n\n",end_sec-start_sec,(end_sec-start_sec)/1000000.0,start_sec,end_sec);
					
					display_int_arr(stdout,lcparray,strsize);
				}
			}else if(ALGORITHM==LPF_DIRECT_CROCHEMORE)
			{
				printf("running CROCHEMORE LPF DIRECT algorithm\n\n");
				
				make_fn(path_for_output_arrays_data,(char*)LPF_FN,output_filename);
				make_fn(path_for_output_arrays_data,(char*)DECIDING_ARR_FN,output_filename2);
				printf("... will write LPF data to %s\n", output_filename);
				printf("... will write DECIDING_ARR data to %s\n", output_filename2);
                         	suffixarray=(int *)malloc((strsize)*(sizeof(int)));
				lpfarray=(int *)malloc((strsize)*(sizeof(int)));
				before_lt_array=(int *)malloc((strsize)*(sizeof(int)));
				before_gt_array=(int *)malloc((strsize)*(sizeof(int)));
				if((fbuffer || intfbuffer) && suffixarray && lpfarray && before_lt_array && before_gt_array)
				{
					//readFile(fn,fbuffer,strsize);
					//fbuffer[strsize]=0;
						
					printf(": reading length-%d SA from %s\n\n",strsize,SA_FILE);
									
					if(read_int_arr(SA_FILE,suffixarray,strsize)==FAILED1){
						printf("*** PROBLEM READING LEN-%d FILE: %s ***\n",strsize,SA_FILE);
						return FAILED1;
					}
					
					start_sec=get_current_microseconds();
					compute_LPF_direct_Crochemore(fbuffer,intfbuffer,lpfarray,before_lt_array,before_gt_array,suffixarray,strsize,output_filename,output_filename2);
					end_sec=get_current_microseconds();
					printf("\n\n*** CROCHEMORE_LPF_DIRECT executed for a duration of %.0Lf usec or %.10Lf sec (from %.0Lf to %.0Lf) ***\n\n",end_sec-start_sec,(end_sec-start_sec)/1000000.0,start_sec,end_sec);
					
					//display_int_arr(stdout,lpfarray,strsize);
				}else printf("********** ALLOCATION ISSUE\n\n");
			}
			else if(ALGORITHM==LPF_DIRECT_CROCHEMORE__LIGHT)
			{
				printf("running CROCHEMORE LPF DIRECT LIGHT!!!!!!!!!!!!!!!! algorithm\n\n");
				make_fn(path_for_output_arrays_data,(char*)LPF_FN,output_filename);
				make_fn(path_for_output_arrays_data,(char*)DECIDING_ARR_FN,output_filename2);
                         	start_sec=get_current_microseconds();
				compute_LPF_direct_Crochemore__LIGHT(fn,SA_FILE,strsize,output_filename,output_filename2);
				end_sec=get_current_microseconds();
				printf("\n\n*** CROCHEMORE_LPF_DIRECT executed for a duration of %.0Lf usec or %.10Lf sec (from %.0Lf to %.0Lf) ***\n\n",end_sec-start_sec,(end_sec-start_sec)/1000000.0,start_sec,end_sec);
					
				//display_int_arr(stdout,lpfarray,strsize);
			}
			else
			{
				//printf("ELSE part BEFORE allocations\n");
				
				if(!JUST_TRADITIONAL){
					prevT=(int *)malloc((strsize+1)*(sizeof(int)));
					forwT=(int *)malloc((strsize+1)*(sizeof(int)));
					types_prevT=(char *)malloc(strsize+1);
					remap_prev_size=strsize+MAP_SIZE;
					remap_prev=(int *)malloc((remap_prev_size)*(sizeof(int)));
					
					prevT[strsize]=0;
					forwT[strsize]=0;
					types_prevT[strsize]=0;
				}

				psuffixarraybytes=strsize*sizeof(int);
				psuffixarray=(int *)malloc(psuffixarraybytes+5*sizeof(int));

				//printf("ELSE part AFTER allocations\n");
				
				if((fbuffer || intfbuffer) && psuffixarray && ((JUST_TRADITIONAL) || (prevT && forwT && types_prevT)))
				{
					if(ALGORITHM==PSA_FOR_PLRF_AND_PLF || ALGORITHM==PLRF || ALGORITHM==PLF) // construct, fbuffer=<T><FREE_CONST><T^R>
					{
						v=(strsize-1)/2;
						readFile(fn,fbuffer,v);
						fbuffer[v]=FREE_CONST;
						fbuffer[strsize]=0;
						z=v+1;
						for(k=v-1; k>=0; k--)
						{
							fbuffer[z++]=fbuffer[k];
							if(k==0) break; /* since no negative in int */
						}
					}
					
					//
					//printf("\nT=");
					//for(k=0; k<strsize; k++) printf("%c",fbuffer[k]);
					//printf("\n\n");
					//
					
					if(!JUST_TRADITIONAL){
						unique_symbols=getAlphaFreq(fbuffer,strsize,alpha_freq,remap_alpha,NO);
						max_distance=prev(prevT,types_prevT,fbuffer,alpha_type,strsize);
						unique_symbols=remapPrevAlpha(prevT,types_prevT,max_distance,strsize,remap_prev,remap_prev_size);
						prev_cdf_size=unique_symbols+1;
						#if DEBUG>1
							printf("cdf size: %d\n",prev_cdf_size);
						#endif
						prev_cdf=(long double *)malloc(prev_cdf_size*(sizeof(long double)));
					}
					if(JUST_TRADITIONAL || prev_cdf)
					{
						if(!JUST_TRADITIONAL){
							printf("if(prev_cdf) A\n");
							make_uniform_cdf(prev_cdf_size,prev_cdf);
							probability=1.0/unique_symbols;
							
							if(FIND_MAX_BLOCK!=0) 
							{
								#if DEBUG>1
									printf("\n\nFinding max block...\n\n");
								#endif
								block=max_block(strsize,prev_cdf);
							}
							#if DEBUG>1
								printf("block=%d\n\n",block);
							#endif
							
							epsilon=calc_epsilon(block,prev_cdf);
							if(epsilon<=0) return FAILED1;
							
							forw(fbuffer,forwT,prevT,strsize,alpha_type);
							
							printf("if(prev_cdf) B\n");
							
							if(DEBUG!=0) 
							{
								//print_psuffixes(0,block,prevT,forwT,strsize,types_prevT,max_distance,prev_cdf,remap_prev,probability);
								//printf("-------------------------\n");
								print_psuffixes(4,block,prevT,forwT,strsize,types_prevT,max_distance,prev_cdf,remap_prev,probability);
							}
						
							if(MAKE_BLOCKS==0)
							{
								printf("if(prev_cdf) B.1\n");
								
								RECALCULATE_PAC=find_recalculate_pAC_window(block,prevT,forwT,strsize,types_prevT,max_distance,prev_cdf,remap_prev,probability);
									
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
						}
						printf("if(prev_cdf) C\n");
						if(ALGORITHM==AVERAGE_PSA) block=(int)(ceil(log2(strsize)));
						printf("if(prev_cdf) D\n");
						fflush(stdout);
						//printf("\nfilename=\"%s\"\nsize=%d\nblock=%d\nMAKE_BLOCKS=%d\nRECALCULATE_PAC=%d\nPARAMS=\"%s\" where ALL_PARAM=%c and NO_PARAM=%c\nunique_symbols(in prev(T))=%d\nuniform_probability=%.100Lf\nepsilon=%.100Lf\n\n",fn,strsize,block,(int)MAKE_BLOCKS,(int)RECALCULATE_PAC,PARAMS,ALL_PARAM,NO_PARAM,(int)unique_symbols,probability,epsilon);
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
															
								printf(": reading length-%d pSA from %s\n\n",strsize,PSA_FILE);
								
								if(read_int_arr(PSA_FILE,psuffixarray,strsize)==FAILED1)
									return FAILED1;
								
								if(ALGORITHM==PLCP_NAIVE)
								{
									plcparray=(int *)malloc((strsize)*(sizeof(int)));
									if(plcparray)
									{
										start_sec=get_current_microseconds();
										
										compute_pLCP(strsize,prevT,types_prevT,plcparray,psuffixarray);
										
										end_sec=get_current_microseconds();
										printf("\n\n*** naive_pLCP executed for a duration of %.0Lf usec or %.10Lf sec (from %.0Lf to %.0Lf) ***\n\n",end_sec-start_sec,(end_sec-start_sec)/1000000.0,start_sec,end_sec);
								
										printf("\n\nDisplaying pLCP...\n\n");
										display_int_arr(stdout,plcparray,strsize);
										printf("\n\nmean(pLCP)=%.10Lf\nmax(pLCP)=%d\n\n",mean(strsize,plcparray),max(strsize,plcparray));
									}
								}else if(ALGORITHM==PLPF)
								{
									plpfarray=(int *)malloc((strsize)*(sizeof(int)));
									Rankarray=(int *)malloc((strsize)*(sizeof(int)));
									before_lt_array=(int *)malloc((strsize)*(sizeof(int)));
									before_gt_array=(int *)malloc((strsize)*(sizeof(int)));
									if(plpfarray && Rankarray && before_lt_array && before_gt_array)
									{
										execute_pLPF_variant(PLPF,psuffixarray,Rankarray,strsize,plpfarray,before_lt_array,before_gt_array,NULL,NULL,prevT,types_prevT,prevT,types_prevT,prevT,types_prevT);
									}
								}else if(ALGORITHM==PLCP_TCS)
								{
									printf("\nIn pLCP_TCS...\n\n");
									
									plcparray=(int *)malloc((strsize)*(sizeof(int)));
									Rankarray=(int *)malloc((strsize)*(sizeof(int)));
									before_lt_array=(int *)malloc((strsize)*(sizeof(int)));
									after_lt_array=(int *)malloc((strsize)*(sizeof(int)));
									if(plcparray && Rankarray && before_lt_array && after_lt_array)
									{
										execute_pLPF_variant(PLCP_TCS,psuffixarray,Rankarray,strsize,plcparray,before_lt_array,after_lt_array,NULL,NULL,prevT,types_prevT,prevT,types_prevT,prevT,types_prevT);
									}
								}else if(ALGORITHM==PLCP_TCS_IMP)
								{
									printf("\nIn pLCP_TCS_IMP...\n\n");
									
									plcparray=(int *)malloc((strsize)*(sizeof(int)));
									Rankarray=(int *)malloc((strsize)*(sizeof(int)));
									neighbor_array=(int *)malloc((strsize)*(sizeof(int)));
									if(plcparray && Rankarray && neighbor_array)
									{
										execute_pLPF_variant(PLCP_TCS_IMP,psuffixarray,Rankarray,strsize,plcparray,neighbor_array,NULL,NULL,NULL,prevT,types_prevT,prevT,types_prevT,prevT,types_prevT);
									}
								}else if(ALGORITHM==PLCP_JDA || ALGORITHM==PERMUTED_PLCP_JDA)
								{
									printf("\nIn pLCP_JDA...\n\n");
									
									plcparray=(int *)malloc((strsize)*(sizeof(int)));
									Rankarray=(int *)malloc((strsize)*(sizeof(int)));
									before_lt_array=(int *)malloc((strsize)*(sizeof(int)));
									before_gt_array=(int *)malloc((strsize)*(sizeof(int)));
									if(plcparray && Rankarray && before_lt_array && before_gt_array)
									{
										execute_pLPF_variant(ALGORITHM,psuffixarray,Rankarray,strsize,plcparray,before_lt_array,before_gt_array,NULL,NULL,prevT,types_prevT,prevT,types_prevT,prevT,types_prevT);
									}
								}else if(ALGORITHM==PLCP_JDA_IMP  || ALGORITHM==PERMUTED_PLCP_JDA_IMP)
								{
									printf("\nIn pLCP_JDA_IMP...\n\n");
									
									plcparray=(int *)malloc((strsize)*(sizeof(int)));
									Rankarray=(int *)malloc((strsize)*(sizeof(int)));
									neighbor_array=(int *)malloc((strsize)*(sizeof(int)));
									if(plcparray && Rankarray && neighbor_array)
									{
										execute_pLPF_variant(ALGORITHM,psuffixarray,Rankarray,strsize,plcparray,neighbor_array,NULL,NULL,NULL,prevT,types_prevT,prevT,types_prevT,prevT,types_prevT);
									}
								}else if(ALGORITHM==PLNEF || ALGORITHM==PLRF || ALGORITHM==PLF)
								{
									if(ALGORITHM==PLNEF)
									{
										plnefarray=(int *)malloc((strsize)*(sizeof(int)));
										Rankarray=(int *)malloc((strsize)*(sizeof(int)));
										neq_lt_array=(int *)malloc((strsize)*(sizeof(int)));
										neq_gt_array=(int *)malloc((strsize)*(sizeof(int)));
										if(plnefarray && Rankarray && neq_lt_array && neq_gt_array)
										{
											if(ALGORITHM!=PLF)
												execute_pLPF_variant(PLNEF,psuffixarray,Rankarray,strsize,plnefarray,neq_lt_array,neq_gt_array,NULL,NULL,prevT,types_prevT,prevT,types_prevT,prevT,types_prevT);
										}
									}
									if(ALGORITHM==PLRF || ALGORITHM==PLF)
									{
										v=(strsize-1)/2;
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
										Rankarray=(int *)malloc((strsize)*(sizeof(int)));
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
									pborderarray=(int *)malloc((strsize)*(sizeof(int)));
									Rankarray=(int *)malloc((strsize)*(sizeof(int)));
									prefix_array=(int *)malloc((strsize)*(sizeof(int)));
									suffix_array=(int *)malloc((strsize)*(sizeof(int)));
									if(pborderarray && Rankarray && prefix_array && suffix_array)
									{
										execute_pLPF_variant(ALGORITHM,psuffixarray,Rankarray,strsize,pborderarray,prefix_array,suffix_array,NULL,NULL,prevT,types_prevT,prevT,types_prevT,prevT,types_prevT);
									}
								}							
								
								break;

							case NAIVE_PBORDER:
								pborderarray=(int *)malloc((strsize)*(sizeof(int)));
								
								start_sec=get_current_microseconds();
								correct_pborder(pborderarray,prevT,types_prevT,strsize);
								end_sec=get_current_microseconds();
								printf("\n\n*** naive_pborder executed for a duration of %.0Lf usec or %.10Lf sec (from %.0Lf to %.0Lf) ***\n\n",end_sec-start_sec,(end_sec-start_sec)/1000000.0,start_sec,end_sec);
								break;
								
							case AVERAGE_PSA: 
								printf("running p_suffix_sort_pAC1 (average case)\n\n");
								
								start_sec=get_current_microseconds();
								
								avg_case=p_suffix_sort_pAC1(block,psuffixarray,prevT,forwT,strsize,types_prevT,max_distance,prev_cdf,remap_prev,probability);
								
								end_sec=get_current_microseconds();
								printf("\n\n*** p_suffix_sort_pAC1 executed for a duration of %.0Lf usec or %.10Lf sec (from %.0Lf to %.0Lf) ***\n\n",end_sec-start_sec,(end_sec-start_sec)/1000000.0,start_sec,end_sec);
								
								if(avg_case==0) printf("\n*** FYI: not completely sorted ***\n\n");
								break;
								
							case PSA_FOR_PLRF_AND_PLF:
							case WORST_CASE_PSA2:
								printf("running p_suffix_sort_pAC2 (worst case)\n\n");
								
								if(ALGORITHM==PSA_FOR_PLRF_AND_PLF) printf("... pSA for pLrF data structure ... \n\n");

												
								start_sec=get_current_microseconds();
								
								p_suffix_sort_pAC2(block,psuffixarray,prevT,forwT,strsize,types_prevT,max_distance,prev_cdf,remap_prev,probability);
								
								end_sec=get_current_microseconds();
								printf("\n\n*** p_suffix_sort_pAC2 executed for a duration of %.0Lf usec or %.10Lf sec (from %.0Lf to %.0Lf) ***\n\n",end_sec-start_sec,(end_sec-start_sec)/1000000.0,start_sec,end_sec);
								break;
								
							case WORST_CASE_PSA3:
								printf("running p_suffix_sort_pAC3 (improved worst case)\n\n");
								
								start_sec=get_current_microseconds();
								
								avg=p_suffix_sort_pAC3(block,psuffixarray,prevT,forwT,strsize,types_prevT,max_distance,prev_cdf,remap_prev,probability);
								
								end_sec=get_current_microseconds();
								printf("\n\n*** p_suffix_sort_pAC3 executed for a duration of %.0Lf usec or %.10Lf sec (from %.0Lf to %.0Lf) ***\n\n",end_sec-start_sec,(end_sec-start_sec)/1000000.0,start_sec,end_sec);
								
								if(avg!=0) printf("\n\n*** Solved with average case algorithm ***\n\n");
								break;
							
							case KS_INT_TRADITIONAL_SA:
								finalintfbuffer=(int *)malloc(strsize*sizeof(int)+5*sizeof(int));
								if(finalintfbuffer != NULL && intfbuffer != NULL){
									intfbuffer[strsize]=intfbuffer[strsize+1]=intfbuffer[strsize+2]=0;
									finalintfbuffer[strsize]=finalintfbuffer[strsize+1]=finalintfbuffer[strsize+2]=0;					
									psuffixarray[strsize]=psuffixarray[strsize+1]=psuffixarray[strsize+2]=0;

									remap_string_int_alpha(intfbuffer, strsize, &largest_remapped_int, finalintfbuffer);
									//free(intfbuffer);
									printf("largest_remapped_int=%d\n",largest_remapped_int);
									printf("running KS traditional suffix sort\n\n");
									fflush(stdout);
									start_sec=get_current_microseconds();
									KS_suffixArray(finalintfbuffer, psuffixarray, strsize, largest_remapped_int);
									end_sec=get_current_microseconds();
									printf("\n\n*** KS traditional suffix sort executed for a duration of %.0Lf usec or %.10Lf sec (from %.0Lf to %.0Lf) ***\n\n",end_sec-start_sec,(end_sec-start_sec)/1000000.0,start_sec,end_sec);
									//free(finalintfbuffer);
								}else{
									printf("**** ERROR: Allocation\n\n");
								}
								break;

							case TRADITIONAL_SA_LCP:
							case TRADITIONAL_SA:
								//
								// this part uses 9*strsize bytes
								//

								printf("running traditional suffix sort ssarray\n\n");
								fflush(stdout);
								start_sec=get_current_microseconds();
								
								//intfbuffer=(int *)malloc(((strsize+1)*(sizeof(int))));
								
								
								//if(intfbuffer)
								{
									// symbols before
									//for(i=0; i<strsize; i++) printf("[%d]",fbuffer[i]);
									//printf("\n");
									//
									
									for(i=0; i<strsize; i++)
									{
										psuffixarray[i]=(int)(fbuffer[i]);
									}
									psuffixarray[i]=0; // the $
									
									//free(fbuffer);
									
									
									//uintfbuffer=(int *)malloc(((strsize+1)*(sizeof(int))));
									

									//if(remap_for_fun)
									//{
    										printf("\nradix_sort_uint...\n");
										radix_sort_uint(psuffixarray,strsize,RSORT_CONST);
										printf("\nradix_sort_uint DONE!\n");
										remap_for_fun=(int *)malloc(ASCII_ALPHABET_SIZE*sizeof(int));

										if(remap_for_fun)
										{
											for(i=0; i<ASCII_ALPHABET_SIZE; i++){
												remap_for_fun[i]=0;
												//uintfbuffer[i]=(int)(psuffixarray[i]);
											}
											i=0;
											j=1;

											
											//printBuf2(psuffixarray,strsize);
											printf("\nRemap 1\n");
											do
											{
												remap_for_fun[psuffixarray[i]]=j++;
												while(++i<strsize && psuffixarray[i]==psuffixarray[i-1]);
											}while(psuffixarray[i]!=0);
											printf("\nRemap 2\n");
											
											for(i=0; i<strsize; i++)
												psuffixarray[i]=remap_for_fun[(int)(fbuffer[i])];
											printf("\nRemap 3\n");

											//printBuf2(psuffixarray,strsize);
											

											//free(remap_for_fun);
											
											// remapped symbols after
											//for(i=0; i<strsize; i++) printf("[%d]",psuffixarray[i]);
											//printf("\n");
											//
											
											//uintfbuffer=(int *)malloc(((strsize+1)*(sizeof(int))));
											//for(i=0; i<strsize; i++){
											//	uintfbuffer[i]=(int)fbuffer[i];
											//}
											//suffixArray(uintfbuffer, psuffixarray, strsize, 256);
											//printf("\n\nsuffixArray() done\n\n");
											printf("\nRunning ssarray\n");
											if(ssarray(psuffixarray)<0)
												printf("*** Problem with ssarray sort ***\n\n");
											else
											{
												printf("\nFinished ssarray\n");
												for(i=1; i<=strsize; i++) // leave out [$] suffix
												{
													psuffixarray[i-1]=psuffixarray[i];
												}
												printf("\nAfter leaving out [$] suffix\n");
												if(ALGORITHM==TRADITIONAL_SA)
												{
													printf("\nWriting...\n");
													f_sa_out=fopen(SA_FILE,"wb");
													if(f_sa_out)
														display_int_arr(f_sa_out,psuffixarray,strsize);
													fclose(f_sa_out);
													printf("\nWrote.\n");
													
												}
												else if(ALGORITHM==TRADITIONAL_SA_LCP)
												{
													plcparray=(int *)malloc((strsize)*(sizeof(int)));
													if(plcparray)
													{
														compute_pLCP(strsize,prevT,types_prevT,plcparray,psuffixarray);
														printf("\n\nmean(LCP)=%.10Lf\nmax(LCP)=%d\n\n",mean(strsize,plcparray),max(strsize,plcparray));
													}
												}
											}
										}
									//}
								}
									
								end_sec=get_current_microseconds();
								printf("\n\n*** traditional ssarray executed for a duration of %.0Lf usec or %.10Lf sec (from %.0Lf to %.0Lf) ***\n\n",end_sec-start_sec,(end_sec-start_sec)/1000000.0,start_sec,end_sec);
								break;
														
							default: return SUCCESS1;
						}
						
						if(ALGORITHM==AVERAGE_PSA || ALGORITHM==WORST_CASE_PSA2 || ALGORITHM==WORST_CASE_PSA3 || ALGORITHM==TRADITIONAL_SA_LCP || ALGORITHM==TRADITIONAL_SA || ALGORITHM==KS_INT_TRADITIONAL_SA || ALGORITHM==PSA_FOR_PLRF_AND_PLF){
							printf("\n\nDisplaying pSA...\n\n");
							//display_pSA(psuffixarray,prevT,strsize,types_prevT);
							
							if(ALGORITHM==TRADITIONAL_SA_LCP || ALGORITHM==TRADITIONAL_SA || ALGORITHM==KS_INT_TRADITIONAL_SA) f_psa_out=fopen(SA_FILE,"wb");
							else f_psa_out=fopen(PSA_FILE,"wb");
							if(f_psa_out)
								display_int_arr(f_psa_out,psuffixarray,strsize);
							fclose(f_psa_out);
						}
						if(VERIFY_PSA!=0 && ALGORITHM!=KS_INT_TRADITIONAL_SA)
						{
							printf("\n\nVerifying correct pSA...\n\n");
							correct=correct_pSA(psuffixarray,prevT,strsize,types_prevT);
							if(correct==0) printf("\n*** Invalid pSA (consider cascading standard floating-point arithmetic error) ***\n\n");
							else printf("\n*** Correct pSA ***\n\n");
						}
					}
				}
			}
		}
	}

	if(fbuffer!=NULL) free(fbuffer);
	free(prevT);free(forwT);free(psuffixarray);free(types_prevT);free(remap_prev);free(prev_cdf);free(plcparray);if(intfbuffer!=NULL)free(intfbuffer);if(finalintfbuffer!=NULL)free(finalintfbuffer);free(remap_for_fun);free(plpfarray);free(Rankarray);free(before_lt_array);free(before_gt_array);free(plnefarray);free(neq_lt_array);free(neq_gt_array);free(after_lt_array);free(after_gt_array);free(rev_lt_array);free(rev_gt_array);free(neighbor_array);free(plrfarray);free(prevQ1);free(types_prevQ1);free(prevQ2);free(types_prevQ2);free(pborderarray);free(prefix_array);free(suffix_array);free(pLF_array);free(lcparray);free(suffixarray);free(lpfarray);
	
	printf("\n\nEXIT\n\n");

	return SUCCESS1;
}


