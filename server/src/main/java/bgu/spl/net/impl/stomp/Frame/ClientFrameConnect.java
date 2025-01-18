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
        if (!validFrame(string)){
            return new ServiceFrameError("connect frame is invalid", receiptId, "problem with frame structure, version, host or invalid headers");
        }
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

    protected boolean validFrame(String toFrame){
        // check the validity of the frame structure
        int headers = toFrame.split(":").length;
        String [] frame = toFrame.split("\n\n");
        String body = frame[1];
        if (headers != 6 | !body.equals("\u0000")){
            return false;
        }
        // check the validity of the headers
        String[] lines = frame[1].split("\n");
            for (int i = 1; i < lines.length; i++){
                String[] header = lines[i].split(":");
                if (!header[0].equals("accept-version") & !header[0].equals("host") & !header[0].equals("username" ) & !header[0].equals("passcode") & !header[0].equals("receipt")){
                    return false;
                }
                if (header[0].equals("accept-version") & !header[1].equals("1.2")){
                    return false;
                }
                if (header[0].equals("host") & !header[1].equals("stomp.cs.bgu.ac.il")){
                    return false;
                }
            }
            return true;    
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

