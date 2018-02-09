#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "str_suite/pcode_sorts/mergesort_pcode_d.c"
#include "str_suite/string_ds.c"
#include "other/filefuns.c"
#include "other/utilfuns.c"
#include "other/timeutils.c"


//#define DEBUG 1

//output
#define Ks_EXPERIMENT "exp_summary.txt"
#define WORDS_FN "words_out"
#define SYMBOLS_FN "symbols_out"

//Modes
#define COMPRESSION_DECOMPRESSION_TEST 1
#define DECOMPRESSION_VERIFICATION 2

// how to compress data
#define SCHEME1 1 // two separate files words (for int triples), symbols (for letters), then LPF
#define SCHEME2 2 // one file for ints; sort LPF and make file with max values
#define SCHEME3 3


struct comp_file_num { 
	int num_words; 
	int num_words_in_bytes;
	int num_symbols; 
	int num_symbols_in_bytes;
};


//prototypes
struct comp_file_num compr1(int FILENAME_TYPE_INDICATOR, char symbolsfn[], char wordfn[], int ref_len, char target[], int target_int[], int target_len, struct pcode_d lpf_on_ref_target[], int lpf_pos[], int lpf_len, int k, double* completed_seconds);
struct comp_file_num compr2(int FILENAME_TYPE_INDICATOR, char wordfn[], char target[], int target_int[], int target_len, struct pcode_d lpf[], int lpf_pos[], int lpf_len, int k, double* completed_seconds);
int decompr1(int FILENAME_TYPE_INDICATOR, char symbols[], int symbols_int[], int symbols_len, int words[], int words_len, char ref[], int ref_int[], int ref_len, char target_out[], int target_out_int[], int target_len, double* completed_seconds);
int decompr2(int FILENAME_TYPE_INDICATOR, int words[], int words_len, char ref[], int ref_int[], int ref_len, char target_out[], int target_out_int[], int target_len, double* completed_seconds);
int decompress_and_validate(int FILENAME_TYPE_INDICATOR, char* exp_fn, int SCHEME, int K, char* wds_fn, char* sym_fn, char REF_FN[], char ref[], int ref_int[], int REF_LEN, char TARGET_FN[], char target[], int target_int[], int TARGET_LEN, int validate, char* description, char* TIME_RAND_STAMP, double compr_time_seconds, char* notes, struct comp_file_num cfn);
void count_cfn_bytes(struct comp_file_num* cfn, int FILENAME_TYPE_INDICATOR);


double avg_di(struct pcode_d arr[],int num);

int COMPRESS_OUT_IN_int_WORDS=1;
// used this way for compression and decompression test
//   ./LCS_BIBM_compress.exe $TARGET $target_len $REF $ref_len $REF_PLUS_TARGET $ref_plus_target_len $LPF $DECIDING_ARR $K_LOW $K_HIGH $K_INCREMENT $SCHEME $FILENAME_TYPE $DESCRIPTION $TIME_RANDOM_STAMP OUTPUT_COMPRESSED_FILES_FOLDER
// used this way to validate decompression
//   ./LCS_BIBM_compress.exe $SCHEME $TARGET $target_len $REF $ref_len $words_fn $symbols_fn
int main(int argc, char* argv[]){
	int found_error = 0;
	int mode = 0;
	int i;
	for(i=0; i<argc; i++) printf("argv[%d]=%s\n",i,argv[i]);
	FILE* tempf=NULL;
	int ii;
	char* TARGET_FN;
	int TARGET_LEN;
	char* REF_FN;
	int REF_LEN;
	char* REF_PLUS_TARGET_FN;
	int REF_PLUS_TARGET_LEN;
	char* LPF_FILENAME;
	char* DECIDING_ARR_FILENAME;
	int K_LOW;
	int K_HIGH;
	int K_INCREMENT;
	int SCHEME;
	int FILENAME_TYPE_INDICATOR=-1;
	char* DESCRIPTION=NULL;
	char* TIME_RAND_STAMP=NULL;
	char* OUTPUT_COMPRESSED_FILES_FOLDER=NULL;
	char OUTPUT_SCHEME_K_FOLDER[MED_BUF_SIZE];
	char a_temp_buf[MED_BUF_SIZE];

	char sym_fn_buf[MED_BUF_SIZE];
	char* sym_fn = sym_fn_buf;
	char wds_fn_buf[MED_BUF_SIZE];
	char* wds_fn = wds_fn_buf;
	char exp_fn[MED_BUF_SIZE];
	char notes[MED_BUF_SIZE];

	if(argc == 17){
		//----------
		// params for compression/decompression test

		mode = COMPRESSION_DECOMPRESSION_TEST;
		TARGET_FN=argv[1];
		TARGET_LEN=atoi(argv[2]);
		REF_FN=argv[3];
		REF_LEN=atoi(argv[4]);
		REF_PLUS_TARGET_FN=argv[5];
		REF_PLUS_TARGET_LEN=atoi(argv[6]);
		LPF_FILENAME=argv[7];
		DECIDING_ARR_FILENAME=argv[8];
		K_LOW=atoi(argv[9]);
		K_HIGH=atoi(argv[10]);
		K_INCREMENT=atoi(argv[11]);
		SCHEME=atoi(argv[12]);
		FILENAME_TYPE_INDICATOR = from_FILENAME_TYPE_str2num(argv[13]);
		DESCRIPTION = argv[14];
		TIME_RAND_STAMP = argv[15];
		OUTPUT_COMPRESSED_FILES_FOLDER = argv[16];
		//----------
	}else if(argc == 7){
		//----------
		// params for decompression validation
		mode = DECOMPRESSION_VERIFICATION;

		//----------
	}

	sprintf(exp_fn,"%s/%s", OUTPUT_COMPRESSED_FILES_FOLDER,Ks_EXPERIMENT);

	//printf("mode=%d  argc=%d\n",mode,argc);


					/////////////////////////////////////////////////////////
					char* target2=NULL; 				
					char* ref2=NULL;
					int* target2_int=NULL;
					int* ref2_int=NULL;
					
					printf("\nReading target and reference...\n");
					switch(FILENAME_TYPE_INDICATOR){
						case FILENAME_TYPE_CHAR_NUM: 
							target2=(char *)malloc((TARGET_LEN+1)*(sizeof(char))); target2[TARGET_LEN]='\0';
							ref2=(char *)malloc((REF_LEN+1)*(sizeof(char))); ref2[REF_LEN]='\0';
							read_char_arr(TARGET_FN,target2,TARGET_LEN);
							read_char_arr(REF_FN,ref2,REF_LEN);
							break;
						case FILENAME_TYPE_INT_NUM:
							target2_int=(int *)malloc((TARGET_LEN+1)*(sizeof(int))); target2_int[TARGET_LEN]='\0';
							ref2_int=(int *)malloc((REF_LEN+1)*(sizeof(int))); ref2_int[REF_LEN]='\0';			
							read_int_arr(TARGET_FN,target2_int,TARGET_LEN);
							read_int_arr(REF_FN,ref2_int,REF_LEN); 
							break;
					}
					/////////////////////////////////////////////////////////



	switch(mode){
		case COMPRESSION_DECOMPRESSION_TEST: {
						printf("\n\n\n\n\n\n\n\n\n\n*************************************************************************\n\n\n\n*** In LCS_BIBM_compress main() ***\n\nTARGET_FN=%s\nTARGET_LEN=%d\nREF_FN=%s\nREF_LEN=%d\nREF_PLUS_TARGET_FN=%s\nREF_PLUS_TARGET_LEN=%d\nLPF_FILENAME=%s\nDECIDING_ARR_FILENAME=%s\nK_HIGH=%d\nK_LOW=%d\nK_INCREMENT=%d\nSCHEME=%d\nFILENAME_TYPE_INDICATOR=%d\nDESCRIPTION=%s\n\n",TARGET_FN,TARGET_LEN,REF_FN,REF_LEN,REF_PLUS_TARGET_FN,REF_PLUS_TARGET_LEN,LPF_FILENAME,DECIDING_ARR_FILENAME,K_HIGH,K_LOW,K_INCREMENT,SCHEME,FILENAME_TYPE_INDICATOR,DESCRIPTION);
	
						//printf("\n\nReady to allocate arrays...\n\n");
						//int* lpf_on_ref_target=(int *)malloc((REF_PLUS_TARGET_LEN)*(sizeof(int)));
						struct pcode_d* lpf_on_ref_target=(struct pcode_d *)malloc((TARGET_LEN)*(sizeof(struct pcode_d))); 
						//printf("Allocated.\n");
						int* deciding_arr=(int *)malloc((TARGET_LEN)*(sizeof(int)));
						//printf("Allocated.\n");

						if(/*target==NULL ||*/ lpf_on_ref_target==NULL || deciding_arr==NULL){
							printf("PROBLEM ALLOCATING DATA\n");
						}
	
						printf("\n\nReading arrays...\n\n");
	
						tempf=fopen(LPF_FILENAME,"rb");
						fseek(tempf, REF_LEN * sizeof(int), SEEK_SET);
						//for(ii=0; ii<REF_LEN; ii++) read_next_int(tempf);
						for(ii=0; ii<TARGET_LEN; ii++){
							lpf_on_ref_target[ii].i=ii;
							lpf_on_ref_target[ii].di=read_next_int(tempf);
						}
						fclose(tempf);

						printf("Read.\n");
						//read_int_arr(LPF_FILENAME,lpf_on_ref_target,REF_PLUS_TARGET_LEN);
						//printf("Read.\n");
	
						//int* deciding_arr=(int *)malloc((TARGET_LEN+REF_LEN)*(sizeof(int)));
						//read_int_arr(DECIDING_ARR_FILENAME,deciding_arr,TARGET_LEN+REF_LEN);
						read_int_arr_skip(DECIDING_ARR_FILENAME,deciding_arr,REF_LEN,TARGET_LEN);
						printf("Read after read_int_arr_skip.\n");

//for(int i=0; i<TARGET_LEN; i++) printf("$$$$$$$$$$$$$ %d: deciding_arr[%d]=%d, lpf_on_ref_target[%d]=(%d,%d)\n",i,i,deciding_arr[i],i,lpf_on_ref_target[i].i,lpf_on_ref_target[i].di);

						int K;

						notes[0]='\0';
						if(SCHEME==SCHEME2){
							double initial_sort_seconds = 0;
							struct timespec t_start1, t_end1;
							struct pcode_d* temppcode=(struct pcode_d *)malloc((TARGET_LEN)*(sizeof(struct pcode_d))); 
							printf(" sorting LPF\n");
							t_start1 = get_current_time();
							mergesort(lpf_on_ref_target,TARGET_LEN,temppcode);
							free(temppcode);
							t_end1 = get_current_time();
							initial_sort_seconds = get_seconds_interval(t_start1, t_end1);
							sprintf(notes,"compr time DOES NOT include initial sort requiring %f seconds",initial_sort_seconds);
						}

						printf("Before loop\n");

						for(K=K_LOW; K<=K_HIGH; K+=K_INCREMENT){
							printf("\n\nReady to compr() with K=%d...\n\n",K);
							
							sprintf(OUTPUT_SCHEME_K_FOLDER,"%s/scheme_%d/k_%d",OUTPUT_COMPRESSED_FILES_FOLDER,SCHEME,K);
							sprintf(a_temp_buf, "mkdir -p %s",OUTPUT_SCHEME_K_FOLDER);
							system(a_temp_buf);

							sprintf(sym_fn,"%s/%s.bin",OUTPUT_SCHEME_K_FOLDER,SYMBOLS_FN);//,K,DESCRIPTION,'\0');
							sprintf(wds_fn,"%s/%s.bin",OUTPUT_SCHEME_K_FOLDER,WORDS_FN);//,K,DESCRIPTION,'\0');
	
							struct comp_file_num cfn;
							cfn.num_words=cfn.num_symbols=0;

							double compr_completed_seconds = -1;

							printf("words_fn will be at %s\n\n", wds_fn);
							switch(SCHEME){
								case SCHEME1: 
									//cfn=compr1(sym_fn, wds_fn, REF_LEN, TARGET_FN, TARGET_LEN, lpf_on_ref_target, deciding_arr, REF_PLUS_TARGET_LEN, K);
									printf("sym_fn will be at %s\n\n", sym_fn);

									cfn=compr1(FILENAME_TYPE_INDICATOR, sym_fn, wds_fn, REF_LEN, target2, target2_int, TARGET_LEN, lpf_on_ref_target, deciding_arr, REF_PLUS_TARGET_LEN, K, &compr_completed_seconds);
									break;
								case SCHEME2:
								
									cfn=compr2(FILENAME_TYPE_INDICATOR, wds_fn, target2, target2_int, TARGET_LEN, lpf_on_ref_target, deciding_arr, REF_PLUS_TARGET_LEN, K, &compr_completed_seconds);

									
									//cfn=compr2(sym_fn, wds_fn, ref2, REF_LEN, target2, TARGET_LEN, lpf_on_ref_target, deciding_arr, REF_PLUS_TARGET_LEN, K);
									break;
								case SCHEME3: 
									break;
							}


							printf("\n... compression completed in %f seconds \n\n", compr_completed_seconds);

							if(cfn.num_words==0 && cfn.num_symbols==0){
								printf(" NO COMPRESSION FILES ... EXITING\n");
							}else{
								found_error = decompress_and_validate(FILENAME_TYPE_INDICATOR, exp_fn, SCHEME, K, wds_fn, sym_fn, REF_FN, ref2, ref2_int, REF_LEN, TARGET_FN, target2, target2_int, TARGET_LEN, 1, DESCRIPTION, TIME_RAND_STAMP, compr_completed_seconds, notes, cfn);
							}
		
						}


						///////////////////////////
						/*printf("REF ");for(int ij=0; ij<REF_LEN; ij++){
							if(ref2 != NULL) printf("[%d] ",ref2[ij]);
							else{ printf("[%d]  ", ref2_int[ij]);
							
							for(int loc = 32-1; loc >= 0; loc--){
								    printf("%d",(ref2_int[ij] & (1 << loc)) ? 1 : 0);
								  }
								printf(" | ");
							}
							
						}
						printf("\n\nTARGET ");for(int ij=0; ij<TARGET_LEN; ij++){
							if(target2 != NULL) printf("[%d] ",target2[ij]);
							else {
								printf("[%d] ", target2_int[ij]);
								for(int loc = 32-1; loc >= 0; loc--){
								    printf("%d",(target2_int[ij] & (1 << loc)) ? 1 : 0);
								  }
								printf(" | ");

							}
						}printf("\n");*/
						///////////////////////////


						free(lpf_on_ref_target);
						free(deciding_arr);
						free(target2);
						free(target2_int);
						free(ref2);
						free(ref2_int);

						//break;
			}
		case DECOMPRESSION_VERIFICATION:
			/************************










			*/
			break;
	}
	
	
	
	return found_error;
}

// in theory, we don't need the TARGET_LEN, since we can just use a StringBuilder or compute TARGET_LEN by a prerun of the algorithm; here, we use it for programming and memory allocation convenience
int decompress_and_validate(int FILENAME_TYPE_INDICATOR, char* exp_fn, int SCHEME, int K, char* wds_fn, char* sym_fn, char REF_FN[], char ref[], int ref_int[], int REF_LEN, char TARGET_FN[], char target[], int target_int[], int TARGET_LEN, int validate, char* description, char* TIME_RAND_STAMP, double compr_time_seconds, char* notes, struct comp_file_num cfn){
	char buf1[BUF_SIZE], buf2[BUF_SIZE];
	//struct comp_file_num cfn;
	double decompr_completed_seconds = -1;

	FILE* fexp=fopen(exp_fn,"a");
	if(get_file_len2(exp_fn) <= 1){
		fprintf(fexp,"TIME_RAND_STAMP\tREF_FN\tTARGET_FN\tSCHEME\tK\tdescription\twords_len_in_bytes\tsymbols_len_in_bytes\tsum_len_in_bytes\tcorrect/wrong\tcompr_time_seconds\tdecompr_time_seconds\tNotes\n");
	}

	//cfn.num_words=cfn.num_symbols=cfn.num_words_in_bytes=cfn.num_symbols_in_bytes=0;
	//if(wds_fn != NULL) cfn.num_words = get_file_len2(wds_fn) / sizeof(int);
	//if(sym_fn != NULL) cfn.num_symbols = get_file_len2(sym_fn);

	//switch(FILENAME_TYPE_INDICATOR){
	//	case FILENAME_TYPE_CHAR_NUM:  break;
	//	case FILENAME_TYPE_INT_NUM: cfn.num_symbols /= sizeof(int);  break;
	//}

	//count_cfn_bytes(&cfn, FILENAME_TYPE_INDICATOR);

	int total_bytes = 0;
	int total = 0;
	char* target_found=NULL;
	int* target_found_int=NULL;

	////// careful...  can overflow...  this is unchecked
	if(cfn.num_words >= 0) total += cfn.num_words;
	if(cfn.num_symbols >= 0) total += cfn.num_symbols;

	switch(FILENAME_TYPE_INDICATOR){
		case FILENAME_TYPE_CHAR_NUM:  
			target_found = (char *)malloc((TARGET_LEN+1)*(sizeof(char))); 
			target_found[TARGET_LEN]='\0';
			break;
		case FILENAME_TYPE_INT_NUM:  
			target_found_int = (int *)malloc((TARGET_LEN+1)*(sizeof(int))); 
			target_found_int[TARGET_LEN]=0;
			break;
	}


	printf("\n\nReady to allocate arrays...\n\n");
	char* symbols=NULL;
	int* symbols_int=NULL;
	int* words=NULL;
		
	if(cfn.num_symbols>0){
		switch(FILENAME_TYPE_INDICATOR){
			case FILENAME_TYPE_CHAR_NUM:  
				symbols=(char *)malloc((cfn.num_symbols+1)*(sizeof(char))); symbols[cfn.num_symbols]='\0';
				printf(" symbols allocated \n");
				read_char_arr(sym_fn,symbols,cfn.num_symbols);
				printf(" symbols read \n");
				break;
			case FILENAME_TYPE_INT_NUM:  
				symbols_int=(int *)malloc((cfn.num_symbols+1)*(sizeof(int))); symbols_int[cfn.num_symbols]=0;
				printf(" symbols allocated \n");
				read_int_arr(sym_fn,symbols_int,cfn.num_symbols);
				printf(" symbols read \n");
				break;
		}
	}
		
	if(cfn.num_words>0){
		words=(int *)malloc((cfn.num_words+1)*(sizeof(int)));
		printf(" words allocated \n");
		
		if(COMPRESS_OUT_IN_int_WORDS==1) read_int_arr(wds_fn,words,cfn.num_words);
		else read_int_arr(wds_fn,words,cfn.num_words);
				
		printf(" words read \n");
		//for(int i=0; i<cfn.num_words; i++) printf("[%d]  ",words[i]);
	}
			
	/*if(target_found==NULL || symbols==NULL || words==NULL){
		printf("PROBLEM ALLOCATING DATA\n");
	}*/
		
	printf("cfn.num_words=%d,cfn.num_symbols=%d\n\n",cfn.num_words,cfn.num_symbols);
		
	printf("\n\nReady to decompr()...\n\n");

	int TARGET_FOUND_LEN = -1;
	switch(SCHEME){
		case SCHEME1: 
			//decompr1(symbols,cfn.num_symbols,words,cfn.num_words,ref,REF_LEN,target_found,TARGET_LEN);
			TARGET_FOUND_LEN = decompr1(FILENAME_TYPE_INDICATOR,symbols,symbols_int,cfn.num_symbols,words,cfn.num_words,ref, ref_int,REF_LEN,target_found,target_found_int,TARGET_LEN,&decompr_completed_seconds);
			break;
		case SCHEME2:
			TARGET_FOUND_LEN = decompr2(FILENAME_TYPE_INDICATOR,words,cfn.num_words,ref,ref_int,REF_LEN,target_found,target_found_int,TARGET_LEN,&decompr_completed_seconds);

			break;
		case SCHEME3: 
			break;
	}
	printf("\n... decompression completed in %f seconds \n\n", decompr_completed_seconds);

	short error=1;
	int substit_errors=0;
	int tempi;
	char result[BUF_SIZE];
	
	if(validate){
		printf("length of target=%d, length of target_found=%d\n",TARGET_LEN,TARGET_FOUND_LEN);
		if(TARGET_LEN != TARGET_FOUND_LEN) printf("\n\nTarget WRONG*****!!! Lengths are not equal!!!\n\n");
		else{
			error=0;
			for(tempi=0; tempi<TARGET_LEN; tempi++){
				int compare_found;
				int compare_actual;
				switch(FILENAME_TYPE_INDICATOR){
					case FILENAME_TYPE_CHAR_NUM: 
						compare_found=(int)target_found[tempi];
						compare_actual=(int)target[tempi];
						break;
					case FILENAME_TYPE_INT_NUM: 
						compare_found=target_found_int[tempi]; 
						compare_actual=target_int[tempi]; 
						break;
				}if((int)compare_found != (int)compare_actual){
//printf("NEQ  %5d, found[%d],actual[%d]\n",tempi,(int)compare_found,(int)compare_actual);
					error=1;
					substit_errors++;
				}//else printf(" EQ  %5d, found[%d],actual[%d]\n",tempi,(int)compare_found,(int)compare_actual);
			}
		}
		if(error != 0){
			printf("\n\nTarget WRONG*****!!!\n");
			write_WRONG_text_art(stdout);
			printf("\n\n# of errors = %d\n\n",(substit_errors==0?-1:substit_errors));
			strcpy(result,"WRONG\0");
		}else { printf("\n\nTarget correct!!!\n"); write_CORRECT_text_art(stdout); strcpy(result,"CORRECT\0"); }
		//printf("\nFOUND:\n[%s]\n\n\n",target_found);
		//printf("\nACTUAL:\n[%s]\n\n\n",target);
	}
	if(fexp != NULL){
		get_nonneg_num_as_str(cfn.num_words_in_bytes,buf1);
		get_nonneg_num_as_str(cfn.num_symbols_in_bytes,buf2);
		if(cfn.num_words_in_bytes >= 0) total_bytes += cfn.num_words_in_bytes;
		if(cfn.num_symbols_in_bytes >= 0) total_bytes += cfn.num_symbols_in_bytes;
		fprintf(fexp,"%s\t%s\t%s\t%d\t%d\t%s\t%s\t%s\t%d\t%s\t%f\t%f\t%s\n",TIME_RAND_STAMP,REF_FN,TARGET_FN,SCHEME,K,description,buf1,buf2,total_bytes,result,compr_time_seconds,decompr_completed_seconds,notes);
		fflush(fexp);
	}
	fclose(fexp);
	free(symbols);
	free(symbols_int);
	free(words);
	free(target_found);
	free(target_found_int);
	return error;	
}

// provide lpf as an array sorted with lengths in asc order
struct comp_file_num compr2(int FILENAME_TYPE_INDICATOR, char wordfn[], char target[], int target_int[], int target_len, struct pcode_d lpf[], int lpf_pos[], int lpf_len, int k, double* completed_seconds)
{
	struct pcode_d* working=(struct pcode_d *)malloc((target_len)*(sizeof(struct pcode_d))); 
	int I_SET=-1, I_NOT_SET=-2, i, j, lpfii, pos, len, adjusted_len, qqq, rrr;
	int write_this_int;
	char sym1, sym2;
	int valid=1;
	struct comp_file_num cfn;
	struct timespec t_start, t_end;

	(*completed_seconds) = -1;
	printf(" sorting LPF\n");
	
	t_start = get_current_time();
	cfn.num_words=cfn.num_words_in_bytes=cfn.num_symbols_in_bytes=0;
	cfn.num_symbols=-1;
	
	if(working==NULL) printf("**** compr2() problem allocating ***\n");
	else{
		//printf("sorted: min:lpf[%d]=%d, max:lpf[%d]=%d, avg:%f \n",lpf[0].i,lpf[0].di,lpf[target_len-1].i,lpf[target_len-1].di,avg_di(lpf,target_len));
		
		for(i=0; i<target_len; i++) working[i].i=I_NOT_SET;

		for(i=target_len-1; i>=0 && lpf[i].di>=k && valid==1; i--)
		{
			lpfii=lpf[i].i;
			len=lpf[i].di;
			pos=lpf_pos[lpfii];
			if(working[lpfii].i==I_NOT_SET){
				adjusted_len=1;
				for(j=lpfii+1; j<lpfii+len; j++){
					if(working[j].i!=I_NOT_SET) break;
					adjusted_len++;
				}
				if(adjusted_len>=k){
					working[lpfii].i=pos;
					working[lpfii].di=1;
					for(j=lpfii+1; j<lpfii+adjusted_len; j++){
						if(working[j].i!=I_NOT_SET) break;
						working[lpfii].di++;
						working[j].i=I_SET;
					}
				}
			}
		}
		
		FILE* out=fopen(wordfn,"wb");
		for(i=0; i<target_len && valid==1; i++){
			if(working[i].i==I_NOT_SET){ // singleton
				// encode the symbol s with -s

				switch(FILENAME_TYPE_INDICATOR){
					case FILENAME_TYPE_CHAR_NUM: write_this_int = -1*(target[i]+1); break; // -1 to allow 0 as a symbol and disinguish from a word 0
					case FILENAME_TYPE_INT_NUM: write_this_int = -1*(target_int[i]+1); break;
				}
//printf(" val [%d] \n", write_this_int);
				write_int_or_int(out,write_this_int,COMPRESS_OUT_IN_int_WORDS);
				cfn.num_words++;
			}else if(working[i].i!=I_SET){
				//printf("(%d,%d)\n",working[i].i,working[i].di);
				write_int_or_int(out,working[i].i,COMPRESS_OUT_IN_int_WORDS);
				write_int_or_int(out,working[i].di,COMPRESS_OUT_IN_int_WORDS);
				i+=working[i].di-1;
				cfn.num_words+=2;
			}
		}
		fclose(out);
	}

	t_end = get_current_time();
	(*completed_seconds) = get_seconds_interval(t_start, t_end);

	free(working);

	count_cfn_bytes(&cfn, FILENAME_TYPE_INDICATOR);
	printf(" leaving compr2()\n");
	
	return cfn;
}

void count_cfn_bytes(struct comp_file_num* cfn, int FILENAME_TYPE_INDICATOR){
	// words are always of type int
	(*cfn).num_words_in_bytes = (*cfn).num_words * sizeof(int);
	// symbols type changes based on FILENAME_TYPE_INDICATORs 
	switch(FILENAME_TYPE_INDICATOR){
		case FILENAME_TYPE_CHAR_NUM: 
			(*cfn).num_symbols_in_bytes = (*cfn).num_symbols * sizeof(char);
			break;
		case FILENAME_TYPE_INT_NUM:
			(*cfn).num_symbols_in_bytes = (*cfn).num_symbols * sizeof(int);
			break;
	}
}

// ..................... remove target_len from this, just use words_len
int decompr2(int FILENAME_TYPE_INDICATOR, int words[], int words_len, char ref[], int ref_int[], int ref_len, char target_out[], int target_out_int[], int target_len, double* completed_seconds){
	int w=0,i=0;
	char tmp;
	struct timespec t_start, t_end;

	(*completed_seconds) = -1;	
	t_start = get_current_time();

	do{ //printf("w = %d, words_len = %d, i = %d, target_len = %d\n", w, words_len, i, target_len); 
		int curr=words[w++];
		if(curr<0){ // one symbol

//printf("if i %d\n\n",i);

			switch(FILENAME_TYPE_INDICATOR){
				case FILENAME_TYPE_CHAR_NUM: target_out[i++]=(char)(-1*curr-1); break;
				case FILENAME_TYPE_INT_NUM: target_out_int[i++]=(int)(-1*curr-1); break;
			}			
		}else{ // 2 ints: lpf_prev length



			int begins=curr;
			int len=words[w++];
			int j;
			//printf("i=%d, b=%d, l=%d, tl=%d, rf=%d\n",i,begins,len,target_len,ref_len);
			for(j=begins; j<begins+len; j++){
//printf("else i %d, targetlen=%d\n\n",i,target_len);
//printf(" ===== else i %d, targetlen=%d, begins %d  len %d\n\n",i,target_len,begins,len);

				//printf("j = %d, target_len = %d, j-reflen=%d, INT_MAX = %d\n",j,target_len,(j-ref_len), INT_MAX/2);
				if(j>=ref_len){
					switch(FILENAME_TYPE_INDICATOR){
						case FILENAME_TYPE_CHAR_NUM: target_out[i++]=target_out[j-ref_len]; break;
						case FILENAME_TYPE_INT_NUM: target_out_int[i++]=target_out_int[j-ref_len]; break;
					}
				}
				else {
					switch(FILENAME_TYPE_INDICATOR){
						case FILENAME_TYPE_CHAR_NUM: target_out[i++]=ref[j]; break;
						case FILENAME_TYPE_INT_NUM: target_out_int[i++]=ref_int[j]; break;
					}
				}
			}
		}	
		switch(FILENAME_TYPE_INDICATOR){
			case FILENAME_TYPE_CHAR_NUM: target_out[i]='\0'; break;
			case FILENAME_TYPE_INT_NUM: target_out_int[i]=(int)0;
; break;
		}
	}while(i<target_len);

	t_end = get_current_time();
	(*completed_seconds) = get_seconds_interval(t_start, t_end);

	return i; // len of target_out / target_out_int
}

double avg_di(struct pcode_d arr[],int num){
	double avg=0;
	int i;
	for(i=0; i<num; i++) avg+=(arr[i].di/(1.0*num));
	return avg;
}

struct comp_file_num compr1(int FILENAME_TYPE_INDICATOR, char symbolsfn[], char wordfn[], int ref_len, char target[], int target_int[], int target_len, struct pcode_d lpf_on_ref_target[], int lpf_pos[], int lpf_len, int k, double* completed_seconds){
	struct comp_file_num cfn;
	int i, pos_in_target, len;
	FILE* out_sym=fopen(symbolsfn,"wb");
	FILE* out_word=fopen(wordfn,"wb");
	struct timespec t_start, t_end;
	
	(*completed_seconds) = -1;
	t_start = get_current_time();

	cfn.num_words=cfn.num_symbols=cfn.num_words_in_bytes=cfn.num_symbols_in_bytes=0;

	for(i=ref_len; i<lpf_len; i++){
		pos_in_target=i-ref_len;

//printf("i-ref_len=%d, target_len=%d, ref_len=%d, lpf_len=%d\n",(i-ref_len), target_len, ref_len, lpf_len);

		len=lpf_on_ref_target[i-ref_len].di;
		if(len<k) { // not significant enough to compress word

			switch(FILENAME_TYPE_INDICATOR){
				case FILENAME_TYPE_CHAR_NUM: write_char(out_sym,target[pos_in_target]); break;
				case FILENAME_TYPE_INT_NUM: write_int_or_int(out_sym,target_int[pos_in_target],COMPRESS_OUT_IN_int_WORDS); break;
			}
			cfn.num_symbols++;
		}else{
			//printf("[pos_in_target=%d,lpf_pos[i]=%d,len=%d]\n",pos_in_target,lpf_pos[i-ref_len],len);
			write_int_or_int(out_word,pos_in_target,COMPRESS_OUT_IN_int_WORDS);
			write_int_or_int(out_word,lpf_pos[i-ref_len],COMPRESS_OUT_IN_int_WORDS); // position in ref
			write_int_or_int(out_word,len,COMPRESS_OUT_IN_int_WORDS);
			cfn.num_words+=3;
			i=i+len-1;	
		}
	}

	t_end = get_current_time();
	(*completed_seconds) = get_seconds_interval(t_start, t_end);
	
	count_cfn_bytes(&cfn, FILENAME_TYPE_INDICATOR);

	fclose(out_sym);
	fclose(out_word);
	return cfn;
}

// return length of decompressed string
int decompr1(int FILENAME_TYPE_INDICATOR, char symbols[], int symbols_int[], int symbols_len, int words[], int words_len, char ref[], int ref_int[], int ref_len, char target_out[], int target_out_int[], int target_len, double* completed_seconds){
	int s=0,w=0,i=0;
	char tmp;
	struct timespec t_start, t_end;
	
	(*completed_seconds) = -1;
	t_start = get_current_time();

	//int found_target_len=words[w++];
	do{
		if(w<words_len && i==words[w]){
			int begins=words[++w];
			int len=words[++w];
			w++;
			int j;
			for(j=begins; j<begins+len; j++){
				if(j>=ref_len){
					switch(FILENAME_TYPE_INDICATOR){
						case FILENAME_TYPE_CHAR_NUM: target_out[i++]=target_out[j-ref_len]; break;
						case FILENAME_TYPE_INT_NUM: target_out_int[i++]=target_out_int[j-ref_len]; break;
					}
				}else{
					switch(FILENAME_TYPE_INDICATOR){
						case FILENAME_TYPE_CHAR_NUM: target_out[i++]=ref[j]; break;
						case FILENAME_TYPE_INT_NUM: target_out_int[i++]=ref_int[j]; break;
					}
				}
			}		
		}else if(s<symbols_len){
			switch(FILENAME_TYPE_INDICATOR){
				case FILENAME_TYPE_CHAR_NUM: target_out[i++]=symbols[s++]; break;
				case FILENAME_TYPE_INT_NUM: target_out_int[i++]=symbols_int[s++]; break;
			}
		}
		switch(FILENAME_TYPE_INDICATOR){
			case FILENAME_TYPE_CHAR_NUM: target_out[i]='\0'; break;
			case FILENAME_TYPE_INT_NUM: target_out_int[i]=0; break;
		}
	}while(i<target_len);

	t_end = get_current_time();
	(*completed_seconds) = get_seconds_interval(t_start, t_end);

	return i;
}



