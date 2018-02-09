#!/bin/bash
# mkrun_helper_SALPF $USE_SA_LPF_TYPE $REF_PLUS_TARGET $PATH_FOR_OUTPUT_DATA

USE_SA_LPF_TYPE=$1
REF_PLUS_TARGET=$2
PATH_FOR_OUTPUT_DATA=$3

ref_plus_target_len=$(wc -c < "$REF_PLUS_TARGET")
REF_PLUS_TARGET_NAME=="$(basename $REF_PLUS_TARGET)"

printf "\n\n\n****** mkrun_helper_SALPF.sh ******\n\n"
echo "USE_SA_LPF_TYPE:" $USE_SA_LPF_TYPE
echo "REF_PLUS_TARGET:" $REF_PLUS_TARGET
echo "MOVE_ARRAYS_TO_FOLDER:" $MOVE_ARRAYS_TO_FOLDER
printf "\n\n\n"


echo "... Running SA/LPF Construction ..."

cd str_suite/

error=0

if [ $USE_SA_LPF_TYPE = "char" ]; then
	printf "\n...Using **CHAR** SA / LPF\n"
	chmod u+x mkrun_charSALPF.sh
	./mkrun_charSALPF.sh "$REF_PLUS_TARGET" $ref_plus_target_len "" $PATH_FOR_OUTPUT_DATA
	error=$?
elif [ $USE_SA_LPF_TYPE = "int" ]; then
	printf "\n...Using **INT** SA / LPF\n"
	chmod u+x mkrun_intSALPF.sh
	./mkrun_intSALPF.sh "$REF_PLUS_TARGET" $ref_plus_target_len "" $PATH_FOR_OUTPUT_DATA
	error=$?
fi

cd ../

exit $error



