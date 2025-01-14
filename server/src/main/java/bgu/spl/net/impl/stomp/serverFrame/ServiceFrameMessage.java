package bgu.spl.net.impl.stomp.serverFrame;

public class ServiceFrameMessage extends ServiceFrame {
    private int messageID;
    private int subscription;
    private String destination;

    public ServiceFrameMessage (int messageID, int subscription, String destination) {
        super(ServiceStompCommand.MESSAGE);
        this.messageID = messageID;
        this.subscription = subscription;
        this.destination = destination;
    }

    public String toString() {
        String result = "Stomp Command: MESSAGE \n" 
                + "Headers: \n" // צריך הבדל? בשביל קידוד ופענוח מזהה את הסוף עם פעמיים סלש ואות אן
                + "Message-id: " + this.messageID + "\n" 
                + "Subscription: " + this.subscription + "\n" 
                + "Destination: " + this.destination + "\n";
        if (this.body == "\n") {
            return result + "\n" + this.nullChar;
        } else {
            return result + "\n" + this.body + "\n" + this.nullChar;
        }
    }

    
}
