package bgu.spl.net.impl.stomp.Frame;

import bgu.spl.net.srv.Connections;

public class ClientFrameDisconnect<T> extends ClientFrame<T> {
    private int receiptId;

    public ClientFrameDisconnect(int receiptId) {
        super(ServiceStompCommand.DISCONNECT);
        this.receiptId = receiptId;
    }
    public ClientFrameDisconnect(String toFrame){
        super(toFrame);
        String[] header = toFrame.split("\n");
        try {
            this.receiptId = Integer.parseInt(header[1].split(":")[1]);
        } catch (Exception e) {
            System.out.println("unable to create frameDisconnect");
        }
    }


    @Override
    public void process (String string, Connections <T> connections){

    }
}