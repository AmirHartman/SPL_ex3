package bgu.spl.net.impl.stomp.ServerFrame;

import bgu.spl.net.impl.stomp.StompCommand;

public class ServerFrameReceipt extends ServerFrame {

    public ServerFrameReceipt(int receiptId) {
        super(StompCommand.RECEIPT);
        this.receiptId = receiptId;
    }

    public String toString() {
        return type.name() + "\n" + 
        "receipt-id:" + this.receiptId + "\n" 
        + this.body;
    }
    
}
