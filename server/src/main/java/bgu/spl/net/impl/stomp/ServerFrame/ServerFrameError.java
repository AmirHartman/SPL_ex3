package bgu.spl.net.impl.stomp.ServerFrame;
import bgu.spl.net.impl.stomp.StompCommand;


public class ServerFrameError extends ServerFrame {
    private String message;

    public ServerFrameError(String message, int receiptId, String body) {
        super(StompCommand.ERROR);
        this.receiptId = receiptId;
        this.message = message;
        this.body = "\n" + body + "\n\u0000";
    }
    
    public String toString() {
        return type.name() + "\n"  
                + "Receipt-id:" + this.receiptId + "\n"
                + "Message:" + this.message + "\n" 
                + this.body;
    }    
    
    public String getMessage() {
        return message;
    }
}
