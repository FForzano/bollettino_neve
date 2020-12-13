import java.io.*;

public class LenPrefWriter {
    private OutputStream bufferOut;

    public LenPrefWriter (OutputStream input){
        this.bufferOut = input;
    }
    
    public void writemex (String[] strings) throws IOException {
        String outBuff="";
        
        int i;
        for (i=0; i<strings.length; i++){
            int len = strings[i].length();

            if (len < 10){
                outBuff = outBuff + "00000" + len + strings[i];
            }
            else if (len < 100){
                outBuff = outBuff + "0000" + len + strings[i];
            }
            else if (len < 1000){
                outBuff = outBuff + "000" + len + strings[i];
            }
            else if (len < 10000){
                outBuff = outBuff + "00" + len + strings[i];
            }
            else if (len < 100000){
                outBuff = outBuff + "0" + len + strings[i];
            }
            else {
                outBuff = outBuff + len + strings[i];
            }
        }

        bufferOut.write (outBuff.getBytes("UTF-8"));
    }
}
