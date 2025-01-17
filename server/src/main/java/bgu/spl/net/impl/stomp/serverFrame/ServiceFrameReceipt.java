package bgu.spl.net.impl.stomp.serverFrame;

public class ServiceFrameReceipt extends ServiceFrame {
    private int receiptId;

    public ServiceFrameReceipt(int receiptId) {
        super(ServiceStompCommand.RECEIPT);
        this.receiptId = receiptId;
    }


    public ServiceFrame process(String string){
        String[] words = string.split(" ");
        if (!words[0].equals("RECEIPT")){
            return null;
        }
        int id = -1;
        try {
            id = Integer.parseInt(words[2], 10);
        } catch (NumberFormatException e) {
            System.out.println("messageId is not an integer");
            return null;
        }
        return new ServiceFrameReceipt(id);
    }

    // public String toString() {
    //     return type.name() + " Receipt-id: " + this.receiptId + "\u0000";
    // }

    public String toString() {
        return type.name() + " \n" + " Receipt-id: " + this.receiptId + " \n" + this.body + " \u0000";
    }

    
}
