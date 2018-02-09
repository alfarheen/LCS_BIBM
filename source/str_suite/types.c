#include <limits.h>

#ifndef _TYPES_C
#define _TYPES_C


#define FAILED1 1
#define SUCCESS1 0



#define DEBUG 0
// 0 for no debugging; 1 for some; 2 for more


#define FILENAME_TYPE_CHAR "char\0"
#define FILENAME_TYPE_CHAR_NUM 0
#define FILENAME_TYPE_INT "int\0"
#define FILENAME_TYPE_INT_NUM 1

int from_FILENAME_TYPE_str2num(char* filename_type_str);
int from_FILENAME_TYPE_str2num(char* filename_type_str){
	int num = -1;
	if(strcmp(filename_type_str,FILENAME_TYPE_CHAR) == 0) num = FILENAME_TYPE_CHAR_NUM;
	else if(strcmp(filename_type_str,FILENAME_TYPE_INT) == 0) num = FILENAME_TYPE_INT_NUM;
	return num;
}

#define YES 1
#define NO 0
#define FAILED1 1
#define SUCCESS1 0
#define MAP_SIZE 256
#define EPSILON (2*LDBL_EPSILON)
#define BUF_SIZE 300
#define MED_BUF_SIZE 10000
#define ASCII_ALPHABET_SIZE 256
// the following symbol is "free" and CANNOT be used in the input text
#define FREE_CONST '`'



#define NOT_EXIST INT_MAX


// retrieve pSA from file
#define PSA_FN "psa.bin\0"
// retrieve SA from file
#define SA_FN "sa.bin\0"
// for PLF, need pSA_T from file
#define PSA_T_FN "../arrays/psa2.txt\0"
// anyone using construct() method on arr1,arr2, write here the inter of the two arr1[i],arr2[i]
#define DECIDING_ARR_FN "deciding_arr.bin\0"
// for LPF only, write the LPF to this file
#define LPF_FN "lpf.bin\0"
#define TEMP_ARR_FN "../arrays/temp.txt\0"
#define TEMP_ARR_FN2 "../arrays/temp2.txt\0"


//@PARAM: for ALGORITHM choice [psa.c: main]
// all algorithms work on char texts, except where noted

// OUR ALGORITHMS
#define AVERAGE_PSA 1
#define WORST_CASE_PSA2 2
#define WORST_CASE_PSA3 3
#define PLPF 5
#define PLCP_TCS 6
#define PLCP_TCS_IMP 7
#define PLCP_JDA 8
#define PERMUTED_PLCP_JDA 9
#define PLCP_JDA_IMP 10
#define PERMUTED_PLCP_JDA_IMP 11
#define PLNEF 12
#define PLRF 13
#define PSA_FOR_PLRF_AND_PLF 14
#define PLF 15
#define PBORDER 16

// OTHER ALGORITHMS
#define PLCP_NAIVE 0
#define NAIVE_PBORDER 17
#define LCP_KASAI 18
#define TRADITIONAL_SA_LCP 4
#define TRADITIONAL_SA 19
#define LPF_DIRECT_CROCHEMORE__LIGHT 20
#define LPF_DIRECT_CROCHEMORE 21 // int and char text
#define KS_INT_TRADITIONAL_SA 22 // int text only

//@PARAM:type for [[psa.c: void preprocess_SA_arrays(int out_arr[],short type,int SA[],int n);]]
#define BEFORE_LT 0
#define BEFORE_GT 1
#define AFTER_LT 2
#define AFTER_GT 3
#define REV_LT 4
#define REV_GT 5
#define NEQ_LT 6
#define NEQ_GT 7


//@ATTRIBUTE: type for [[struct wtuple]]
#define TRIVIAL 0
#define FIG_1A 1
#define FIG_1B 2
#define FIG_1C 3
#define FIG_1D 4
#define FIG_1E 5
#define FIG_1F 6
#define NUM_WCASES 7


struct pcode_d { int i; int di; };
struct pcode_f { int i; long double fi; };
struct wtuple { int j; int k; short type; };
struct wcases { int num[NUM_WCASES]; };
struct wcases_pair { struct wcases w1; struct wcases w2; };





char* fbuffer=NULL;
int* intfbuffer=NULL;
int* finalintfbuffer=NULL;
int* uintfbuffer=NULL;
int* remap_for_fun=NULL;
int* prevT=NULL;
int* prevQ1=NULL;
char* types_prevQ1=NULL;
int* prevQ2=NULL;
char* types_prevQ2=NULL;
int* forwT=NULL;
char* types_prevT=NULL;
int* remap_prev=NULL;
int *psuffixarray=NULL;
int *suffixarray=NULL;
int *Rankarray=NULL;
int *plcparray=NULL;
int *lcparray=NULL;
int *plpfarray=NULL;
int *lpfarray=NULL;
int *plnefarray=NULL;
int *pLF_array=NULL;
int *plrfarray=NULL;
int *pborderarray=NULL;
int *prefix_array=NULL;
int *suffix_array=NULL;
long double *prev_cdf=NULL;
int *before_lt_array=NULL;
int *before_gt_array=NULL;
int *neq_lt_array=NULL;
int *neq_gt_array=NULL;
int *after_lt_array=NULL;
int *after_gt_array=NULL;
int *rev_lt_array=NULL;
int *rev_gt_array=NULL;
int *neighbor_array=NULL;
int alpha_freq[MAP_SIZE];
int remap_alpha[MAP_SIZE];
short alpha_type[MAP_SIZE];
int max_distance=0;
int remap_prev_size=0;
short avg_case;
short RSORT_CONST=5;
int prev_cdf_size=0;
long double probability;
int MAKE_D_CONSTANT=1000;// average case constant
short VERIFY_PSA=1; // nonzero to verify pSA
short VERIFY_PLPF_VARIANTS=0; // nonzero to verify pLPF variants
short RUN_NAIVE_PBORDER_CHECK=0; // nonzero to run naive p-border validation
short DISPLAY_PSA_ERRORS=1;  // nonzero to display all pSA errors if pSA is invalid
char PSA_FILE[BUF_SIZE];
char SA_FILE[BUF_SIZE];
int largest_remapped_int;




#endif
