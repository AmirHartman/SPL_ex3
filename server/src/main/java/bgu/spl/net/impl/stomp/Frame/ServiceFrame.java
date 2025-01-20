package bgu.spl.net.impl.stomp.Frame;


public abstract class ServiceFrame {
    protected StompCommand type;
    protected String body;
    protected int receiptId = -1; // indication of an invalid recepit id

    public ServiceFrame(StompCommand type) {
        this.type = type;
        this.body = "\n\u0000";
    }

    public StompCommand getType() {
        return type;
    }

    public String getBody() {
        return body;
    }

    public int getReceiptId() {
        return receiptId;
    }

    public StompCommand stringToCommand (String type) {
        switch (type) {
            case "CONNECTED":
                return StompCommand.CONNECTED;
            case "MESSAGE":
                return StompCommand.MESSAGE;
            case "RECEIPT":
                return StompCommand.RECEIPT;
            case "ERROR":
                return StompCommand.ERROR;
            default:
                return null;
        }
    }

}
