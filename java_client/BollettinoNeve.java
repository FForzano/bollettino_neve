// java implementation of client

import java.io.*;
import java.net.*;

public class BollettinoNeve {
    public static void main (String args[]){
        // main parameter control
        if (args.length != 2){
            System.out.println("Usage: java BollettinoNeve host port");
            System.exit(-1);
        }
        try {
            int portNum = Integer.parseInt(args[1]);
            if (portNum > 65535) new NumberFormatException("Invalid port");
        } catch (NumberFormatException e){
            System.out.println ("port parameter have to be a number of a valid service port\n");
            System.out.println (e.getMessage());
            System.exit (-1);
        }

        try{
            BufferedReader usrIn = new BufferedReader (new InputStreamReader(System.in));
            Socket s = new Socket (args[0], Integer.parseInt(args[1]));
            CSExpReader netIn = new CSExpReader(s.getInputStream());
            CSExpWriter netOut = new CSExpWriter(s.getOutputStream());

            // User interface
            String info[]= new String[2];
            System.out.println ("Inserire la regione di interesse:");
            info[0]=usrIn.readLine();
            while (!info[0].equals("fine")){
                System.out.println ("Inserire numero di località da visualizzare:");
                info[1]=usrIn.readLine();

                netOut.writemex(info);
                netOut.flush();

                // print all return messages
                String[] messages = netIn.readmex();
                System.out.println (messages[0] + "media: " + messages[1] + " cm");

                // New user request
                System.out.println ("\nInserire la regione di interesse");
                info[0]=usrIn.readLine();
            }
            
            s.close();
        } catch (Exception e){
            System.out.println (e.getMessage());
            e.getStackTrace();
            System.exit(-1);
        }
    }  
}
