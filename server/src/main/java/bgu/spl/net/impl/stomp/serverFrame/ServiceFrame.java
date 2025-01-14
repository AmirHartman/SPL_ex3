package bgu.spl.net.impl.stomp.serverFrame;

import bgu.spl.net.impl.stomp.serverFrame.ServiceStompCommand;

public abstract class ServiceFrame {
    private ServiceStompCommand type;
    private String body;
    final private char nullChar = '\u0000';

    public ServiceFrame(ServiceStompCommand type) {
        this.type = type;
        this.body = "\n";
    }

    public String commandToString () {
        switch (type) {
            case CONNECTED:
                return "CONNECTED";
            case MESSAGE:
                return "MESSAGE";
            case RECEIPT:
                return "RECEIPT";
            case ERROR:
                return "ERROR";
            default:
                return null;
        }
    }
    
}
