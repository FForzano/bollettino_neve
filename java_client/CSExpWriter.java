import java.io.*;

public class CSExpWriter {
    private OutputStream output;

    public CSExpWriter (OutputStream output){
        this.output = output;
    }

    // write on the output stream one message in Canonical S-Expression format
    public void writemex(String[] strings) throws Exception{
        String outBuff="";
        
        outBuff = outBuff + "(";
        
        int i;
        for (i=0; i<strings.length; i++){
            outBuff = outBuff + strings[i].length() + ":" + strings[i];;
        }

        outBuff = outBuff + ")";
        output.write (outBuff.getBytes("UTF-8"));
    }

    public void flush() throws IOException{
        output.flush();
    }
}
