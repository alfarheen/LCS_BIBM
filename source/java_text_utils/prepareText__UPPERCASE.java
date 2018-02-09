import java.io.*;
import java.util.*;

// param1: input filename
// param2: output UPPERCASE filename
// param3: will omit first line
public class prepareText__UPPERCASE {
  public static void main(String[] args) throws Exception{
    Scanner f = new Scanner(new FileReader(args[0]));
    PrintWriter p = new PrintWriter(args[1]);
    int will_omit_first_line = new Integer(args[2]);
    if(will_omit_first_line != 0) f.nextLine();
    while(f.hasNext()){
      String curr = f.next().trim().toUpperCase();
      if(curr!=null && curr.length()>0) p.write(curr);
    }
    f.close();
    p.close();
  }
}
