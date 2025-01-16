package bgu.spl.net.impl.stomp.serverFrame;

public class ServiceFrameReceipt extends ServiceFrame {
    private String receiptId;

    public ServiceFrameReceipt(String receiptId) {
        super(ServiceStompCommand.RECEIPT);
        this.receiptId = receiptId;
    }

    public String toString() {
        return type.name() + "\n" 
                // + "Headers:\n" // צריך הבדל? בשביל קידוד ופענוח מזהה את הסוף עם פעמיים סלש ואות אן
                + "Receipt-id: " + this.receiptId + "\n"
                + "\n" + this.nullChar;
    }
    
}
