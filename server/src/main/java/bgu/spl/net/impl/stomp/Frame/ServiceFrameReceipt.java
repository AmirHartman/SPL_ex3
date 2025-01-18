package bgu.spl.net.impl.stomp.Frame;

public class ServiceFrameReceipt extends ServiceFrame {
    private int receiptId;

    public ServiceFrameReceipt(int receiptId) {
        super(ServiceStompCommand.RECEIPT);
        this.receiptId = receiptId;
    }

    // לשנות לPOTECTED
    public ServiceFrameReceipt (String string){
        super(ServiceStompCommand.RECEIPT);
        String[] words = string.split(" ");
        if (!words[0].equals("RECEIPT")){
            throw new IllegalArgumentException("Not a RECEIPT frame");
        }
        this.receiptId = Integer.parseInt(words[3], 10);
    }

    // public ServiceFrame process(String string){
    //     String[] words = string.split(" ");
    //     if (!words[0].equals("ERROR")){
    //         throw new IllegalArgumentException("Not an ERROR frame");
    //     }
    //     int id = -1;
    //     try {
    //         id = Integer.parseInt(words[2], 10);
    //     } catch (NumberFormatException e) {
    //         System.out.println("messageId is not an integer");
    //         return null;
    //     }
    //     return new ServiceFrameReceipt(id);
    // }

    // public String toString() {
    //     return type.name() + " Receipt-id: " + this.receiptId + "\u0000";
    // }

    public String toString() {
        return type.name() + " \n" + " Receipt-id: " + this.receiptId + " \n" + this.body;
    }

    
}
