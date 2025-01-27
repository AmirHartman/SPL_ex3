package bgu.spl.net.impl.stomp.ServerFrame;

import bgu.spl.net.impl.stomp.StompCommand;

public class ServerFrameReceipt extends ServerFrame {

    public ServerFrameReceipt(int receiptId) {
        super(StompCommand.RECEIPT);
        this.receiptId = receiptId;
    }

    // למחוק?
    public ServerFrameReceipt (String string){
        super(StompCommand.RECEIPT);
        String[] words = string.split(" ");
        if (!words[0].equals("RECEIPT")){
            throw new IllegalArgumentException("Not a RECEIPT frame");
        }
        this.receiptId = Integer.parseInt(words[3], 10);
    }

    public String toString() {
        return type.name() + "\n" + 
        "Receipt-id:" + this.receiptId + "\n" 
        + this.body;
    }

    
}
