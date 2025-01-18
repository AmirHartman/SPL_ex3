package bgu.spl.net.impl.stomp.Frame;

import bgu.spl.net.srv.Connections;

public class ClientFrameUnsubscribe<T> extends ClientFrame<T> {
    private int subscription;

    public ClientFrameUnsubscribe(int subscription) {
        super(ServiceStompCommand.UNSUBSCRIBE);
        this.subscription = subscription;
    }

    public ClientFrameUnsubscribe(String toFrame){
        super(toFrame);
        String[] header = toFrame.split("\n");
        try {
            this.subscription = Integer.parseInt(header[1].split(":")[1]);
        } catch (Exception e) {
            System.out.println("unable to create frameUnsubscribe, invalid subscription id");
        }
    }

    @Override
    public void process (String string, Connections <T> connections){

    }
}