#!/bin/bash

rm -f suite_light.exe
g++ suite_light.c -o suite_light.exe;

if [ $? -ne 0 ]; then
	exit $?
fi

# BEGIN execution to compute LPF
./suite_light.exe 22 $1 "" $2 "n" 3 "n" "int" $4 #SA construction
error=$?
./suite_light.exe 21 $1 "" $2 "n" 3 "n" "int" $4 #LPF construction
let error+=$?
# END execution

exit $error 

