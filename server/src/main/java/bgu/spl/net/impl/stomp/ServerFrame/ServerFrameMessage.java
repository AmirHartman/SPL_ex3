package bgu.spl.net.impl.stomp.ServerFrame;

import bgu.spl.net.impl.stomp.StompCommand;

public class ServerFrameMessage extends ServerFrame {
    private int messageID;
    private int subscribtion;
    private String destination;

    public ServerFrameMessage (int messageID, int subscribtion, String destination, String body) {
        super(StompCommand.MESSAGE);
        this.messageID = messageID;
        this.subscribtion = subscribtion;
        this.destination = destination;
        this.body = "\n" + body + "\n\u0000";
    }

    public String getTopic() {
        return destination;
    }

    public String toString() {
        return type.name() + "\n"  
                + "message-id:" + this.messageID + "\n" 
                + "subscribtion:" + this.subscribtion + "\n" 
                + "destination:/topic/" + this.destination + "\n" + this.body;
    }

    public void setSubscribtion(int subscribtion) {
        this.subscribtion = subscribtion;
    }

}
