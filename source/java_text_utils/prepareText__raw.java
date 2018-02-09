import java.io.*;
import java.util.*;

// param1: input filename
// param2: output filename
public class prepareText__raw {
  public static void main(String[] args) throws Exception{
    Scanner f = new Scanner(new FileReader(args[0]));
    PrintWriter p = new PrintWriter(args[1]);
    f.nextLine();
    while(f.hasNext()){
      String curr = f.next().trim();
      if(curr!=null && curr.length()>0) { p.write(curr); p.flush();} 
    }
    f.close();
    p.close();
  }
}
