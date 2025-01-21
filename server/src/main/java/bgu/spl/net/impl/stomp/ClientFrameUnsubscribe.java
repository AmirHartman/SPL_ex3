package bgu.spl.net.impl.stomp;

import bgu.spl.net.impl.stomp.Frame.StompCommand;
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
    public ServerFrame process (int connectionId, Connections <String> connections, ConnectionHandler<String> handler, StompMessagingProtocolImpl protocol){
        if (!connections.isConnected(handler.getUserName())){
            return new ServerFrameError("Unconnected user is trying to Unsubscribe from a channel", receiptId, toString());
        }
        if (!protocol.subscriberIds.containsKey(subscription)){
            return new ServerFrameError("user is not subscribed to channel", receiptId, toString());
        }
        String topic = protocol.subscriberIds.remove(subscription);
        connections.unsubscribe(connectionId, topic));
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