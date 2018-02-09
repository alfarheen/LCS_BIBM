import java.io.*;
import java.util.*;

// param1: input filename
// param2: output filename
public class prepareText {
  public static void main(String[] args) throws Exception{
    Scanner f = new Scanner(new FileReader(args[1]));
    PrintWriter p = new PrintWriter(args[2]);
    f.nextLine();
    while(f.hasNextLine()){
      String curr = f.nextLine().trim().toUpperCase();
      if(curr!=null && curr.length()>0) p.write(curr);
    }
    f.close();
    p.close();
  }
}