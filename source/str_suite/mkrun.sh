#!/bin/bash

#rm preprocess.exe
#g++ preprocess.c -o preprocess.exe

rm -f suite_light.exe
g++ suite_light.c -o suite_light.exe;

#echo $1 $2

# BEGIN execution to compute LPF
./suite_light.exe 19 $1 "" $2 "n" 3 "n" #SA construction
./suite_light.exe 21 $1 "" $2 "n" 3 "n" #LPF construction
# END execution 

# BEGIN execution to test CROCHEMORE LPF construction
#./suite.exe 19 texts/fibo/f30.txt "" 25 n 3 y #SA construction
#./suite.exe 20 texts/fibo/f30.txt "" 25 n 3 y #LPF construction
#./suite.exe 2 texts/fibo/f30.txt "" 25 n 3 y #pSA construction
#./suite.exe 5 texts/fibo/f30.txt "" 25 n 3 y #pLPF construction
# END execution

# BEGIN execution to test KASAI LCP construction
#./suite.exe 19 texts/fibo/f30.txt "" 11150 n 3 y #SA construction
#./suite.exe 18 texts/fibo/f30.txt "" 11150 n 3 y #LCP construction
#./suite.exe 2 texts/fibo/f30.txt "" 11150 n 3 y #pSA construction
#./suite.exe 10 texts/fibo/f30.txt "" 11150 n 3 y #pLCP construction
# END execution

# BEGIN execution to test PLF construction
#./suite.exe 14 texts/ecoli/ecoli.txt "acgt" 7500 n 3 y #pSA construction
#./suite.exe 15 texts/ecoli/ecoli.txt "acgt" 7500 n 3 y #pLF construction
# END execution

# BEGIN execution to test p-border construction
#./suite.exe 2 texts/ecoli/ecoli.txt "acgt" 500 n 3 y #pSA construction
#./suite.exe 16 texts/ecoli/ecoli.txt "acgt" 500 n 3 y #p-border construction
# END execution

# BEGIN execution to test PLRF construction
#./suite.exe 14 texts/ecoli/ecoli.txt "acgt" 5 n 3 y #pSA construction
#./suite.exe 13 texts/ecoli/ecoli.txt "acgt" 5 n 3 y #pLrF construction
# END execution

# BEGIN execution to test PLCP_JDA construction
#./suite.exe 2 texts/ecoli/ecoli.txt "acgt" 1000 n 3 y #pSA construction
#./suite.exe 8 texts/ecoli/ecoli.txt "acgt" 1000 #permuted-pLCP construction
# END execution

# BEGIN execution to test pLPF construction
#./suite.exe 2 texts/ecoli/ecoli.txt "acgt" 1000 n 3 y #pSA construction
#./suite.exe 5 texts/ecoli/ecoli.txt "acgt" 1000 #pLPF construction
# END execution

# BEGIN execution to test pLneF construction
#./suite.exe 2 texts/ecoli/ecoli.txt "acgt" 1000 n 3 y #pSA construction
#./suite.exe 12 texts/ecoli/ecoli.txt "acgt" 1000 #pLneF construction
# END execution

#
# OTHER
#
#./suite.exe 3 texts/examples/t3.txt AGCT #65536 n y
#./suite.exe 5 texts/ecoli/ecoli.txt "acgt" 1000 n 3 y #pLPF construction
#./suite.exe 4 texts/ecoli/ecoli.txt "" 1000 #traditional SA ssarray
#./suite.exe 2 texts/fibo/f25.txt ab 20000 y 18 y  #works w/ significant RECALC_PAC
#./suite.exe 3 ../pSA_code_Tomohiro_I_et_al/t12.txt a 0 y 1 y
#./suite.exe 2 "texts/random/random__1000000__N(32.00,16.00).txt" "*" 0 y 10 y
#./suite.exe 2 "texts/random/random__1000000__U(1,64).txt" "*" 10 y 3 y
#./suite.exe 0 "texts/random/random__1000000__U(1,64).txt" "-" 10 #plcp algorithm!!!
#./suite.exe 3 texts/bible/bible_final.txt ABCDEFGHIJKLMN 100000 y 0 y
#./suite.exe 3 texts/bible/bible_final.txt A 100000 n 0 y
