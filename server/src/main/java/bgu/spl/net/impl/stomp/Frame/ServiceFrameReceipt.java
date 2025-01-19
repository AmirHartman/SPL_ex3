package bgu.spl.net.impl.stomp.Frame;

public class ServiceFrameReceipt extends ServiceFrame {
    private int receiptId;

    public ServiceFrameReceipt(int receiptId) {
        super(ServiceStompCommand.RECEIPT);
        this.receiptId = receiptId;
    }

    // למחוק?
    public ServiceFrameReceipt (String string){
        super(ServiceStompCommand.RECEIPT);
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
