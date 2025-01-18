package bgu.spl.net.impl.stomp.Frame;

import bgu.spl.net.srv.Connections;

public class ClientFrameUnsubscribe<T> extends ClientFrame<T> {
    private int subscribtion;

    public ClientFrameUnsubscribe(int subscribtion) {
        super(ServiceStompCommand.UNSUBSCRIBE);
        this.subscribtion = subscribtion;
    }

    @Override
    public void process (String string, Connections <T> connections){

    }
}