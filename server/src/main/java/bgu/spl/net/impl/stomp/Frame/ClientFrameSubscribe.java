package bgu.spl.net.impl.stomp.Frame;

import bgu.spl.net.srv.Connections;

public class ClientFrameSubscribe<T> extends ClientFrame<T> {
    private int subscribtion;
    private String destination;

    public ClientFrameSubscribe(int subscribtion, String destination) {
        super(ServiceStompCommand.SUBSCRIBE);
        this.subscribtion = subscribtion;
        this.destination = destination;
    }

    @Override
    public void process (String string, Connections <T> connections){

    }
}