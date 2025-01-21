package bgu.spl.net.impl.stomp.Frame;

import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;


public class ClientFrameConnect extends ClientFrame {
    private String acceptVersion = "1.2";
    private String host = "stomp.cs.bgu.ac.il";
    private String username;
    private String passcode;

    public ClientFrameConnect(String username, String passcode, int receiptId){
        super(StompCommand.CONNECT);
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
    public ServerFrame process (String string, int connectionId, Connections <String> connections, ConnectionHandler<String> handler){
        // check the structure of client frame before creation
        if (!validFrame(string)){
            return new ServerFrameError("connect frame is invalid", -1, "problem with frame structure, version, host or invalid headers");
        }

        // check correctness of version and host
        ServerFrameError error = checkHeaders(string);
        if (error != null){
            return error;
        }
        //connect client if password is correct and is not already connected
        ClientFrameConnect clientFrame = new ClientFrameConnect(string);
        if (!connections.correctPassword(username, passcode)){
            return new ServerFrameError("Wrong password", clientFrame.receiptId, "user is not connected but the password is wrong");
        }
        if (connections.connectClient(connectionId, handler, clientFrame)){
            return new ServerFrameConnected(clientFrame.receiptId);
        } else {
            return new ServerFrameError("User already logged in", clientFrame.receiptId, "User already connected");
        }
    }


    protected boolean validFrame(String toFrame){
        // check the validity of the frame structure
        int headers = toFrame.split(":").length-1;
        String [] frame = toFrame.split("\n\n");
        String body = frame[1];
        if (headers != 5 | !body.equals("\u0000")){
            return false;
        }
        // check the validity of the headers
        String[] lines = frame[1].split("\n");
            for (int i = 1; i < lines.length; i++){
                String[] header = lines[i].split(":");
                if (!header[0].equals("accept-version") & !header[0].equals("host") & !header[0].equals("username" ) & !header[0].equals("passcode") & !header[0].equals("receipt")){
                    return false;
                }
            }
            return true;    
        }

        private ServerFrameError checkHeaders (String toFrame){
            ClientFrameConnect clientFrame = new ClientFrameConnect(toFrame);
            String[] lines = toFrame.split("\n");
            for (int i = 1; i < lines.length; i++){
                String[] header = lines[i].split(":");
                if (header[0].equals("accept-version") & !header[1].equals("1.2")){
                    return new ServerFrameError("Wrong version", clientFrame.receiptId, "version is not 1.2");
                }
                if (header[0].equals("host") & !header[1].equals("stomp.cs.bgu.ac.il")){
                    return new ServerFrameError("Wrong host", clientFrame.receiptId, "host is not stomp.cs.bgu.ac.il");
                }
            }
            return null;
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

