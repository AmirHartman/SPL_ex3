package bgu.spl.net.impl.stomp;

import bgu.spl.net.impl.stomp.ServerFrame.ServerFrame;
import bgu.spl.net.impl.stomp.ServerFrame.ServerFrameError;
import bgu.spl.net.impl.stomp.ServerFrame.ServerFrameReceipt;
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
        String[] lines = toFrame.split("\n");
        // initialize headers
        for (int i = 1; i < lines.length; i++){
            String[] header = lines[i].split(":");
            switch (header[0]){
                case "id":
                    try {
                        this.subscription = Integer.parseInt(header[1]);
                    } catch (Exception e) {
                        System.out.println("unable to create frame UNSUBSCRIBE, invalid subscription id");
                    } 
                    break;
                case "receipt":
                    try {
                        this.receiptId = Integer.parseInt(header[1]);
                    } catch (Exception e) {
                        System.out.println("unable to create frame UNSUBSCRIBE, invalid receipt id");
                    } 
                    break;
            }}}

    @Override
    public ServerFrame process (int connectionId, Connections <String> connections, StompMessagingProtocolImpl protocol){
        if (!protocol.subscriberIds.containsKey(subscription)){// should never happen 
            return new ServerFrameError("user is not subscribed to channel", receiptId, toString());
        }
        String topic = protocol.subscriberIds.remove(subscription);
        connections.unsubscribe(connectionId, topic);
        return new ServerFrameReceipt(receiptId);
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