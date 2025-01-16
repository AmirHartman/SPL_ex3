package bgu.spl.net.impl.stomp.serverFrame;

public class ServiceFrameMessage extends ServiceFrame {
    private int messageID;
    private int subscription;
    private String destination;

    public ServiceFrameMessage (int messageID, int subscription, String destination, String body) {
        super(ServiceStompCommand.MESSAGE);
        this.messageID = messageID;
        this.subscription = subscription;
        this.destination = destination;
        this.body = body;
    }

    public String toString() {
        String result = type.name() + "\n"  
                // + "Headers:\n" // צריך הבדל? בשביל קידוד ופענוח מזהה את הסוף עם פעמיים סלש ואות אן
                + "Message-id: " + this.messageID + "\n" 
                + "Subscription: " + this.subscription + "\n" 
                + "Destination: " + this.destination + "\n";
        if (this.body == "\n") {
            return result + "\n";
        } else {
            return result + "\n" + this.body + "\n";
        }
    }

    
}
