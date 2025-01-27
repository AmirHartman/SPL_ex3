package bgu.spl.net.impl.stomp.ServerFrame;

import bgu.spl.net.impl.stomp.StompCommand;

public class ServerFrameConnected extends ServerFrame {
    private final String version;

    public ServerFrameConnected(int receiptid) {
        super(StompCommand.CONNECTED);
        this.version = "1.2";
        this.receiptId = receiptid;
    }

    public ServerFrameConnected(String string) {
        super(StompCommand.CONNECTED);
        this.version = "1.2";
    }


    public String toString() {
        return type.name() + "\n" 
                + "Version:" + this.version + "\n" 
                + "receipt:" + this.receiptId + "\n"
                + this.body;
    }
    
}
