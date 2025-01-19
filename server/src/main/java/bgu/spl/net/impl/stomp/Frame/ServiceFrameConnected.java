package bgu.spl.net.impl.stomp.Frame;


public class ServiceFrameConnected extends ServiceFrame {
    private final String version;
    private int receiptId;

    public ServiceFrameConnected(int receiptid) {
        super(ServiceStompCommand.CONNECTED);
        this.version = "1.2";
        this.receiptId = receiptid;
    }

    public ServiceFrameConnected(String string) {
        super(ServiceStompCommand.CONNECTED);
        this.version = "1.2";
    }


    public String toString() {
        return type.name() + "\n" 
                + "Version:" + this.version + "\n" 
                + "receipt:" + this.receiptId + "\n"
                + this.body;
    }
    
}
