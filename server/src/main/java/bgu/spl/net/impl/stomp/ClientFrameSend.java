package bgu.spl.net.impl.stomp;

import bgu.spl.net.impl.stomp.ServerFrame.ServerFrame;
import bgu.spl.net.impl.stomp.ServerFrame.ServerFrameError;
import bgu.spl.net.impl.stomp.ServerFrame.ServerFrameMessage;
import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;

import java.util.concurrent.ConcurrentHashMap;

public class ClientFrameSend extends ClientFrame {
    private String destination;

    public ClientFrameSend(String destination, String body) {
        super(StompCommand.SEND);
        this.destination = destination;
        // this.receiptId = receiptId;
        this.body = "\n" + body + "\n\u0000";
    }

    public ClientFrameSend(String toFrame){
        super(toFrame);
        String[] header = toFrame.split("\n");
        this.destination = header[1].split(":/")[1];
        // for (int i = 1; i < lines.length; i++){
        //     String[] header = lines[i].split(":");
        //     switch (header[0]){
        //         case "destination":
        //             // String[] topic = header[1].split("/");
        //             this.destination = header[1];
        //             break;
        //         case "receipt":
        //             try {
        //                 this.receiptId = Integer.parseInt(header[1]);
        //             } catch (Exception e) {
        //                 System.out.println("unable to create frameSend, invalid receipt id");
        //             } 
        //             break;
        //     }
        // }
        String[] body = toFrame.split("\n\n");
        this.body = "\n" + body[1] + "\u0000";
    }

    @Override
    public ServerFrame process (int connectionId, Connections <String> connections, ConnectionHandler<String> handler, StompMessagingProtocolImpl protocol){
        // if (!connections.isConnected(connectionId)){
        //     return new ServerFrameError("Unconnected user is trying to send a message", receiptId, toString());
        // }
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
            messageFrame.setSubscribtion(connections.getSubscriptionId(destination, handlerId));
            connections.send(handlerId, messageFrame.toString());
        }
        return null;
    }

    protected boolean validFrame(String toFrame){
        return false;
    }

    public String toString (){
        return "SEND\n" +
                "destination:" + destination + "\n" +
                // "receipt:" + receiptId + "\n" +
                this.body;
    }


}