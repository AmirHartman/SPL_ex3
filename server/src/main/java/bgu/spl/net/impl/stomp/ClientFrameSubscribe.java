package bgu.spl.net.impl.stomp;

import bgu.spl.net.impl.stomp.ServerFrame.ServerFrame;
import bgu.spl.net.impl.stomp.ServerFrame.ServerFrameError;
import bgu.spl.net.impl.stomp.ServerFrame.ServerFrameReceipt;
import bgu.spl.net.srv.Connections;


public class ClientFrameSubscribe extends ClientFrame {
    private int subscription;
    private String destination;

    public ClientFrameSubscribe(int subscription, String destination, int receiptId) {
        super(StompCommand.SUBSCRIBE);
        this.subscription = subscription;
        this.destination = destination;
        this.receiptId = receiptId;
    }

    public ClientFrameSubscribe(String toFrame){
        super(toFrame);
        String[] lines = toFrame.split("\n");
        // initialize headers
        for (int i = 1; i < lines.length; i++){
            String[] header = lines[i].split(":");
            switch (header[0]){
                case "destination":
                    this.destination = header[1].substring(1);
                    // this.destination = header[1];
                    break;
                case "id":
                    try {
                        this.subscription = Integer.parseInt(header[1]);
                    } catch (Exception e) {
                        System.out.println("unable to create frameSubscribe, invalid subscription id");
                    } 
                    break;
                case "receipt":
                    try {
                        this.receiptId = Integer.parseInt(header[1]);
                    } catch (Exception e) {
                        System.out.println("unable to create frame SUBSCRIBE, invalid receipt id");
                    } 
                    break;
        }}}

    @Override
    public ServerFrame process (int connectionId, Connections <String> connections, StompMessagingProtocolImpl protocol){
        if (!connections.isConnected(connectionId)){// should never happen
            return new ServerFrameError("Unconnected user is trying to subscribe to a channel", receiptId, toString());
        }
        protocol.subscriberIds.put(subscription, destination);
        connections.subscribe(connectionId, destination, subscription);
        return new ServerFrameReceipt(receiptId);
    }


    public String toString (){
        return "SUBSCRIBE\n" +
                "destination:" + destination + "\n" +
                "id:" + subscription + "\n" +
                "receipt:" + receiptId + "\n" +
                this.body;
    }

}