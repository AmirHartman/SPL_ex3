package bgu.spl.net.impl.stomp.Frame;

import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;

public class ClientFrameSubscribe extends ClientFrame {
    private int subscription;
    private String destination;

    public ClientFrameSubscribe(int subscription, String destination) {
        super(ServiceStompCommand.SUBSCRIBE);
        this.subscription = subscription;
        this.destination = destination;
    }

    public ClientFrameSubscribe(String toFrame){
        super(toFrame);
        String[] lines = toFrame.split("\n");
        // initialize headers
        for (int i = 1; i < lines.length; i++){
            String[] header = lines[i].split(":");
            switch (header[0]){
                case "destination":
                    this.destination = header[1];
                    break;
                case "id":
                    try {
                        this.subscription = Integer.parseInt(header[1]);
                    } catch (Exception e) {
                        System.out.println("unable to create frameSubscribe, invalid subscription id");
                    } 
                    break;
        }}}

    @Override
    public ServiceFrame process (String string, int connectionId, Connections <String> connections, ConnectionHandler<String> handler){
        return null;
    }

    public String toString (){
        return "SUBSCRIBE\n" +
                "destination:" + destination + "\n" +
                "id:" + subscription + "\n" +
                this.body;
    }
}