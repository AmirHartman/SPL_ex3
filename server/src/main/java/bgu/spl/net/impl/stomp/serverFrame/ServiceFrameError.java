package bgu.spl.net.impl.stomp.serverFrame;


public class ServiceFrameError extends ServiceFrame {
    private String receiptId;
    private String message;

    public ServiceFrameError(String message, String receiptId) {
        super(ServiceStompCommand.ERROR);
        this.receiptId = receiptId;
        this.message = message;
    }

    public String toString() {
        String result =  "Stomp Command: ERROR \n" 
                + "Headers: \n" // צריך הבדל? בשביל קידוד ופענוח מזהה את הסוף עם פעמיים סלש ואות אן
                + "Receipt-id: " + this.receiptId + "\n"
                + "Message: " + this.message + "\n" ;
        if (this.body == "\n") {
            return result + "\n" + this.nullChar;
        } else {
            return result + "\n" + this.body + "\n" + this.nullChar;
        }
    }    
    
}
