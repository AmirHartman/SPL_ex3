package bgu.spl.net.impl.stomp.serverFrame;


public class ServiceFrameError extends ServiceFrame {
    private int receiptId;
    private String message;

    public ServiceFrameError(String message, int receiptId, String body) {
        super(ServiceStompCommand.ERROR);
        this.receiptId = receiptId;
        this.message = message;
        this.body = body;
    }

    public String toString() {
        String result =  type.name() + "\n"  
                // + "Headers:\n" // צריך הבדל? בשביל קידוד ופענוח מזהה את הסוף עם פעמיים סלש ואות אן
                + "Receipt-id: " + this.receiptId + "\n"
                + "Message: " + this.message + "\n" ;
        if (this.body == "\n") {
            return result + "\n" + this.nullChar;
        } else {
            return result + "\n" + this.body + "\n" + this.nullChar;
        }
    }    
    
}
