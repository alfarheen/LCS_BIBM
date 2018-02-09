import java.io.*;
import java.util.*;

// param1: input filename
public class prepareText_and_count {
  public static void main(String[] args) throws Exception{
    for(String a: args) System.out.println(a);
    String outFn = "outfn.txt";
    PrintWriter pout = new PrintWriter(outFn);

//    String[] mid = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","M","X","Y"};
//    String pref = "/home/droz/Downloads/KOREF_20090224/from_web/chromosome_";
//    String suf = ".fa";
//    String[] mid = {"1","2","3","4","5"};
//    String pref = "/home/alf/Downloads/TAIR8/TAIR8_chr";
//    String suf = ".fas";
    String[] mid = {"1","2","3","4","5","6","7","8","9","10","11","12"};
    String pref = "/home/alf/Downloads/oryza_sativa_TIGR5/Oryza_sativa.MSU6.dna.chromosome.";
    String suf = ".fa";

    final int START_AT_CHAR = 32, END_AT_CHAR = 126;
    // header
	pout.print("Chromosome\t");
	pout.flush();
        for(char mm = START_AT_CHAR; mm<=END_AT_CHAR; mm++){
		if(mm == '\t' || mm == '\r' || mm == '\n') pout.print("WHITESPACE\t");
                else if(mm == ' ') pout.print("SPACE\t");
                else if(mm == '"') pout.print("DOUBLEQUOTE\t");
                else if(mm == '\'') pout.print("QUOTE\t");
                else pout.print(mm+"\t");
        }
       pout.println("upper_count\tlower_count\tactual_case_overlap_with_best_case\tbest_case_is");
       pout.flush();
    for(String mm : mid){
            String fn = pref + mm + suf;
            pout.print(fn+"\t");
            System.out.println(fn+"\t");
            System.out.flush();
	    Scanner f = new Scanner(new FileReader(fn));
	    f.nextLine();
	    int count[] = new int[256];
	    while(f.hasNext()){
	      String curr = f.next().trim();
	      for(int iii=0; iii<curr.length(); iii++) count[curr.charAt(iii)]++;
	    }
	    for(int ii=START_AT_CHAR; ii<=END_AT_CHAR; ii++){
	       //if(count[ii]>0)
		   pout.print(count[ii]+"\t");
	    }
	    int upper = 0, lower = 0;
	    for(int ii=0; ii<count.length; ii++){
	       if(count[ii]>0){
		 if(Character.isUpperCase((char) ii)) upper+=count[ii];
		 else if(Character.isLowerCase((char) ii)) lower+=count[ii]; 
	       }
	    }
	    int case_overlap1 = 0, case_overlap2 = 0;
	    for(char ii='a'; ii<='z'; ii++){
	       if(count[ii]>0 && count[Character.toUpperCase(ii)]>0) {
		   case_overlap1 += count[ii];
		   case_overlap2 += count[Character.toUpperCase(ii)];
	       }
	    }
	    int actual_case_overlap = Math.min(case_overlap1,case_overlap2);
	    String best_case_is = (actual_case_overlap == case_overlap1) ? "lowercase" : "UPPERCASE";
	    pout.println(upper+"\t"+lower+"\t"+actual_case_overlap+"\t"+best_case_is);
            pout.flush();
	    f.close();
    }
    pout.close();
  }
}



/*

import java.io.*;
import java.util.*;

// param1: input filename
public class prepareText_and_count {
  public static void main(String[] args) throws Exception{
    for(String a: args) System.out.println(a);
    String[] mid = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","M","X","Y"};
    String pref = "/home/droz/Downloads/KOREF_20090224/from_web/chromosome_";
    String suf = ".fa";
    for(String mm : mid){
            String fn = pref + mm + suf;
            System.out.println("\n\n\n"+fn+"\n");
	    Scanner f = new Scanner(new FileReader(fn));
	    f.nextLine();
	    int count[] = new int[256];
	    while(f.hasNext()){
	      String curr = f.next().trim();
	      for(int iii=0; iii<curr.length(); iii++) count[curr.charAt(iii)]++;
	    }
	    for(int ii=0; ii<count.length; ii++){
	       if(count[ii]>0)
		   System.out.println((char) ii + " = " + count[ii]);
	    }
	    int upper = 0, lower = 0;
	    for(int ii=0; ii<count.length; ii++){
	       if(count[ii]>0){
		 if(Character.isUpperCase((char) ii)) upper+=count[ii];
		 else if(Character.isLowerCase((char) ii)) lower+=count[ii]; 
	       }
	    }
	    int case_overlap1 = 0, case_overlap2 = 0;
	    for(char ii='a'; ii<='z'; ii++){
	       if(count[ii]>0 && count[Character.toUpperCase(ii)]>0) {
		   case_overlap1 += count[ii];
		   case_overlap2 += count[Character.toUpperCase(ii)];
	       }
	    }
	    int actual_case_overlap = Math.min(case_overlap1,case_overlap2);
	    System.out.println("upper = "+upper);
	    System.out.println("lower = "+lower);
	    System.out.println("actual_case_overlap_with_best_case = "+actual_case_overlap);
	    String best_case_is = (actual_case_overlap == case_overlap1) ? "lowercase" : "UPPERCASE";
	    System.out.println("best_case_is = "+best_case_is);

	    f.close();
    }
  }
}

*/

/*

import java.io.*;
import java.util.*;

// param1: input filename
public class prepareText_and_count {
  public static void main(String[] args) throws Exception{
    for(String a: args) System.out.println(a);
    Scanner f = new Scanner(new FileReader(args[0]));
    f.nextLine();
    int count[] = new int[256];
    while(f.hasNext()){
      String curr = f.next().trim();
      for(int iii=0; iii<curr.length(); iii++) count[curr.charAt(iii)]++;
    }
    for(int ii=0; ii<count.length; ii++){
       if(count[ii]>0)
           System.out.println((char) ii + " = " + count[ii]);
    }
    int upper = 0, lower = 0;
    for(int ii=0; ii<count.length; ii++){
       if(count[ii]>0){
         if(Character.isUpperCase((char) ii)) upper+=count[ii];
         else if(Character.isLowerCase((char) ii)) lower+=count[ii]; 
       }
    }
    int case_overlap1 = 0, case_overlap2 = 0;
    for(char ii='a'; ii<='z'; ii++){
       if(count[ii]>0 && count[Character.toUpperCase(ii)]>0) {
           case_overlap1 += count[ii];
           case_overlap2 += count[Character.toUpperCase(ii)];
       }
    }
    int actual_case_overlap = Math.min(case_overlap1,case_overlap2);
    System.out.println("upper = "+upper);
    System.out.println("lower = "+lower);
    System.out.println("actual_case_overlap_with_best_case = "+actual_case_overlap);
    String best_case_is = (actual_case_overlap == case_overlap1) ? "lowercase" : "UPPERCASE";
    System.out.println("best_case_is = "+best_case_is);

    f.close();
  }
}

*/
