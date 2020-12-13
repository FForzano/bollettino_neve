import java.io.*;

public class LenPrefReader {
    private InputStream socketIn;
    private static final int lenDigit = 6;

    public LenPrefReader (InputStream input){
        this.socketIn = input;
    }

    public String[] readmex () throws IOException{
        byte buff[] = new byte[lenDigit];
        String output[] = new String[0];
        int Nout=0;
        boolean endFlag = false;

        while(!endFlag){
            // Length read
            int i;

            if (socketIn.read(buff) == 0) endFlag=true;

            if(!endFlag){
                String lenmex = new String (buff, "UTF-8"); // length string

                byte info[] = new byte[Integer.parseInt(lenmex)];
                socketIn.read (info,0,info.length);

                Nout++;
                String[] output1 = new String[Nout];
                for (i=0; i < output.length; i++){
                    output1[i]=output[i];
                }
                output1[i] = new String (info, "UTF-8");
                output=output1;
                endFlag=true; // 1 sola lettura... bisogna trovare condizione di fine
            }
        }

        return output;
    }
}
