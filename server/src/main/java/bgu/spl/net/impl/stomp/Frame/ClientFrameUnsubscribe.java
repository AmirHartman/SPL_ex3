package bgu.spl.net.impl.stomp.Frame;

import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;

public class ClientFrameUnsubscribe extends ClientFrame {
    private int subscription;

    public ClientFrameUnsubscribe(int subscription, int receiptId){
        super(StompCommand.UNSUBSCRIBE);
        this.subscription = subscription;
        this.receiptId = receiptId;
    }

    // לתקן בבוקר
    public ClientFrameUnsubscribe(String toFrame){
        super(toFrame);
        String[] header = toFrame.split("\n");
        try {
            this.subscription = Integer.parseInt(header[1].split(":")[1]);
        } catch (Exception e) {
            System.out.println("unable to create frameUnsubscribe, invalid subscription id");
        }
    }

    @Override
    public ServerFrame process (String string, int connectionId, Connections <String> connections, ConnectionHandler<String> handler){
        if (!validFrame(string)){
            return new ServerFrameError("unsubscribe frame is invalid", receiptId, "frame structure or headers or both are invalid");
        }
        if (!connections.isConnected(handler.getUserName())){
            return new ServerFrameError("user is not connected", receiptId, "user is not connected to the server and trying to unsubscribe from a channel");
        }
        ClientFrameUnsubscribe clientFrame = new ClientFrameUnsubscribe(string);
        // connections.unsubscribeClient(connectionId, clientFrame.subscription);
        return null;
    }
    
    protected boolean validFrame(String toFrame){
        // check the validity of the frame structure
        int headers = toFrame.split(":").length-1;
        String [] frame = toFrame.split("\n\n");
        String body = frame[1];
        if (headers != 1 | !body.equals("\u0000")){
            return false;
        }
        // check the validity of the header
        String[] lines = frame[1].split("\n");
        String[] header = lines[1].split(":");
        if (!header[0].equals("id") ){
            return false;
        }
        try {
            Integer.parseInt(header[1]);
        } catch (Exception e) {
            return false;
        }
        return true;
    }

    public String toString (){
        return "UNSUBSCRIBE\n" +
                "id:" + subscription + "\n" +
                "receipt:" + receiptId + "\n" +
                this.body;
    }

}