package bgu.spl.net.impl.stomp;

import bgu.spl.net.impl.stomp.ServerFrame.ServerFrame;
import bgu.spl.net.impl.stomp.ServerFrame.ServerFrameReceipt;
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
    public ServerFrame process (int connectionId, Connections <String> connections, StompMessagingProtocolImpl protocol){
        connections.disconnect(connectionId);
        // ServerFrameReceipt receipt = new ServerFrameReceipt(receiptId);
        // connections.send(connectionId, receipt.toString());
        // System.out.println("message sent to send method in connections from process in ClientFrameDisconnect");
        return new ServerFrameReceipt(receiptId);
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