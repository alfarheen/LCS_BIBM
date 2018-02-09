#ifndef BITUTILS_C
#define BITUTILS_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "filefuns.c"
#include "ASCII_alphabets/alphas.c"
#define BITS_PER_CHAR 8
#define BITS_PER_INT (sizeof(int)*BITS_PER_CHAR)
#define BITS_PER_INT_TO_USE (BITS_PER_INT-2)  // subtract 2 because we don't want to touch the negative space and need space for remapping...
#define LF (char)10
#define MEDIUM_BUF_SIZE 1000




// general functions
int pack_int_prefix(char msb_to_lsb[],int num_bits);
void set_bit(int* val, int loc);
char get_bit(int val, int loc);
char get_bit_in_char_arr(char char_arr[], int bit_i);
char get_bit_in_packed_int_arr(int packed_int_arr[], char pack_prefix_num_bits, int packed_bit_i);
int get_total_char_bits(int char_arr_size);
char from_bit_to_ASCII(int bit);
char from_ASCII_to_bit(int ASCII_bit);
int get_position_in_packed_arr_of_original_bit(int packed_bit_i, char pack_prefix_num_bits);
int get_bitnum_in_packed_arr_of_original_bit(int packed_bit_i, char pack_prefix_num_bits);

// pack functions
int pack_char_arr_in_int_arr(char from_arr[], int from_arr_size, int to_arr[], int to_arr_size, char pack_prefix_num_bits);
int valid_pack(char orig_arr[], int orig_arr_size, int packed_arr[], int packed_arr_size, char pack_prefix_num_bits);
int get_total_ints_needed(int total_char_bits, char pack_prefix_num_bits);
void test_pack(void);
int valid_params(int orig_arr_size, int packed_arr_size, char pack_prefix_num_bits);
int read_file_and_pack_in_int_file(char ALPHABET_TYPE, char in_fn[], char out_fn[], char pack_prefix_num_bits);
int check_the_fun_read_file_and_pack_in_int_file(char in_fn[], char out_fn[]);
int unpack_the_packed_file(char unpack_in[], char packed_fn[], int orig_file_len);
int test_file_read_check(char infn[], char outfn[]);

// display functions
void print_bits_char(char val, int prefix_len);
void print_bits_int(int val, int prefix_len);
void print_bits(int val, int BITS_PER_TYPE, int prefix_len);
void print_arrays(char orig_arr[], int orig_arr_size, int to_arr[], int total_ints_needed);
void print_corresponding_bitstrings(char* orig_arr, int orig_arr_size, int total_char_bits, int* to_arr, char pack_prefix_num_bits);

// ./bitutils.exe $input_filename_to_pack $output_packed_filename $pack_prefix_num_bits 
int main(int argc, char* argv[]) {
  int returnval = 0, error = 0;
  char* INPUT_FILENAME_TO_PACK;
  char* OUTPUT_PACKED_FILENAME;
  char PACK_PREFIX_NUM_BITS;
  char ALPHABET_TYPE;
  int i;

  write_ASCII_alphabet_files();


  for(i=0; i<argc; i++) printf("argv[%d]=%s\n",i,argv[i]);

  printf("\n********************** bitutils ************************ \n\n");
  if(argc == 4){
    INPUT_FILENAME_TO_PACK = argv[1];
    OUTPUT_PACKED_FILENAME = argv[2];
    PACK_PREFIX_NUM_BITS = (char)atoi(argv[3]);
    
    printf("\nINPUT_FILENAME_TO_PACK=%s\nOUTPUT_PACKED_FILENAME=%s\nPACK_PREFIX_NUM_BITS=%d\n\n",INPUT_FILENAME_TO_PACK,OUTPUT_PACKED_FILENAME,PACK_PREFIX_NUM_BITS);

    ALPHABET_TYPE = get_minimal_ASCII_alphabet(INPUT_FILENAME_TO_PACK);
    printf("...using ALPHABET_TYPE=%d\n\n",ALPHABET_TYPE);
    //returnval = read_ASCII01_file_and_pack_in_int_file(INPUT_FILENAME_TO_PACK, OUTPUT_PACKED_FILENAME, PACK_PREFIX_NUM_BITS);
    returnval = read_file_and_pack_in_int_file(ALPHABET_TYPE, INPUT_FILENAME_TO_PACK, OUTPUT_PACKED_FILENAME, PACK_PREFIX_NUM_BITS);
    error = check_the_fun_read_file_and_pack_in_int_file(INPUT_FILENAME_TO_PACK, OUTPUT_PACKED_FILENAME);

    if(returnval >= 0 && error == 0) printf("\n... VALID PACK / UNPACK\n");
    else {
      printf("\n*** INVALID, ERROR, WRONG PACK / UNPACK ***\n");
      error = -2;
    }
  }else error = -3;
  printf("\nEXIT\n\n");
  //test_file_read_check("/home/alf/Documents/NEW_UBUNTU_compr_program/FINAL/arrays/target_RAW.txt", "/home/alf/Documents/NEW_UBUNTU_compr_program/FINAL/arrays/target_RAW.txt2");//("/home/alf/Downloads/bitstring.txt", "/home/alf/Downloads/bitstring2.bin");
  return error<0 ? error : 0;
}

int test_file_read_check(char infn[], char outfn[]) {
  char pack_prefix_num_bits;
  char ALPHABET_TYPE = get_minimal_ASCII_alphabet(infn);
  int returnval = 0, error = 0;
  for(pack_prefix_num_bits = 1; pack_prefix_num_bits <= BITS_PER_INT_TO_USE && error == 0; pack_prefix_num_bits++){
    //printf("@@ ALPHABET_TYPE = %d \npack_prefix_num_bits = %d\n", ALPHABET_TYPE, pack_prefix_num_bits);
    returnval = read_file_and_pack_in_int_file(ALPHABET_TYPE, infn, outfn, pack_prefix_num_bits);
    error = check_the_fun_read_file_and_pack_in_int_file(infn, outfn);
    if(returnval < 0 || error != 0) {
       printf("!!! ERROR !!! %d %d\n",returnval,error);
       error = 1;
    }else printf("...VALID \n\n");
  }
  return error;
}

char from_bit_to_ASCII(int bit){
  return bit != 0 ? '1' : '0';
}

char from_ASCII_to_bit(int ASCII_bit){
  return ASCII_bit == '0' ? 0 : 1;
}

int check_the_fun_read_file_and_pack_in_int_file(char in_fn[], char out_fn[]){
  int error = 0;
  char* unpack_in = NULL;
  int orig_file_len;
  FILE* in;
  char cbit;
  int i;
  in = fopen(in_fn,"r");
  if(in != NULL){
    orig_file_len = get_file_len2(in_fn);
//printf("[[[orig_file_len=%d]]]\n\n",orig_file_len);
    unpack_in = (char*)malloc((orig_file_len+2)*sizeof(char));
    if(unpack_in != NULL){
      unpack_in[orig_file_len]=unpack_in[orig_file_len+1]='\0';
      error = unpack_the_packed_file(unpack_in, out_fn, orig_file_len);
      for(i = 0; i < orig_file_len && error == 0; i++){
        cbit = read_next_char(in);
        if(i+1 == orig_file_len && (cbit == EOF || ferror(in) != 0 || cbit == LF)) break;
        if(cbit != unpack_in[i]) error = 3; // wrong bit!
        //printf("[%d,%d]",cbit,unpack_in[i]);
      }//printf("Unpacked: %s\n",unpack_in);
    }else error = 4;
    free(unpack_in);
  }else error = 1;
  fclose(in);
  return error;
}

// should allocate unpack_in at least orig_file_len, since the last symbol can be a LF
int unpack_the_packed_file(char unpack_in[], char packed_fn[], int orig_file_len){
  int file_len;
  int i, j, ii;
  FILE* in2;
  int temp;
  char ctemp;
  int total_orig_unpacked = 0;
  int error = 0;
  int done = 0;
  char ALPHABET_TYPE;
  char pack_prefix_num_bits;
  char BINARY_TO_ASCII_MAP[CH_BUF_SIZE];
  int fixed_length_binary;
  char alphabet_fn[MEDIUM_BUF_SIZE];
  int binary_rep = 0;
  char binary_num;

  in2 = fopen(packed_fn,"rb");
  if(in2 != NULL){
    // header info
    ALPHABET_TYPE = read_next_char2(in2);
    pack_prefix_num_bits = read_next_char2(in2);
    read_next_char2(in2); // reserved
    read_next_char2(in2); // reserved

printf("UNPACK: ALPHABET_TYPE = %d, pack_prefix_num_bits = %d\n\n",ALPHABET_TYPE,pack_prefix_num_bits);
    get_alphabet_filename(ALPHABET_TYPE,alphabet_fn);
    fixed_length_binary = set_ASCII_alphabet_map_from_file(alphabet_fn,NULL,BINARY_TO_ASCII_MAP);
    file_len = get_file_len2(packed_fn);
//printf("[[[file_len=%d]]]\n\n",file_len);
    binary_num = fixed_length_binary - 1;
    for(i = 0; i < file_len && done == 0; i++){
      temp = read_next_int(in2);
      for(j = BITS_PER_INT_TO_USE-1; j >= (int)BITS_PER_INT_TO_USE - pack_prefix_num_bits && done == 0; j--){
        ctemp = get_bit(temp,j);
        if(ctemp != 0) set_bit(&binary_rep,binary_num);
        binary_num--;
        if(binary_num < 0){ 
          unpack_in[total_orig_unpacked++] = BINARY_TO_ASCII_MAP[binary_rep];
          binary_rep = 0;
          binary_num = fixed_length_binary - 1;
        }
        if(total_orig_unpacked == orig_file_len) done = 1;
      }
    }
  }else error = 1;
  
  fclose(in2);
  
  return error;
}

// give file with alphabet ALPHABET_TYPE, we pack this into the file: <ALPHABET_TYPE><pack_prefix_num_bits><packed_int><packed_int>...<packed_int>
/// if return >=0, contains number of ints written/packed ; otherwise, error
int read_file_and_pack_in_int_file(char ALPHABET_TYPE, char in_fn[], char out_fn[], char pack_prefix_num_bits){
  int file_len;
  int i, ii, jj;
  char msb_to_lsb[BITS_PER_INT];
  char leftover_bits[BITS_PER_INT*2];
  int leftover_bits_num = 0;
  char binary_rep;
  int bit_num = 0;
  FILE* infile = fopen(in_fn,"rb");
  FILE* outfile = fopen(out_fn,"wb");
  int error = 0;
  char ctemp;
  char curr_bit;
  int num_ints_written=0;
  char ASCII_TO_BINARY_MAP[CH_BUF_SIZE];
  char alphabet_fn[MEDIUM_BUF_SIZE];
  int fixed_length_binary;
  int write_this;
  int end = 0;

printf("PACK: ALPHABET_TYPE =%d\n",ALPHABET_TYPE);
  get_alphabet_filename(ALPHABET_TYPE,alphabet_fn);
  fixed_length_binary = set_ASCII_alphabet_map_from_file(alphabet_fn,ASCII_TO_BINARY_MAP,NULL);

  if(fixed_length_binary > 0 && infile != NULL && outfile != NULL && pack_prefix_num_bits > 0 && pack_prefix_num_bits <= BITS_PER_INT_TO_USE){
printf("PACK: ALPHABET_TYPE =%d\n",ALPHABET_TYPE);
    // header info
    write_char(outfile,ALPHABET_TYPE);
    write_char(outfile,pack_prefix_num_bits);
    write_char(outfile,0); // reserved
    write_char(outfile,0); // reserved

    file_len = get_file_len2(in_fn);
    for(i = 0; i < file_len; i++){
     ctemp = read_next_char2(infile);
     binary_rep = ASCII_TO_BINARY_MAP[ctemp];
//printf("ctemp %d binary_rep %d map['0']=%d  map['1']=%d \n",ctemp,binary_rep, ASCII_TO_BINARY_MAP['0'], ASCII_TO_BINARY_MAP['1']);
     if(i+1 == file_len && (ctemp == EOF || ferror(infile) != 0 || ctemp == LF)) end = 1;
     
     
      for(ii=fixed_length_binary-1; ii >= 0 && end == 0; ii--) {
        curr_bit = get_bit(binary_rep, ii);
//printf("curr_bit=%d\n",curr_bit);
        if(bit_num < pack_prefix_num_bits) msb_to_lsb[bit_num++]=curr_bit;
        else leftover_bits[leftover_bits_num++] = curr_bit;
//printf("bit_num = %d , leftover_num = %d, i=%d, file_len=%d\n", bit_num, leftover_bits_num, i, file_len);

      }
      if(bit_num == pack_prefix_num_bits || i+1 == file_len){
        do{
//printf("do{\n");
          write_this = pack_int_prefix(msb_to_lsb, bit_num);
//printf("write_this=%d, bit_num=%d, leftover=%d\n",write_this, bit_num, leftover_bits_num);
          write_int_or_int(outfile, write_this, 1);
//printf("leftover_bits_num=%d  wrote %d\n",leftover_bits_num,write_this);


          num_ints_written++;
          bit_num = 0;

	  for(ii = 0; ii < leftover_bits_num && bit_num < pack_prefix_num_bits; ii++){
            msb_to_lsb[bit_num++]=leftover_bits[ii];
          }
          leftover_bits_num-=ii;
          for(jj = ii; jj < leftover_bits_num+ii; jj++) leftover_bits[jj-ii]=leftover_bits[jj];
//printf("&%d, leftover=%d, bit_num=%d t=%d\n",write_this, leftover_bits_num, bit_num, (i+1 == file_len && leftover_bits > 0 || bit_num == pack_prefix_num_bits));
        }while(i+1 == file_len && bit_num > 0 || bit_num == pack_prefix_num_bits);
      }
     }
    
  }else error = -1;
  fclose(infile);
  fclose(outfile);
  return error<0 ? error : num_ints_written;
}

char get_bit_in_char_arr(char char_arr[], int bit_i){
  int index_num = bit_i / BITS_PER_CHAR;
  return get_bit(char_arr[index_num], BITS_PER_CHAR - (bit_i % BITS_PER_CHAR) - 1);
}

char get_bit_in_packed_int_arr(int packed_int_arr[], char pack_prefix_num_bits, int packed_bit_i){
  int index_num = get_position_in_packed_arr_of_original_bit(packed_bit_i, pack_prefix_num_bits);
  int bit_num = get_bitnum_in_packed_arr_of_original_bit(packed_bit_i, pack_prefix_num_bits);
  return get_bit(packed_int_arr[index_num], bit_num);
}

int get_position_in_packed_arr_of_original_bit(int packed_bit_i, char pack_prefix_num_bits){
  return packed_bit_i / pack_prefix_num_bits;
}

int get_bitnum_in_packed_arr_of_original_bit(int packed_bit_i, char pack_prefix_num_bits){
  return BITS_PER_INT_TO_USE - (packed_bit_i % pack_prefix_num_bits) - 1;
}

// msb_to_lsb is list of bits, where msb_to_lsb[0] is the msb
int pack_int_prefix(char msb_to_lsb[],int num_bits){
  int final_val = 0;
  int loc;

  if(num_bits <= BITS_PER_INT_TO_USE){
    for(loc = 0; loc < num_bits; loc++){
      if(msb_to_lsb[loc] != 0){
        set_bit(&final_val, (BITS_PER_INT_TO_USE - loc - 1));
      }
    }
  }
  return final_val;
}

// will set 2^loc bit of val to 1 (note, int is 2^31, 2^16, ..., 2^1, 2^0)
void set_bit(int* val, int loc){
  (*val) = (*val) | (1 << loc); 
}

// will get bit of 2^loc location in val (note, int is 2^31, 2^16, ..., 2^1, 2^0)
char get_bit(int val, int loc){
  return (val & (1 << loc)) != 0;
}

int get_total_char_bits(int char_arr_size){
  return char_arr_size * BITS_PER_CHAR;
}

int get_total_ints_needed(int total_char_bits, char pack_prefix_num_bits){
  return ceil(total_char_bits / (double)pack_prefix_num_bits);
}

// check invalid size OR invalid pack_prefix_num_bits
int valid_params(int from_char_arr_size, int to_int_arr_size, char pack_prefix_num_bits){
  int valid = 1;
  int total_char_bits = get_total_char_bits(from_char_arr_size);
  int total_ints_needed = get_total_ints_needed(total_char_bits,pack_prefix_num_bits);

  if(to_int_arr_size < total_ints_needed || pack_prefix_num_bits <= 0 || pack_prefix_num_bits > BITS_PER_INT_TO_USE){
    valid = 0;
    printf("\n*** INVALID PARAMETERS ***\n");
    printf("... from_char_arr_size = %d\n",from_char_arr_size);
    printf("... to_int_arr_size = %d\n",to_int_arr_size);
    printf("... pack_prefix_num_bits = %d\n",pack_prefix_num_bits);
    printf("... total_char_bits = %d\n",total_char_bits);
    printf("... total_ints_needed = %d\n\n",total_ints_needed);
  }
  return valid;
}


// returns -1 if size of to_arr is not enough; otherwise returns the number of elements in to_arr
int pack_char_arr_in_int_arr(char from_arr[], int from_arr_size, int to_arr[], int to_arr_size, char pack_prefix_num_bits){
  char msb_to_lsb[BITS_PER_INT_TO_USE];
  int from_arr_num;
  int to_arr_num = 0;
  int from_bit_num;
  int msb_to_lsb_num = 0;
  int last_entry = 0;
  int valid = 1;

  valid = valid_params(from_arr_size, to_arr_size, pack_prefix_num_bits);
  if(valid == 0) to_arr_num = -1;

  for(from_arr_num = 0; from_arr_num < from_arr_size && last_entry == 0 && valid != 0; from_arr_num++){
    from_bit_num = BITS_PER_CHAR-1;
    while(last_entry == 0 && from_bit_num >= 0 && msb_to_lsb_num < pack_prefix_num_bits){
      msb_to_lsb[msb_to_lsb_num++] = get_bit(from_arr[from_arr_num],from_bit_num--);
      if(from_arr_num + 1 == from_arr_size && from_bit_num < 0) last_entry = 1;
      if(msb_to_lsb_num == pack_prefix_num_bits || last_entry){
        to_arr[to_arr_num++] = pack_int_prefix(msb_to_lsb,msb_to_lsb_num);
        msb_to_lsb_num = 0;
      }
    }
  }
  return to_arr_num;
}

int valid_pack(char orig_arr[], int orig_arr_size, int packed_arr[], int packed_arr_size, char pack_prefix_num_bits) {
  int valid = 1;
  int char_bit, int_bit;
  int i;

  valid = valid_params(orig_arr_size, packed_arr_size, pack_prefix_num_bits);
  for(i = 0; i < get_total_char_bits(orig_arr_size); i++){
    char_bit = get_bit_in_char_arr(orig_arr, i);
    int_bit = get_bit_in_packed_int_arr(packed_arr, pack_prefix_num_bits, i);
    if(char_bit != int_bit) {
       printf("*** mismatch: orig_arr[bit=%d]=%d NEQ packed_arr[bit=%d]=%d ***\n",i,char_bit,i,int_bit);
       valid = 0;
    }
  }

  return valid;
}

void print_bits_int(int val, int prefix_len){
  print_bits(val, BITS_PER_INT_TO_USE, prefix_len);
}

void print_bits_char(char val, int prefix_len){
  print_bits(val, BITS_PER_CHAR, prefix_len);
}

void print_bits(int val, int BITS_PER_TYPE, int prefix_len){
  int loc;
  for(loc = BITS_PER_TYPE-1; loc >= 0 && (BITS_PER_TYPE - loc <= prefix_len); loc--){
    printf("%c",from_bit_to_ASCII((val & (1 << loc))));
    if(loc == 0) break;
  }
}

void test_pack(void){
  char orig_arr[] = {1,2,4,8,16,21,64,128,255,88,74};
  int orig_arr_size = sizeof(orig_arr);

  #define TO_ARR_SIZE 100000
  int to_arr[TO_ARR_SIZE];
  int to_arr_size = TO_ARR_SIZE;

  char pack_prefix_num_bits;
  int from_pack_prefix_num_bits;
  int total_char_bits = get_total_char_bits(orig_arr_size);
  int total_ints_needed;
  int len_packed_arr;
  int len_unpacked_arr;
  int valid = 1;
   
  for(pack_prefix_num_bits = 1; pack_prefix_num_bits <= BITS_PER_INT_TO_USE && valid != 0; pack_prefix_num_bits++){
    printf("\n-------------------------\n pack_prefix_num_bits = %d\n", pack_prefix_num_bits);
    total_ints_needed = get_total_ints_needed(get_total_char_bits(orig_arr_size), pack_prefix_num_bits);
    len_packed_arr = pack_char_arr_in_int_arr(orig_arr, orig_arr_size, to_arr, to_arr_size, pack_prefix_num_bits);
    valid = valid_pack(orig_arr, orig_arr_size, to_arr, len_packed_arr, pack_prefix_num_bits);
    if(valid == 0) printf("\n!!! INVALID !!!\n");
    //print_arrays(orig_arr, orig_arr_size, to_arr, total_ints_needed);
    print_corresponding_bitstrings(orig_arr, orig_arr_size, total_char_bits, to_arr, pack_prefix_num_bits);
  }
}

void print_arrays(char orig_arr[], int orig_arr_size, int to_arr[], int total_ints_needed){  
  int i;
  printf("... char orig_arr[] =\n");
  for(i=0; i < orig_arr_size; i++){
    printf("[");
    print_bits_char(orig_arr[i], BITS_PER_CHAR);
    printf("]");
  }
  printf("\n\n... int to_arr[] =\n");
  for(i=0; i < total_ints_needed; i++){
    print_bits_int(to_arr[i], BITS_PER_INT_TO_USE);
    printf("\n");
  }
  printf("\n\n");
}

void print_corresponding_bitstrings(char* orig_arr, int orig_arr_size, int total_char_bits, int* to_arr, char pack_prefix_num_bits){  
  int i;
  int printed_num = 0;
  int print_num = 0;
  if(orig_arr != NULL){
    printf("... char_arr :\n");
    for(i=0; i < orig_arr_size; i++){
      print_bits_char(orig_arr[i], BITS_PER_CHAR);
    }
    printf("\n");
  }
  if(to_arr != NULL){
    printf("\n... from to_arr :\n");
    for(i = 0; i < total_char_bits; i++) printf("%c",(from_bit_to_ASCII(get_bit_in_packed_int_arr(to_arr, pack_prefix_num_bits, i))));
    printf("\n\n");
  }
}

#endif
