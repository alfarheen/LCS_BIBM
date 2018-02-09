To compile and run this program in a UNIX or Cygwin environment, follow these steps.  



(1) For EACH target/reference, you will need to compute the SA/LPF/DECISION_ARR once (by following this step).  

--- (a) In mk.sh, set construct_SA_LPF=1 and run_compress_decompress=0.  

--- (b) In mk.sh, modify FOLDER to be the folder including the target and reference to compress.  Set TARGET to the filename of the target and REF to the filename of the reference.  

--- (c) In the command line, make this folder the current path (just so all relative path info is correct).  Then, type ./mk.sh (a slight variation of this call is needed for other UNIX command lines).  If the reference and target are big, this process can take some time.  The command line will give some updates.  

--- (d) If you plan to run more experiments on this target/reference, please save sa.txt/lpf.txt/decision_arr.txt (they are in arrays/) in a new area (and remember which target/reference you used) and use them later for step (2).  



(2) After you have the SA/LPF/DECISION_ARR (from step (1)) for the target and reference, you will run the compression routine as many times as needed.  The compression routine will compress the data with one of two schemes, with a set of K parameters.  The final compressed result is a pair of files (symbols_out and words_out).  After the result is made, the decompression routine will run just for fun and a flag will say if the compression was correct.  Follow the steps below.

--- (a) In mk.sh, set construct_SA_LPF=0 and run_compress_decompress=1.  Set FOLDER2 to the folder containing the sa.txt/lpf.txt/decision_arr.txt.

--- (b) In mk.sh, set SCHEME to 1 or 2 for SCHEME 1 or SCHEME 2, respectively (see BIBM paper).  

--- (c) One call to the compression program will run the program for the selected SCHEME and EACH K, where K\in{K_LOW,K_LOW+K_INCREMENT,K_LOW+2*K_INCREMENT,...,K_HIGH}.  So, in mk.sh, set K_LOW, K_HIGH, and K_INCREMENT.  

--- (d) In the command line, make this folder the current path (just so all relative path info is correct).  Then, type ./mk.sh (a slight variation of this call is needed for other UNIX command lines).  This may take a little time, but the command line will have updates.  Feel free to kill the program at any time, because after each individual experiment is complete, the results are placed in exp/exp.txt and the compressed files K_symbols_out and K_words_out are stored in exp/.


