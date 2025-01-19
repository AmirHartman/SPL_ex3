package bgu.spl.net.impl.stomp.Frame;

import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;

public abstract class ClientFrame {
    protected ServiceStompCommand type;
    protected String body;
    protected Connections<String> connections;

    public ClientFrame(ServiceStompCommand type) {
        this.type = type;
        this.body = "\n \u0000";
    }

    public ClientFrame(String toFrame){
        String type = toFrame.substring(0, toFrame.indexOf('\n'));
        this.type = stringToCommand(type);
        this.body = "\n\u0000";
    }

    public ServiceStompCommand getType() {
        return type;
    }

    public String getBody() {
        return body;
    }

    public abstract ServiceFrame process (String string, int connectionId, Connections <String> connections, ConnectionHandler<String> handler);

    protected abstract boolean validFrame(String toFrame);


    public ServiceStompCommand stringToCommand (String type) {
        switch (type) {
            case "CONNECT":
                return ServiceStompCommand.CONNECT;
            case "SUBSCRIBE":
                return ServiceStompCommand.SUBSCRIBE;
            case "UNSUBSCRIBE":
                return ServiceStompCommand.UNSUBSCRIBE;
            case "SEND":
                return ServiceStompCommand.SEND;
            case "DISCONNECT":
                return ServiceStompCommand.DISCONNECT;
            default:
                return null;
        }
    }

}
