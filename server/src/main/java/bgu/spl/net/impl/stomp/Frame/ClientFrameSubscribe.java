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

    public ClientFrameSubscribe(String toFrame){
        super(toFrame);
        String[] lines = toFrame.split("\n");
        // initialize headers
        for (int i = 1; i < lines.length && lines[i] != "\u0000"; i++){
            String[] header = lines[i].split(":");
            switch (header[0]){
                case "destination":
                    this.destination = header[1];
                case "id":
                    try {
                        this.subscribtion = Integer.parseInt(header[1]);
                    } catch (Exception e) {
                        System.out.println("unable to create frameSubscribe, unvalid subscribtion id");
                    } 
        }}}

    @Override
    public void process (String string, Connections <T> connections){

    }
}