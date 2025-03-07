package bgu.spl.net.impl.stomp.ServerFrame;

import bgu.spl.net.impl.stomp.StompCommand;

public class ServerFrameMessage extends ServerFrame {
    private int messageID;
    private int subscription;
    private String destination;

    public ServerFrameMessage (int messageID, int subscription, String destination, String body) {
        super(StompCommand.MESSAGE);
        this.messageID = messageID;
        this.subscription = subscription;
        this.destination = destination;
        this.body = "\n" + body;
    }

    public String getTopic() {
        return destination;
    }

    public String toString() {
        return type.name() + "\n"  
                + "message-id:" + this.messageID + "\n" 
                + "subscription:" + this.subscription + "\n" 
                + "destination:/" + this.destination + "\n" + this.body;
    }

    public void setSubscription(int subscription) {
        this.subscription = subscription;
    }

}
