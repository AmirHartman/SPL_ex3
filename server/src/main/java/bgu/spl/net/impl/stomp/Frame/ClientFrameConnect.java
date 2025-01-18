package bgu.spl.net.impl.stomp.Frame;

import bgu.spl.net.srv.Connections;

public class ClientFrameConnect extends ClientFrame {
    private String acceptVersion = "1.2";
    private String host = "stomp.cs.bgu.ac.il";
    private String login;
    private String passcode;


    public ClientFrameConnect(String login, String passcode) {
        super(ServiceStompCommand.CONNECT);
        this.login = login;
        this.passcode = passcode;
    }

    public ClientFrameConnect(String toFrame){
        super(toFrame);
        String[] lines = toFrame.split("\n");
        // initialize headers
        for (int i = 1; i < lines.length; i++){
            String[] header = lines[i].split(":");
            switch (header[0]){
                case "login":
                    this.login = header[1];
                    break;
                case "passcode":
                    this.passcode = header[1];  
                    break;
            }
        }
    }


    // @Override
    // public void process (String string, Connections <String> connections){
    // }

    public String toString (){
        return "CONNECT\n" +
                "accept-version:" + acceptVersion + "\n" +
                "host:" + host + "\n" +
                "login:" + login + "\n" +
                "passcode:" + passcode + "\n" +
                this.body;
        }
}

