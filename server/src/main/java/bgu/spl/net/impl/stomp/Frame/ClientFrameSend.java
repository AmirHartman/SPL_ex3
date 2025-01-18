package bgu.spl.net.impl.stomp.Frame;

import bgu.spl.net.srv.Connections;

public class ClientFrameSend<T> extends ClientFrame<T> {
    private String destination;

    public ClientFrameSend(String destination) {
        super(ServiceStompCommand.SEND);
        this.destination = destination;
    }

    @Override
    public void process (String string, Connections <T> connections){

    }
}