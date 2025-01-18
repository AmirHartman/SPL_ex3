package bgu.spl.net.impl.stomp.Frame;

import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;


public class ClientFrameConnect extends ClientFrame {
    private String acceptVersion = "1.2";
    private String host = "stomp.cs.bgu.ac.il";
    private String username;
    private String passcode;
    private int receiptId;

    public ClientFrameConnect(String username, String passcode, int receiptId){
        super(ServiceStompCommand.CONNECT);
        this.username = username;
        this.passcode = passcode;
        this.receiptId = receiptId;
    }


    public ClientFrameConnect(String toFrame){
        super(toFrame);
        String[] lines = toFrame.split("\n");
        // initialize headers
        for (int i = 1; i < lines.length; i++){
            String[] header = lines[i].split(":");
            switch (header[0]){
                case "username":
                    this.username = header[1];
                    break;
                case "passcode":
                    this.passcode = header[1];  
                    break;
                case "receipt":
                    try {
                        this.receiptId = Integer.parseInt(header[1]);
                    } catch (Exception e) {
                        System.out.println("unable to create frameConnect, invalid receipt id");
                    } 
                    break;
            }
        }
    }

    public String getUsername(){
        return this.username;
    }

    public String getPasscode(){
        return this.passcode;
    }

    @Override
    public ServiceFrame process (String string, int connectionId, Connections <String> connections, ConnectionHandler<String> handler){
        ClientFrameConnect clientFrame = new ClientFrameConnect(string);
        if (!connections.isConnected(connectionId)){
            if (!connections.correctPassword(username, passcode)){
                return new ServiceFrameError("Wrong password", clientFrame.receiptId, "user is not connected but the password is wrong");
            }
            connections.connectClient(connectionId, handler, clientFrame);
            return new ServiceFrameConnected();
        } else {
            return new ServiceFrameError("User already logged in", clientFrame.receiptId, "User already connected");
        }
    }

    public String toString (){
        return "CONNECT\n" +
                "accept-version:" + acceptVersion + "\n" +
                "host:" + host + "\n" +
                "username:" + username + "\n" +
                "passcode:" + passcode + "\n" +
                "receipt:" + receiptId + "\n" +
                this.body;
        }
}

