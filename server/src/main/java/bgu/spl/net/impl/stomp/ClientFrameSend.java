package bgu.spl.net.impl.stomp;

import bgu.spl.net.impl.stomp.ServerFrame.*;
import bgu.spl.net.srv.Connections;


import java.util.concurrent.ConcurrentHashMap;

public class ClientFrameSend extends ClientFrame {
    private String destination;

    public ClientFrameSend(String destination, String body, int receiptId) {
        super(StompCommand.SEND);
        this.destination = destination;
        this.receiptId = receiptId;
        this.body = "\n" + body + "\n\u0000";
    }

    public ClientFrameSend(String toFrame){
        super(toFrame);
        String[] headers = toFrame.split("\n\n");
        String [] lines = headers[0].split("\n");
        for (int i = 1; i < lines.length; i++){
            String[] header = lines[i].split(":");
            switch (header[0]){
                case "destination":
                    this.destination = header[1].substring(1);
                    break;
                case "receipt":
                    try {
                        this.receiptId = Integer.parseInt(header[1]);
                    } catch (Exception e) {
                        System.out.println("unable to create frameSend, invalid receipt id");
                    } 
                    break;
            }
        }
        String[] body = toFrame.split("\n\n");
        this.body = "\n" + body[1] + "\u0000";
    }

    @Override
    public ServerFrame process (int connectionId, Connections <String> connections, StompMessagingProtocolImpl protocol){
        ConcurrentHashMap<Integer, Integer> subscribers = connections.getSubscribers(destination);
        if (subscribers == null){// should not happen
            return new ServerFrameError("channel does not exist", receiptId, toString());
        }
        if (!subscribers.containsKey(connectionId)){
            return new ServerFrameError("user is not subscribed to channel", receiptId, toString());
        }
        ServerFrameMessage messageFrame = new ServerFrameMessage(connections.getNextMessageId(), -1, destination, body);
        // updates subscription id for each subscriber
        for (Integer handlerId : subscribers.keySet()){
            if (handlerId != connectionId){
                messageFrame.setSubscribtion(connections.getSubscriptionId(destination, handlerId));
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