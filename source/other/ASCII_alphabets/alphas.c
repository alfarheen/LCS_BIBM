#ifndef ALPHAS_C
#define ALPHAS_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "../filefuns.c"

#define CH_BUF_SIZE 256
#define UNDEFINED_MAP_ENTRY -1


// --- ASCII 0/1 BINARY
#define ALPHABET_TYPE_ASCII_01 0  // 0, 1
char* ALPHA_ASCII_01_FN = "other/ASCII_alphabets/ASCII_01_BINARY.txt";

// --- ASCII DNA SMALL
#define ALPHABET_TYPE_ASCII_DNA_SMALL 1
char ASCII_DNA_SMALL_ALPHA[] = {'A','C','G','K','M','R','S','T','W','Y','N'}; // added 'N' just in case
#define ASCII_DNA_SMALL_ALPHA_SIZE (sizeof(ASCII_DNA_SMALL_ALPHA)/sizeof(char))
char* ASCII_DNA_SMALL_FN = "other/ASCII_alphabets/ASCII_DNA_SMALL.txt";

// --- ASCII DNA LARGE
#define ALPHABET_TYPE_ASCII_DNA_LARGE 2
char ASCII_DNA_LARGE_ALPHA[] = {'A','C','G','K','M','R','S','T','W','Y','a','c','g','k','m','n','r','s','t','w','y'};
#define ASCII_DNA_LARGE_ALPHA_SIZE (sizeof(ASCII_DNA_LARGE_ALPHA)/sizeof(char))
char* ASCII_DNA_LARGE_FN = "other/ASCII_alphabets/ASCII_DNA_LARGE.txt";

// --- ASCII UPPERCASE
#define ALPHABET_TYPE_ASCII_UPPERCASE_LETTERS 3  // A, B, ...
char* ALPHA_ASCII_UPPERCASE_LETTERS_FN = "other/ASCII_alphabets/ASCII_UPPERCASE_LETTERS.txt";

// --- ASCII lowercase
#define ALPHABET_TYPE_ASCII_LOWERCASE_LETTERS 4  // a, b, ...
char* ALPHA_ASCII_LOWERCASE_LETTERS_FN = "other/ASCII_alphabets/ASCII_LOWERCASE_LETTERS.txt";

// --- ASCII ALL [0,127]
#define ALPHABET_TYPE_ASCII_ALL 5 
char* ALPHA_ASCII_ALL_FN = "other/ASCII_alphabets/ASCII_ALL.txt";


// --- ALPHABET TYPES ORDERED BY SIZES (ASCENDING)
char ALPHABET_TYPES_SORTED_BY_LENGTH[] = { ALPHABET_TYPE_ASCII_01, ALPHABET_TYPE_ASCII_DNA_SMALL, ALPHABET_TYPE_ASCII_DNA_LARGE, ALPHABET_TYPE_ASCII_UPPERCASE_LETTERS, ALPHABET_TYPE_ASCII_LOWERCASE_LETTERS, ALPHABET_TYPE_ASCII_ALL };
#define ALPHABET_TYPES_SORTED_BY_LENGTH_SIZE (sizeof(ALPHABET_TYPES_SORTED_BY_LENGTH)/sizeof(char))



void get_alphabet_filename(char ALPHABET_TYPE, char in_fn[]);
int set_ASCII_alphabet_map_from_file(char fn[], char* ASCII_TO_BINARY_MAP, char* BINARY_TO_ASCII_MAP);
void write_ASCII_alphabet_files();
void write_all_ASCII_alphabet_symbols_to_file(char fn[], char lower_inclusive, char upper_inclusive);
char get_minimal_ASCII_alphabet(char in_fn[]);
int alphabet_map_is_contained_in_another(char ALPHABET_MAP_A[CH_BUF_SIZE], char ALPHABET_MAP_B[CH_BUF_SIZE]);




// returns the minimal alphabet index needed for the input string in in_fn
char get_minimal_ASCII_alphabet(char in_fn[]){
  char minimal_alphabet_index = ALPHABET_TYPE_ASCII_ALL;
  char target_ASCII_TO_BINARY_MAP[CH_BUF_SIZE];
  char other_ASCII_TO_BINARY_MAP[CH_BUF_SIZE];
  int i;
  char curr_alpha_type;
  char alpha_fn[CH_BUF_SIZE];

  printf("Finding minimal fixed alphabet for %s...\n", in_fn);

  set_ASCII_alphabet_map_from_file(in_fn, target_ASCII_TO_BINARY_MAP, NULL);
  
  for(i=0; i<ALPHABET_TYPES_SORTED_BY_LENGTH_SIZE; i++){
    curr_alpha_type = ALPHABET_TYPES_SORTED_BY_LENGTH[i];
    get_alphabet_filename(curr_alpha_type, alpha_fn);
    printf("... considering %s (id=%d) ...\n", alpha_fn, curr_alpha_type);
    set_ASCII_alphabet_map_from_file(alpha_fn, other_ASCII_TO_BINARY_MAP, NULL);

    if(alphabet_map_is_contained_in_another(target_ASCII_TO_BINARY_MAP, other_ASCII_TO_BINARY_MAP) != 0){
      minimal_alphabet_index = curr_alpha_type;
      printf("...... success!\n");
      break;
    }
  }

  return minimal_alphabet_index;
}

// returns nonzero is ALPHABET_MAP_A is contained in ALPHABET_MAP_B
int alphabet_map_is_contained_in_another(char ALPHABET_MAP_A[CH_BUF_SIZE], char ALPHABET_MAP_B[CH_BUF_SIZE]){
  int contained = 1;
  char i;

  for(i = 0; i<CHAR_MAX; i++){
    if(i != UNDEFINED_MAP_ENTRY && ALPHABET_MAP_A[i] != UNDEFINED_MAP_ENTRY && ALPHABET_MAP_B[i] == UNDEFINED_MAP_ENTRY){
      printf("...... failed on symbol [int_rep=%d, char=%c]\n",i,i);
      contained = 0;
      break;
    }
  }
  return contained;
}

void write_ASCII_alphabet_files(){
  write_all_ASCII_alphabet_symbols_to_file(ALPHA_ASCII_01_FN,'0','1');
  write_all_ASCII_alphabet_symbols_to_file(ALPHA_ASCII_UPPERCASE_LETTERS_FN,'A','Z');
  write_all_ASCII_alphabet_symbols_to_file(ALPHA_ASCII_LOWERCASE_LETTERS_FN,'a','z');
  write_all_ASCII_alphabet_symbols_to_file(ALPHA_ASCII_ALL_FN,0,127);
  write_char_arr(ASCII_DNA_SMALL_FN,ASCII_DNA_SMALL_ALPHA,ASCII_DNA_SMALL_ALPHA_SIZE);
  write_char_arr(ASCII_DNA_LARGE_FN,ASCII_DNA_LARGE_ALPHA,ASCII_DNA_LARGE_ALPHA_SIZE);
}

void write_all_ASCII_alphabet_symbols_to_file(char fn[], char lower_inclusive, char upper_inclusive){
  FILE *fout = fopen(fn,"w");
  char i;
  for(i=lower_inclusive; ; i++){
    write_char(fout,i);
    if(i == upper_inclusive) break;
  }
  fclose(fout);
}

/*
// return number of bits in binary, returns maps for alphabet
int set_ASCII_alphabet_map_from_file(char fn[], char* ASCII_TO_BINARY_MAP, char* BINARY_TO_ASCII_MAP){
  int i;
  char ALPHABET[CH_BUF_SIZE];
  int SIZE;
  char curr = 0, inc_curr = 0;
  int fixed_len = 0;

    for(i = 0; i < (int)CH_BUF_SIZE; i++){
      if(ASCII_TO_BINARY_MAP != NULL) ASCII_TO_BINARY_MAP[i]=UNDEFINED_MAP_ENTRY;
      if(BINARY_TO_ASCII_MAP != NULL) BINARY_TO_ASCII_MAP[i]=UNDEFINED_MAP_ENTRY;
    }
    ALPHABET[0] = '\0';
    SIZE = read_char_arr(fn, ALPHABET, get_file_len2(fn));
    no_ending_cr(ALPHABET, &SIZE);
printf("%s\n",ALPHABET);
    //SIZE = strlen(ALPHABET);
    fixed_len = (SIZE <= 0) ? 0 : (int)ceil(log2(SIZE));
//printf("SIZE=%d, fixed_len=%d, alphabet=[%s]\n",SIZE,fixed_len,ALPHABET);
    for(i = 0; i < SIZE; i++) {
      inc_curr = 0;
      if(ASCII_TO_BINARY_MAP != NULL && ASCII_TO_BINARY_MAP[ALPHABET[i]] == UNDEFINED_MAP_ENTRY){
        inc_curr = 1;
        ASCII_TO_BINARY_MAP[ALPHABET[i]] = curr;
//printf("A2B[%c]=%d\n",ALPHABET[i],ASCII_TO_BINARY_MAP[ALPHABET[i]]);
      }
      if(BINARY_TO_ASCII_MAP != NULL){
        inc_curr = 1;
        BINARY_TO_ASCII_MAP[curr] = ALPHABET[i]; 
      }
      if(inc_curr != 0) curr++;
    }
  return fixed_len;
}*/

// return number of bits in binary, returns maps for alphabet
int set_ASCII_alphabet_map_from_file(char fn[], char* ASCII_TO_BINARY_MAP, char* BINARY_TO_ASCII_MAP){
  int i;
  char* ALPHABET;
  int SIZE;
  int curr = 0, inc_curr = 0;
  int fixed_len = 0;

    for(i = 0; i < CH_BUF_SIZE; i++){
//printf("i = %d\n", i);
      if(ASCII_TO_BINARY_MAP != NULL) ASCII_TO_BINARY_MAP[i]=UNDEFINED_MAP_ENTRY;
      if(BINARY_TO_ASCII_MAP != NULL) BINARY_TO_ASCII_MAP[i]=UNDEFINED_MAP_ENTRY;
//printf("i = %d, A2B[i]=%d\n", i, ASCII_TO_BINARY_MAP[i]);
    }
    SIZE = get_file_len2(fn);
    ALPHABET=(char *)malloc((SIZE+2)*(sizeof(char))); 
    ALPHABET[SIZE]=ALPHABET[SIZE+1]='\0';
    read_char_arr(fn, ALPHABET, SIZE);
    no_ending_cr(ALPHABET, &SIZE);

    //SIZE = strlen(ALPHABET);
//printf("SIZE=%d, fixed_len=%d, alphabet=[%s]\n",SIZE,fixed_len,ALPHABET);
    for(i = 0; i < SIZE; i++) {
      inc_curr = 0;
//printf("ALPHABET[%d] = %d, A2B[Alpha[i]]=%d\n", i, ALPHABET[i], ASCII_TO_BINARY_MAP[ALPHABET[i]]);
      if(ASCII_TO_BINARY_MAP != NULL && ASCII_TO_BINARY_MAP[ALPHABET[i]] == UNDEFINED_MAP_ENTRY){
        inc_curr = 1;
        ASCII_TO_BINARY_MAP[ALPHABET[i]] = curr;
//printf("A2B[%c]=%d\n",ALPHABET[i],ASCII_TO_BINARY_MAP[ALPHABET[i]]);
      }
      if(BINARY_TO_ASCII_MAP != NULL){
        inc_curr = 1;
        BINARY_TO_ASCII_MAP[curr] = ALPHABET[i]; 
      }
      if(inc_curr != 0) curr++;
    }
  free(ALPHABET);
  fixed_len = (curr <= 0) ? 0 : (int)ceil(log2(curr));
  return fixed_len;
}

void get_alphabet_filename(char ALPHABET_TYPE, char in_fn[]){
  switch(ALPHABET_TYPE){
    case ALPHABET_TYPE_ASCII_01: strcpy(in_fn, ALPHA_ASCII_01_FN); break;
    case ALPHABET_TYPE_ASCII_DNA_SMALL: strcpy(in_fn, ASCII_DNA_SMALL_FN); break;
    case ALPHABET_TYPE_ASCII_DNA_LARGE: strcpy(in_fn, ASCII_DNA_LARGE_FN); break;
    case ALPHABET_TYPE_ASCII_UPPERCASE_LETTERS: strcpy(
in_fn, ALPHA_ASCII_UPPERCASE_LETTERS_FN); break;
    case ALPHABET_TYPE_ASCII_LOWERCASE_LETTERS: strcpy(in_fn, ALPHA_ASCII_LOWERCASE_LETTERS_FN); break;
    case ALPHABET_TYPE_ASCII_ALL: strcpy(in_fn, ALPHA_ASCII_ALL_FN); break;
    default: in_fn[0] = '\0'; break;
  }
}


#endif

