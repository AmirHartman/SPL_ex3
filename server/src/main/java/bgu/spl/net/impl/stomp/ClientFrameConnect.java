package bgu.spl.net.impl.stomp;

import bgu.spl.net.impl.stomp.ServerFrame.ServerFrame;
import bgu.spl.net.impl.stomp.ServerFrame.ServerFrameConnected;
import bgu.spl.net.impl.stomp.ServerFrame.ServerFrameError;
import bgu.spl.net.srv.Connections;
// import bgu.spl.net.impl.stomp.StompMessagingProtocolImpl;


public class ClientFrameConnect extends ClientFrame {
    private String acceptVersion = "1.2";
    private String host = "stomp.cs.bgu.ac.il";
    private String username;
    private String passcode;

    public ClientFrameConnect(String username, String passcode){
        super(StompCommand.CONNECT);
        this.username = username;
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
                    this.username = header[1];
                    break;
                case "passcode":
                    this.passcode = header[1];  
                    break;
            }}}

    public String getUsername(){
        return this.username;
    }

    public String getPasscode(){
        return this.passcode;
    }

    @Override
    public ServerFrame process (int connectionId, Connections <String> connections, StompMessagingProtocolImpl protocol){
        if (!connections.correctPassword(username, passcode)){
            return new ServerFrameError("Wrong Password", receiptId, toString());
        }
        if (!connections.connect(connectionId, username, passcode)){
            return new ServerFrameError("User already logged in", receiptId, toString());
        }
        return new ServerFrameConnected(receiptId);
    }


    public String toString (){
        return "CONNECT\n" +
                "accept-version:" + acceptVersion + "\n" +
                "host:" + host + "\n" +
                "login:" + username + "\n" +
                "passcode:" + passcode + "\n" +
                this.body;
        }
}

