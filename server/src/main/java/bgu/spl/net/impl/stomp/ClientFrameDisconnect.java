package bgu.spl.net.impl.stomp.Frame;

import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;

public class ClientFrameDisconnect extends ClientFrame {

    public ClientFrameDisconnect(int receiptId) {
        super(StompCommand.DISCONNECT);
        this.receiptId = receiptId;
    }

    public ClientFrameDisconnect(String toFrame){
        super(toFrame);
        String[] header = toFrame.split("\n");
        try {
            this.receiptId = Integer.parseInt(header[1].split(":")[1]);
        } catch (Exception e) {
            System.out.println("unable to create frameDisconnect, invalid receipt id");
        }
    }

    @Override
    public ServerFrame process (String string, int connectionId, Connections <String> connections, ConnectionHandler<String> handler){
        return null;
    }

    protected boolean validFrame(String toFrame){
        return false;
    }


    public String toString (){
        return "DISCONNECT\n" +
                "receipt:" + receiptId + "\n" +
                this.body;
    }
}