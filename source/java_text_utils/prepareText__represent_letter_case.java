import java.io.*;
import java.util.*;

// param1: input filename
// param2: output filename
// param3: will omit first line
public class prepareText__represent_letter_case {
  public static void main(String[] args) throws Exception{
    Scanner f = new Scanner(new FileReader(args[0]));
    PrintWriter p = new PrintWriter(args[1]);
    int will_omit_first_line = new Integer(args[2]);
    if(will_omit_first_line != 0) f.nextLine();
    while(f.hasNext()){
      String curr = f.next().trim();
      for(int k=0; k<curr.length(); k++){
         if(Character.isUpperCase(curr.charAt(k))){
            p.write("1");
         }else{
            p.write("0");
         }
       }
    }
    f.close();
    p.close();
  }
}
