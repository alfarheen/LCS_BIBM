#!/bin/bash

################################################################################################################
################################################################################################################
################################################################################################################
################################################################################################################
################################################################################################################
################################################################################################################
################################################################################################################
################################################################################################################
################################################################################################################
################################################################################################################
####### Below are the DATA MODES that exist:
#######
####### A: SINGLE-STRING METHOD
#######
#######    1. work with the RAW (PAYLOAD) string:
####### 	..... set MAKE_UPPERCASE_AND_CASE_BITSTRING = false
####### 	..... set PAYLOAD_PACK_PREFIX_NUM_BITS = 0
#######
#######    2. work with the *PACKED* RAW (PAYLOAD) string:
####### 	..... set MAKE_UPPERCASE_AND_CASE_BITSTRING = false
####### 	..... set PAYLOAD_PACK_PREFIX_NUM_BITS = {1,2,...,30}
#######
####### B: TWO-STRING METHOD
#######
#######    NOTE: toggle CONSTRUCT_BITSTRING_PART and CONSTRUCT_PAYLOAD_PART flags to construct 
#######          only the parts of the payload+bitstring combo desired
#######
#######    1. work with the UPPERCASE RAW string (PAYLOAD) and the case BITSTRING: 
####### 	..... set MAKE_UPPERCASE_AND_CASE_BITSTRING = true
#######         ..... set PAYLOAD_PACK_PREFIX_NUM_BITS = 0
####### 	..... set BITSTRING_PACK_PREFIX_NUM_BITS = 0
#######
#######    2. work with the *PACKED* UPPERCASE PAYLOAD and *PACKED* BITSTRING
####### 	..... set MAKE_UPPERCASE_AND_CASE_BITSTRING = true
#######         ..... set PAYLOAD_PACK_PREFIX_NUM_BITS = {1,2,...,30}
####### 	..... set BITSTRING_PACK_PREFIX_NUM_BITS = {1,2,...,30}
#######
#######    3. work with the UPPERCASE PAYLOAD and *PACKED* BITSTRING:
####### 	..... set MAKE_UPPERCASE_AND_CASE_BITSTRING = true
#######         ..... set PAYLOAD_PACK_PREFIX_NUM_BITS = 0
####### 	..... set BITSTRING_PACK_PREFIX_NUM_BITS = {1,2,...,30}
#######
#######    4. work with the *PACKED* UPPERCASE PAYLOAD and BITSTRING
####### 	..... set MAKE_UPPERCASE_AND_CASE_BITSTRING = true
#######         ..... set PAYLOAD_PACK_PREFIX_NUM_BITS = {1,2,...,30}
####### 	..... set BITSTRING_PACK_PREFIX_NUM_BITS = 0
#######
#######
################################################################################################################
################################################################################################################
################################################################################################################
################################################################################################################
################################################################################################################
################################################################################################################
################################################################################################################
################################################################################################################
################################################################################################################
################################################################################################################
####### CHANGE THESE PARAMS
#######
#######
####### MAKE THE LPF AND DECIDING_ARR FIRST, THEN COMPRESS/DECOMPRESS
construct_SA_LPF=true # preprocessing needed for compression/decompression; true=construct, false=do not construct and instead use the files located in ORIG_ARRAYS_FOLDER
run_compress_decompress=true # true=compress/decompress, false=do not compress/decompress
#######
#######
####### NOTE:
####### FOR TARGET AND REF, MUST USE THE "RAW" FORM OF THE CHROMOSOME (example below).  WE ALWAYS ASSUME THAT WE NEED TO REMOVE THE FIRST LINE!
#######
####### RAW CHROMOSOME EXAMPLE:
#######
####### >EG: 1 dna:chromosome chromosome:MSU6:1:1:43268879:1
####### CTAAACCCTAAACCCTAAACCCTAAACCCTAAACCCTAAACCCTAAACCCTAAACCCTAA
####### CCCTAAACCCTAACCCTAAACCCTAAACCCTAAACCCTAAACCCTAAACCCTAAACAGCT
####### ...
#######
############## FOR TARGET, REFERENCE, ORIG_ARRAYS_FOLDER, and OUTPUT_COMPRESSED_FILES_FOLDER, USE ABSOLUTE PATHS!
PROGRAM_DIR="/home/alf/Documents/NEW_UBUNTU_compr_program/source"
TARGET="/home/alf/Downloads/t2" #"/home/alf/Downloads/oryza_sativa_TIGR5/Oryza_sativa.MSU6.dna.chromosome.1.fa" #"/home/alf/Downloads/t2"
REFERENCE="/home/alf/Downloads/t1" #"/home/alf/Downloads/oryza_sativa_TIGR5/Oryza_sativa.MSU6.dna.chromosome.1.fa"  #"/home/alf/Downloads/t1"
ORIG_ARRAYS_FOLDER="/home/alf/Documents/NEW_UBUNTU_compr_program/all_results" # location for the output files/arrays
#######
####### ------------------
####### PAYLOAD PARAMETERS
####### ------------------
K_LOW=5
K_HIGH=5
K_INCREMENT=1
SCHEME=1 # 1 or 2
PAYLOAD_PACK_PREFIX_NUM_BITS=25 # 0=do not pack payload; otherwise, =number of bits to pack, value between 1 and 30 inclusive
#######
####### --------------------
####### BITSTRING PARAMETERS
####### --------------------
#######
MAKE_UPPERCASE_AND_CASE_BITSTRING=true  # true=yes(make uppercase & case bitstring), false=no(use only raw file)
####### IF SELECT MAKE_UPPERCASE_AND_CASE_BITSTRING=true, MUST SPECIFY ALL PARAMETERS BELOW ;; ELSE CAN OMIT
BITSTRING_PACK_PREFIX_NUM_BITS=30 # if MAKE_UPPERCASE_AND_CASE_BITSTRING, =number of bits to pack, value between 1 and 30 inclusive
K_LOW_BITSTRING=$K_LOW
K_HIGH_BITSTRING=$K_HIGH
K_INCREMENT_BITSTRING=$K_INCREMENT
SCHEME_BITSTRING=$SCHEME # 1 or 2
CONSTRUCT_BITSTRING_PART=true # these parameters are used to construct only the
CONSTRUCT_PAYLOAD_PART=true # part of the payload+bitstring combo desired
################################################################################################################
################################################################################################################
################################################################################################################
################################################################################################################
################################################################################################################
################################################################################################################
################################################################################################################
################################################################################################################
################################################################################################################
################################################################################################################







echo "construct_SA_LPF = " $construct_SA_LPF
echo "run_compress_decompress = " $run_compress_decompress
echo "--- FOLDERS ---"
echo "PROGRAM_DIR        = " $PROGRAM_DIR
echo "TARGET             = " $TARGET
echo "REFERENCE          = " $REFERENCE
echo "ORIG_ARRAYS_FOLDER = " $ORIG_ARRAYS_FOLDER
echo "--- PAYLOAD PARAMETERS ---"
echo "K_LOW       = " $K_LOW
echo "K_HIGH      = " $K_HIGH
echo "K_INCREMENT = " $K_INCREMENT
echo "SCHEME      = " $SCHEME
echo "PAYLOAD_PACK_PREFIX_NUM_BITS = " $PAYLOAD_PACK_PREFIX_NUM_BITS
echo "--- BITSTRING PARAMETERS ---"
echo "MAKE_UPPERCASE_AND_CASE_BITSTRING      = " $MAKE_UPPERCASE_AND_CASE_BITSTRING
if [ $MAKE_UPPERCASE_AND_CASE_BITSTRING = true ]; then
	echo "BITSTRING_PACK_PREFIX_NUM_BITS = " $BITSTRING_PACK_PREFIX_NUM_BITS
	echo "K_LOW_BITSTRING                = " $K_LOW_BITSTRING
	echo "K_HIGH_BITSTRING               = " $K_HIGH_BITSTRING
	echo "K_INCREMENT_BITSTRING          = " $K_INCREMENT_BITSTRING
	echo "SCHEME_BITSTRING               = " $SCHEME_BITSTRING
	echo "CONSTRUCT_BITSTRING_PART       = " $CONSTRUCT_BITSTRING_PART
	echo "CONSTRUCT_PAYLOAD_PART         = " $CONSTRUCT_PAYLOAD_PART
fi
printf "\n\n"






#-----
# GENERATE FOLDERS
TARGET_PATH=`dirname $TARGET`
TARGET_FOLDER=`basename $TARGET_PATH`
TARGET_FILENAME="$(basename $TARGET)"
TARGET_NAME=$TARGET_FOLDER"--"$TARGET_FILENAME
REFERENCE_PATH=`dirname $REFERENCE`
REFERENCE_FOLDER=`basename $REFERENCE_PATH`
REFERENCE_FILENAME="$(basename $REFERENCE)"
REFERENCE_NAME=$REFERENCE_FOLDER"--"$REFERENCE_FILENAME
ORIG_ARRAYS_FOLDER=$ORIG_ARRAYS_FOLDER"/TARGET_["$TARGET_NAME"]____REFERENCE_["$REFERENCE_NAME"]"
ORIG_ARRAYS_COMPRESSED_FILES_FOLDER=$ORIG_ARRAYS_FOLDER"/compressed"
PAYLOAD_PACKED_ARRAYS_FOLDER=$ORIG_ARRAYS_FOLDER"/PACKED_"$PAYLOAD_PACK_PREFIX_NUM_BITS
PAYLOAD_PACKED_ARRAYS_COMPRESSED_FOLDER=$PAYLOAD_PACKED_ARRAYS_FOLDER"/compressed"
UPPERCASE_PAYLOAD_ARRAYS_FOLDER=$ORIG_ARRAYS_FOLDER"/UPPERCASE_payload"
UPPERCASE_PAYLOAD_ARRAYS_COMPRESSED_FOLDER=$UPPERCASE_PAYLOAD_ARRAYS_FOLDER"/compressed"
UPPERCASE_PAYLOAD_PACKED_ARRAYS_FOLDER=$UPPERCASE_PAYLOAD_ARRAYS_FOLDER"/PACKED_"$PAYLOAD_PACK_PREFIX_NUM_BITS
UPPERCASE_PAYLOAD_PACKED_ARRAYS_COMPRESSED_FOLDER=$UPPERCASE_PAYLOAD_PACKED_ARRAYS_FOLDER"/compressed"
CASE_BITSTRING_ARRAYS_FOLDER=$ORIG_ARRAYS_FOLDER"/CASE_bitstring"
CASE_BITSTRING_ARRAYS_COMPRESSED_FOLDER=$CASE_BITSTRING_ARRAYS_FOLDER"/compressed"
CASE_BITSTRING_PACKED_ARRAYS_FOLDER=$CASE_BITSTRING_ARRAYS_FOLDER"/PACKED_"$BITSTRING_PACK_PREFIX_NUM_BITS
CASE_BITSTRING_PACKED_ARRAYS_COMPRESSED_FOLDER=$CASE_BITSTRING_PACKED_ARRAYS_FOLDER"/compressed"
mkdir -p $ORIG_ARRAYS_FOLDER
mkdir -p $ORIG_ARRAYS_COMPRESSED_FILES_FOLDER
if [[ $PAYLOAD_PACK_PREFIX_NUM_BITS -gt 0 && $MAKE_UPPERCASE_AND_CASE_BITSTRING = false ]]; then
	mkdir -p $PAYLOAD_PACKED_ARRAYS_FOLDER
	mkdir -p $PAYLOAD_PACKED_ARRAYS_COMPRESSED_FOLDER
fi
if [ $MAKE_UPPERCASE_AND_CASE_BITSTRING = true ]; then
	if [ $CONSTRUCT_PAYLOAD_PART = true ]; then
		mkdir -p $UPPERCASE_PAYLOAD_ARRAYS_FOLDER
		mkdir -p $UPPERCASE_PAYLOAD_ARRAYS_COMPRESSED_FOLDER
		if [ $PAYLOAD_PACK_PREFIX_NUM_BITS -gt 0 ]; then
			mkdir -p $UPPERCASE_PAYLOAD_PACKED_ARRAYS_FOLDER
			mkdir -p $UPPERCASE_PAYLOAD_PACKED_ARRAYS_COMPRESSED_FOLDER
		fi
	fi
	if [ $CONSTRUCT_BITSTRING_PART = true ]; then
		mkdir -p $CASE_BITSTRING_ARRAYS_FOLDER
		mkdir -p $CASE_BITSTRING_ARRAYS_COMPRESSED_FOLDER
		if [ $BITSTRING_PACK_PREFIX_NUM_BITS -gt 0 ]; then
			mkdir -p $CASE_BITSTRING_PACKED_ARRAYS_FOLDER
			mkdir -p $CASE_BITSTRING_PACKED_ARRAYS_COMPRESSED_FOLDER
		fi
	fi
fi



#-----
# MAKE STAMP
time_rand_stamp="date"$(date --rfc-3339=ns | sed -e 's/ /_time/' | tr -d ":-" | sed -e "s/[.]/_rand/")$RANDOM
echo "TIME_RAND_STAMP=" $time_rand_stamp



#-----
# TOOLS TO PROCESS THE RAW CHROMOSOMES
JAVA_TEXT_UTILS_FOLDER=$PROGRAM_DIR"/java_text_utils"
RAW_STRING_TOOL="prepareText__raw"
UPPERCASE_TOOL="prepareText__UPPERCASE"
BITSTRING_CASE_TOOL="prepareText__represent_letter_case"
cd $JAVA_TEXT_UTILS_FOLDER
javac $RAW_STRING_TOOL".java"
javac $UPPERCASE_TOOL".java"
javac $BITSTRING_CASE_TOOL".java"
cd ../
PACK_BITSTRING_TOOL_C=$PROGRAM_DIR"/other/bitutils.c"
PACK_BITSTRING_TOOL_EXE=$PROGRAM_DIR"/other/bitutils.exe"
gcc $PACK_BITSTRING_TOOL_C -o $PACK_BITSTRING_TOOL_EXE -lm
#-----


#-----
# RAW STRING
processing1_error=0
REFERENCE_PLUS_TARGET=$ORIG_ARRAYS_FOLDER"/reference_plus_target.txt"
PROCESSED_TARGET=$ORIG_ARRAYS_FOLDER"/target.txt"
PROCESSED_REFERENCE=$ORIG_ARRAYS_FOLDER"/reference.txt"
if [ $construct_SA_LPF = true ]; then
	cd $JAVA_TEXT_UTILS_FOLDER
	echo "...Processing target!"
	java $RAW_STRING_TOOL $TARGET $PROCESSED_TARGET
	let processing1_error+=$?
	echo "...Processing reference!"
	java $RAW_STRING_TOOL $REFERENCE $PROCESSED_REFERENCE
	let processing1_error+=$?
	cd ../
	echo "...Concatenating reference and target!"
	cat $PROCESSED_REFERENCE $PROCESSED_TARGET > $REFERENCE_PLUS_TARGET
fi
# ERROR?
if [ $processing1_error -ne 0 ]; then
	echo "PROCESSING1 ERROR" $processing1_error
	exit $processing1_error
fi
#-----


#-----
# UPPERCASE PAYLOAD AND CASE BITSTRING
processing_error=0
REFERENCE_UPPERCASE=$UPPERCASE_PAYLOAD_ARRAYS_FOLDER"/reference.txt"
TARGET_UPPERCASE=$UPPERCASE_PAYLOAD_ARRAYS_FOLDER"/target.txt"
REFERENCE_PLUS_TARGET_UPPERCASE=$UPPERCASE_PAYLOAD_ARRAYS_FOLDER"/reference_plus_target.txt"
REFERENCE_CASE_BITSTRING=$CASE_BITSTRING_ARRAYS_FOLDER"/reference.txt"
TARGET_CASE_BITSTRING=$CASE_BITSTRING_ARRAYS_FOLDER"/target.txt"
REFERENCE_PLUS_TARGET_CASE_BITSTRING=$CASE_BITSTRING_ARRAYS_FOLDER"/reference_plus_target.txt"
if [[ $MAKE_UPPERCASE_AND_CASE_BITSTRING = true ]] && [[ $construct_SA_LPF = true ]]; then
	cd $JAVA_TEXT_UTILS_FOLDER
	echo "*** MAKE_UPPERCASE_AND_CASE_BITSTRING=true ***"
	if [ $CONSTRUCT_PAYLOAD_PART = true ]; then
		echo "...Processing uppercase reference!"
		java $UPPERCASE_TOOL $PROCESSED_REFERENCE $REFERENCE_UPPERCASE 0
		let processing_error+=$?
		echo "...Processing uppercase target!"
		java $UPPERCASE_TOOL $PROCESSED_TARGET $TARGET_UPPERCASE 0
		let processing_error+=$?
		echo "...Processing uppercase reference + target!"
		java $UPPERCASE_TOOL $REFERENCE_PLUS_TARGET $REFERENCE_PLUS_TARGET_UPPERCASE 0
		let processing_error+=$?
	fi
	if [ $CONSTRUCT_BITSTRING_PART = true ]; then
		echo "...Processing case-bitstring for reference!"
		java $BITSTRING_CASE_TOOL $PROCESSED_REFERENCE $REFERENCE_CASE_BITSTRING 0
		let processing_error+=$?
		echo "...Processing case-bitstring for target!"
		java $BITSTRING_CASE_TOOL $PROCESSED_TARGET $TARGET_CASE_BITSTRING 0
		let processing_error+=$?
		echo "...Processing case-bitstring for reference + target!"
		java $BITSTRING_CASE_TOOL $REFERENCE_PLUS_TARGET $REFERENCE_PLUS_TARGET_CASE_BITSTRING 0
		let processing_error+=$?
	fi
	cd ../
fi
# ERROR?
if [ $processing_error -ne 0 ]; then
	echo "PROCESSING ERROR" $processing_error
	exit $processing_error
fi
#-----


#-----
# PACKED STRINGS
packed_error=0
# ... UPPERCASE and BITSTRING
PACKED_REFERENCE_UPPERCASE=$UPPERCASE_PAYLOAD_PACKED_ARRAYS_FOLDER"/reference.txt"
PACKED_TARGET_UPPERCASE=$UPPERCASE_PAYLOAD_PACKED_ARRAYS_FOLDER"/target.txt"
PACKED_REFERENCE_PLUS_TARGET_UPPERCASE=$UPPERCASE_PAYLOAD_PACKED_ARRAYS_FOLDER"/reference_plus_target.txt"
PACKED_REFERENCE_CASE_BITSTRING=$CASE_BITSTRING_PACKED_ARRAYS_FOLDER/"reference.txt"
PACKED_TARGET_CASE_BITSTRING=$CASE_BITSTRING_PACKED_ARRAYS_FOLDER/"target.txt"
PACKED_REFERENCE_PLUS_TARGET_CASE_BITSTRING=$CASE_BITSTRING_PACKED_ARRAYS_FOLDER/"reference_plus_target.txt"
if [[ $MAKE_UPPERCASE_AND_CASE_BITSTRING = true ]] && [[ $construct_SA_LPF = true ]]; then
	if [[ $CONSTRUCT_BITSTRING_PART = true ]] && [[ $BITSTRING_PACK_PREFIX_NUM_BITS -gt 0 ]]; then
		printf "\n\n...Creating packed bitstring (reference)"
		$PACK_BITSTRING_TOOL_EXE $REFERENCE_CASE_BITSTRING $PACKED_REFERENCE_CASE_BITSTRING $BITSTRING_PACK_PREFIX_NUM_BITS
		let packed_error+=$?
		printf "\n\n...Creating packed bitstring (target)"
		$PACK_BITSTRING_TOOL_EXE $TARGET_CASE_BITSTRING $PACKED_TARGET_CASE_BITSTRING $BITSTRING_PACK_PREFIX_NUM_BITS
		let packed_error+=$?
		printf "\n\n...Concatenating packed reference and packed target bitstrings"
		cat $PACKED_REFERENCE_CASE_BITSTRING $PACKED_TARGET_CASE_BITSTRING > $PACKED_REFERENCE_PLUS_TARGET_CASE_BITSTRING
	fi
	if [[ $CONSTRUCT_PAYLOAD_PART = true ]] && [[ $PAYLOAD_PACK_PREFIX_NUM_BITS -gt 0 ]]; then
		printf "\n\n...Creating packed UPPERCASE payload (reference)"
		$PACK_BITSTRING_TOOL_EXE $REFERENCE_UPPERCASE $PACKED_REFERENCE_UPPERCASE $PAYLOAD_PACK_PREFIX_NUM_BITS
		let packed_error+=$?
		printf "\n\n...Creating packed UPPERCASE payload (target)"
		$PACK_BITSTRING_TOOL_EXE $TARGET_UPPERCASE $PACKED_TARGET_UPPERCASE $PAYLOAD_PACK_PREFIX_NUM_BITS
		let packed_error+=$?
		printf "\n\n...Concatenating packed UPPERCASE reference and packed UPPERCASE target payloads"
		cat $PACKED_REFERENCE_UPPERCASE $PACKED_TARGET_UPPERCASE > $PACKED_REFERENCE_PLUS_TARGET_UPPERCASE
	fi
fi
# ... PAYLOAD
PACKED_TARGET=$PAYLOAD_PACKED_ARRAYS_FOLDER"/target.txt"
PACKED_REFERENCE=$PAYLOAD_PACKED_ARRAYS_FOLDER"/reference.txt"
PACKED_REFERENCE_PLUS_TARGET=$PAYLOAD_PACKED_ARRAYS_FOLDER"/reference_plus_target.txt"
if [[ $PAYLOAD_PACK_PREFIX_NUM_BITS -gt 0 ]] && [[ $MAKE_UPPERCASE_AND_CASE_BITSTRING = false ]] && [[ $construct_SA_LPF = true ]]; then
	printf "\n\n...Creating packed payload (reference)"
	$PACK_BITSTRING_TOOL_EXE $PROCESSED_REFERENCE $PACKED_REFERENCE $PAYLOAD_PACK_PREFIX_NUM_BITS
	let packed_error+=$?
	printf "\n\n...Creating packed payload (target)"
	$PACK_BITSTRING_TOOL_EXE $PROCESSED_TARGET $PACKED_TARGET $PAYLOAD_PACK_PREFIX_NUM_BITS
	let packed_error+=$?
	printf "\n\n...Concatenating packed reference and packed target payloads"
	cat $PACKED_REFERENCE $PACKED_TARGET > $PACKED_REFERENCE_PLUS_TARGET
fi
# ERROR?
if [ $packed_error -ne 0 ]; then
	echo "PACKED ERROR" $packed_error
	exit $packed_error
fi
#-----


#-----
# SA/LPF CONSTRUCTION
if [ $construct_SA_LPF = true ]; then
	chmod u+x mkrun_helper_SALPF.sh 
	if [ $MAKE_UPPERCASE_AND_CASE_BITSTRING = false ]; then
		if [ $PAYLOAD_PACK_PREFIX_NUM_BITS -eq 0 ]; then
			printf "... Making SA/LPF for RAW text\n\n"
			./mkrun_helper_SALPF.sh "char" "$REFERENCE_PLUS_TARGET" "$ORIG_ARRAYS_FOLDER"
		else
			printf "... Making SA/LPF for PACKED RAW text\n\n"
			./mkrun_helper_SALPF.sh "int" "$PACKED_REFERENCE_PLUS_TARGET" "$PAYLOAD_PACKED_ARRAYS_FOLDER"
		fi
	else
		if [ $CONSTRUCT_PAYLOAD_PART = true ]; then
			if [ $PAYLOAD_PACK_PREFIX_NUM_BITS -eq 0 ]; then
				printf "... Making SA/LPF for UPPERCASE PAYLOAD\n\n"
				./mkrun_helper_SALPF.sh "char" "$REFERENCE_PLUS_TARGET_UPPERCASE" "$UPPERCASE_PAYLOAD_ARRAYS_FOLDER"
			else
				printf "... Making SA/LPF for PACKED UPPERCASE PAYLOAD\n\n"
				./mkrun_helper_SALPF.sh "int" "$PACKED_REFERENCE_PLUS_TARGET_UPPERCASE" "$UPPERCASE_PAYLOAD_PACKED_ARRAYS_FOLDER"
			fi
		fi
		if [ $CONSTRUCT_BITSTRING_PART = true ]; then
			if [ $BITSTRING_PACK_PREFIX_NUM_BITS -eq 0 ]; then
				printf "... Making SA/LPF for CASE BITSTRING\n\n"
				./mkrun_helper_SALPF.sh "char" "$REFERENCE_PLUS_TARGET_CASE_BITSTRING" "$CASE_BITSTRING_ARRAYS_FOLDER" 
			else
				printf "... Making SA/LPF for PACKED CASE BITSTRING\n\n"
				./mkrun_helper_SALPF.sh "int" "$PACKED_REFERENCE_PLUS_TARGET_CASE_BITSTRING" "$CASE_BITSTRING_PACKED_ARRAYS_FOLDER" 
			fi
		fi
	fi
fi
#-----



#-----
# COMPRESSION/DECOMPRESSION
comprdecompr_error=0
if [ $run_compress_decompress = true ]; then

	chmod u+x mkrun_helper_COMPRDECOMPR.sh
	if [ $MAKE_UPPERCASE_AND_CASE_BITSTRING = false ]; then
		if [ $PAYLOAD_PACK_PREFIX_NUM_BITS -eq 0 ]; then
			printf "... Compressing/Decompressing the RAW text\n\n"
			./mkrun_helper_COMPRDECOMPR.sh $PROCESSED_TARGET $PROCESSED_REFERENCE $REFERENCE_PLUS_TARGET $ORIG_ARRAYS_FOLDER $K_LOW $K_HIGH $K_INCREMENT $SCHEME "char" "__RAW_text_wo_newlines_ws" $time_rand_stamp $ORIG_ARRAYS_COMPRESSED_FILES_FOLDER
		else
			printf "... Compressing/Decompressing the PACKED RAW text\n\n"
			./mkrun_helper_COMPRDECOMPR.sh $PACKED_TARGET $PACKED_REFERENCE $PACKED_REFERENCE_PLUS_TARGET $PAYLOAD_PACKED_ARRAYS_FOLDER $K_LOW $K_HIGH $K_INCREMENT $SCHEME "int" "__RAW_text_wo_newlines_ws__PACKED_"$PAYLOAD_PACK_PREFIX_NUM_BITS"_bits_in_int" $time_rand_stamp $PAYLOAD_PACKED_ARRAYS_COMPRESSED_FOLDER
		fi

		comprdecompr_error=$?
		if [ $comprdecompr_error -ne 0 ]; then
			echo "Compression/Decompression ERROR"
			exit $comprdecompr_error
		fi
	else
		if [ $CONSTRUCT_PAYLOAD_PART = true ]; then
			if [ $PAYLOAD_PACK_PREFIX_NUM_BITS -eq 0 ]; then
				printf "... Compressing/Decompressing the UPPERCASE PAYLOAD\n\n"
				./mkrun_helper_COMPRDECOMPR.sh $TARGET_UPPERCASE $REFERENCE_UPPERCASE $REFERENCE_PLUS_TARGET_UPPERCASE $UPPERCASE_PAYLOAD_ARRAYS_FOLDER $K_LOW $K_HIGH $K_INCREMENT $SCHEME "char" "__UPPERCASE_payload" $time_rand_stamp $UPPERCASE_PAYLOAD_ARRAYS_COMPRESSED_FOLDER
			else
				printf "... Compressing/Decompressing the PACKED UPPERCASE PAYLOAD\n\n"
				./mkrun_helper_COMPRDECOMPR.sh $PACKED_TARGET_UPPERCASE $PACKED_REFERENCE_UPPERCASE $PACKED_REFERENCE_PLUS_TARGET_UPPERCASE $UPPERCASE_PAYLOAD_PACKED_ARRAYS_FOLDER $K_LOW $K_HIGH $K_INCREMENT $SCHEME "int" "__UPPERCASE_payload_PACKED_"$PAYLOAD_PACK_PREFIX_NUM_BITS"_bits_in_int" $time_rand_stamp $UPPERCASE_PAYLOAD_PACKED_ARRAYS_COMPRESSED_FOLDER
			fi
			comprdecompr_error=$?
		fi		
		if [ $comprdecompr_error -ne 0 ]; then
			echo "Compression/Decompression ERROR"
			exit $comprdecompr_error
		fi
		if [ $CONSTRUCT_BITSTRING_PART = true ]; then
			if [ $BITSTRING_PACK_PREFIX_NUM_BITS -eq 0 ]; then
				printf "... Compressing/Decompressing the CASE BITSTRING\n\n"
				./mkrun_helper_COMPRDECOMPR.sh $TARGET_CASE_BITSTRING $REFERENCE_CASE_BITSTRING $REFERENCE_PLUS_TARGET_CASE_BITSTRING $CASE_BITSTRING_ARRAYS_FOLDER $K_LOW_BITSTRING $K_HIGH_BITSTRING $K_INCREMENT_BITSTRING $SCHEME_BITSTRING "char" "__ASCII_symbol_case_bitstring" $time_rand_stamp $CASE_BITSTRING_ARRAYS_COMPRESSED_FOLDER
			else
				printf "... Compressing/Decompressing the PACKED CASE BITSTRING\n\n"
				./mkrun_helper_COMPRDECOMPR.sh $PACKED_TARGET_CASE_BITSTRING $PACKED_REFERENCE_CASE_BITSTRING $PACKED_REFERENCE_PLUS_TARGET_CASE_BITSTRING $CASE_BITSTRING_PACKED_ARRAYS_FOLDER $K_LOW_BITSTRING $K_HIGH_BITSTRING $K_INCREMENT_BITSTRING $SCHEME_BITSTRING "int" "__symbol_case_bitstring_PACKED_"$BITSTRING_PACK_PREFIX_NUM_BITS"_bits_in_int" $time_rand_stamp $CASE_BITSTRING_PACKED_ARRAYS_COMPRESSED_FOLDER
			fi
			comprdecompr_error=$?
		fi
		if [ $comprdecompr_error -ne 0 ]; then
			echo "Compression/Decompression ERROR"
			exit $comprdecompr_error
		fi
	fi
fi
#-----

