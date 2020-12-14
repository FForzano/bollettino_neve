import java.io.*;
import java.util.zip.DataFormatException;

public class CSExpReader {
    private InputStream input;

    public CSExpReader (InputStream socketIn){
        this.input = socketIn;
    }

    // read from input stream one message in Canonical S-Expression format
    public String[] readmex() throws Exception{
        byte buff[] = new byte[8];
        String output[] = new String[0];
        int Nout=0;
        boolean endFlag = false;

        // starter char '(' read
        input.read(buff, 0, 1);
        if (buff[0] != (byte)'('){
            new DataFormatException ("Invalid format for message");
        }

        while(!endFlag){
            // Length read
            int i=0;
            do{
                input.read(buff, i, 1);

                if ((char) buff[i] == ')') endFlag=true; // end of message
                i++;
            } while ((char) buff[i-1] != ':' && !endFlag);

            if(!endFlag){
                String lenmex = new String (buff, 0, i-1, "UTF-8"); // length string

                byte info[] = new byte[Integer.parseInt(lenmex)];
                input.read (info,0,info.length);

                Nout++;
                String[] output1 = new String[Nout];
                for (i=0; i< output.length; i++){
                    output1[i]=output[i];
                }
                output1[i] = new String (info, "UTF-8");
                output=output1;
            }
        }

        return output;
    }
}
