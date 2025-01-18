package bgu.spl.net.impl.stomp.Frame;


public abstract class ServiceFrame {
    protected ServiceStompCommand type;
    protected String body;

    public ServiceFrame(ServiceStompCommand type) {
        this.type = type;
        this.body = "\n";
    }

    public ServiceStompCommand getType() {
        return type;
    }

    public String getBody() {
        return body;
    }

    public ServiceStompCommand stringToCommand (String type) {
        switch (type) {
            case "CONNECTED":
                return ServiceStompCommand.CONNECTED;
            case "MESSAGE":
                return ServiceStompCommand.MESSAGE;
            case "RECEIPT":
                return ServiceStompCommand.RECEIPT;
            case "ERROR":
                return ServiceStompCommand.ERROR;
            default:
                return null;
        }
    }

    public void test () {
        System.out.println("test, i am OGclass");
    }


}
