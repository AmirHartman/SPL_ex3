package bgu.spl.net.impl.stomp.Frame;

import bgu.spl.net.srv.Connections;

public class ClientFrameSend<T> extends ClientFrame<T> {
    private String destination;
    private int receiptId;

    public ClientFrameSend(String destination, int receiptId) {
        super(ServiceStompCommand.SEND);
        this.destination = destination;
        this.receiptId = receiptId;
    }

    public ClientFrameSend(String toFrame){
        super(toFrame);
        String[] header = toFrame.split("\n");
        this.destination = header[1].split(":")[1];
        String[] body = toFrame.split("\n\n");
        this.body = body[1];
    }

    @Override
    public void process (String string, Connections <T> connections){

    }
}