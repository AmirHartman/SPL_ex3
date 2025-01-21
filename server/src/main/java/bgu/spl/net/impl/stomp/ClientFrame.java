package bgu.spl.net.impl.stomp.Frame;

import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.impl.stomp.Auxiliary;

public abstract class ClientFrame {
    protected StompCommand type;
    protected String body;
    protected int receiptId = -1; // indication of an invalid recepit id

    public ClientFrame(StompCommand type) {
        this.type = type;
        this.body = "\n \u0000";
    }

    public ClientFrame(String toFrame){
        String type = toFrame.substring(0, toFrame.indexOf('\n'));
        this.type = Auxiliary.stringToCommand(type);
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

    public abstract ServerFrame process (int connectionId, Connections <String> connections, ConnectionHandler<String> handler);

    protected abstract boolean validFrame(String toFrame);



}
