package bgu.spl.net.impl.stomp.serverFrame;

public abstract class ServiceFrame {
    protected ServiceStompCommand type;
    protected String body;
    protected final char nullChar = '\u0000';

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

    
}
