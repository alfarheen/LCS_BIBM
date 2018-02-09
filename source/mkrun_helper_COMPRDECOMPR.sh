#!/bin/bash
# mkrun_helper_COMPRDECOMPR $TARGET $REF $REF_PLUS_TARGET $FOLDER_WITH_ARRAYS $K_LOW $K_HIGH $K_INCREMENT $SCHEME $FILENAME_TYPE $DESCRIPTION $TIME_RAND_STAMP $OUTPUT_COMPRESSED_FILES_FOLDER

TARGET=$1
REF=$2
REF_PLUS_TARGET=$3
FOLDER_WITH_ARRAYS=$4
K_LOW=$5
K_HIGH=$6
K_INCREMENT=$7
SCHEME=$8
FILENAME_TYPE=$9
DESCRIPTION=${10}
TIME_RANDOM_STAMP=${11}
OUTPUT_COMPRESSED_FILES_FOLDER=${12}

LPF=$FOLDER_WITH_ARRAYS"/lpf.bin"
DECIDING_ARR=$FOLDER_WITH_ARRAYS"/deciding_arr.bin"

target_len=$(wc -c < "$TARGET")
ref_len=$(wc -c < "$REF")
ref_plus_target_len=$(wc -c < "$REF_PLUS_TARGET")
BYTES_IN_INT=4

if [ $FILENAME_TYPE = "int" ]; then
	target_len=$((target_len/BYTES_IN_INT))
	ref_len=$((ref_len/BYTES_IN_INT))
	ref_plus_target_len=$((ref_plus_target_len/BYTES_IN_INT))
fi

printf "\n\n\n****** mkrun_helper_COMPRDECOMPR.sh ******\n\n"
echo "TARGET:          " $TARGET ", length:" $target_len
echo "REF:             " $REF ", length:" $ref_len
echo "REF_PLUS_TARGET: " $REF_PLUS_TARGET ", length:" $ref_plus_target_len
echo "LPF:             " $LPF
echo "DECIDING_ARR:    " $DECIDING_ARR
printf "\n\n\n"

echo "... Running Compression / Decompression ..."

LCS_BIBM_COMPRESSION_TOOL="LCS_BIBM_compress__LIGHT"

rm -f $LCS_BIBM_COMPRESSION_TOOL".exe"
g++ $LCS_BIBM_COMPRESSION_TOOL".c" -o $LCS_BIBM_COMPRESSION_TOOL".exe"

if [ $? -ne 0 ]; then
	exit $?
fi

"./"$LCS_BIBM_COMPRESSION_TOOL".exe" $TARGET $target_len $REF $ref_len $REF_PLUS_TARGET $ref_plus_target_len $LPF $DECIDING_ARR $K_LOW $K_HIGH $K_INCREMENT $SCHEME $FILENAME_TYPE $DESCRIPTION $TIME_RANDOM_STAMP $OUTPUT_COMPRESSED_FILES_FOLDER

exit $?


