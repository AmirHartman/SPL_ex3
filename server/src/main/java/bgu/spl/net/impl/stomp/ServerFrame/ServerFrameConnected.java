package bgu.spl.net.impl.stomp.ServerFrame;

import bgu.spl.net.impl.stomp.StompCommand;

public class ServerFrameConnected extends ServerFrame {
    private final String version;

    public ServerFrameConnected(int receiptid) {
        super(StompCommand.CONNECTED);
        this.version = "1.2";
        this.receiptId = receiptid;
    }

    public String toString() {
        return type.name() + "\n" 
                + "version:"+ this.version+ "\n\n\u0000"; 
    }
    
}
