package bgu.spl.net.impl.stomp;

import bgu.spl.net.impl.stomp.ServerFrame.ServerFrame;
import bgu.spl.net.impl.stomp.ServerFrame.ServerFrameError;
import bgu.spl.net.impl.stomp.ServerFrame.ServerFrameMessage;
import bgu.spl.net.impl.stomp.ServerFrame.ServerFrameReceipt;
import bgu.spl.net.srv.Connections;

import java.util.concurrent.ConcurrentHashMap;

public class ClientFrameSend extends ClientFrame {
    private String destination;

    public ClientFrameSend(String destination, String body) {
        super(StompCommand.SEND);
        this.destination = destination;
        // this.receiptId = receiptId;
        this.body = "\n" + body;
    }

    public ClientFrameSend(String toFrame){
        super(toFrame);
        String[] headers = toFrame.split("\n\n")[0].split("\n");
        for (int i = 1; i < headers.length; i++){
            String[] header = headers[i].split(":");
            switch (header[0]){
                case "destination":
                    this.destination = header[1].substring(1);
                    break;
                case "receipt":
                    try {
                        this.receiptId = Integer.parseInt(header[1]);
                    } catch (Exception e) {
                        System.out.println("unable to create frame SEND, invalid receipt id");
                    } 
                    break;
            }
        }
        String body = toFrame.split("\n\n")[1];
        this.body = "\n" + body;
    }

    @Override
    public ServerFrame process (int connectionId, Connections <String> connections, StompMessagingProtocolImpl protocol){
        ConcurrentHashMap<Integer, Integer> subscribers = connections.getSubscribers(destination);
        if (subscribers == null){// should never happen
            return new ServerFrameError("channel does not exist", receiptId, toString());
        }
        if (!subscribers.containsKey(connectionId)){
            return new ServerFrameError("user tries to send a message to a channel it's not subscribe to", receiptId, toString());
        }
        ServerFrameMessage messageFrame = new ServerFrameMessage(connections.getNextMessageId(), -1, destination, body);
        // updates subscription id for each subscriber
        for (Integer handlerId : subscribers.keySet()){
            if (handlerId != connectionId){
                messageFrame.setSubscription(connections.getSubscriptionId(destination, handlerId));
                connections.send(handlerId, messageFrame.toString());
            }
        }
        return new ServerFrameReceipt(receiptId);
    }

    protected boolean validFrame(String toFrame){
        return false;
    }

    public String toString (){
        return "SEND\n" +
                "destination:/" + destination + "\n" +
                "receipt:" + receiptId + "\n" +
                this.body;
    }


}