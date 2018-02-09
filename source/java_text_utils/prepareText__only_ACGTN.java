import java.io.*;
import java.util.*;

// param1: input filename
// param2: output filename
public class prepareText__only_ACGTN {
  public static void main(String[] args) throws Exception{
    Scanner f = new Scanner(new FileReader(args[0]));
    PrintWriter p = new PrintWriter(args[1]);
    f.nextLine();
    while(f.hasNext()){
      String curr = f.next().trim();
      for(int k=0; k<curr.length(); k++){
         char ch = Character.toUpperCase(curr.charAt(k));
         if(ch=='A' || ch=='C' || ch=='G' || ch=='T' || ch=='N'){
            p.write(ch);
         }else{
            p.write("N");
         }
       }
    }
    f.close();
    p.close();
  }
}
