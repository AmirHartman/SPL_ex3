package bgu.spl.net.impl.stomp.Frame;

import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;

public class ClientFrameSend extends ClientFrame {
    private String destination;
    private int receiptId;

    public ClientFrameSend(String destination, int receiptId, String body) {
        super(ServiceStompCommand.SEND);
        this.destination = destination;
        this.receiptId = receiptId;
        this.body = "\n" + body + "\n\u0000";


    }

    public ClientFrameSend(String toFrame){
        super(toFrame);
        String[] lines = toFrame.split("\n");
        for (int i = 1; i < lines.length; i++){
            String[] header = lines[i].split(":");
            switch (header[0]){
                case "destination":
                    this.destination = header[1];
                    break;
                case "receipt":
                    try {
                        this.receiptId = Integer.parseInt(header[1]);
                    } catch (Exception e) {
                        System.out.println("unable to create frameSend, invalid receipt id");
                    } 
                    break;
            }
        }
        String[] body = toFrame.split("\n\n");
        this.body = "\n" + body[1] + "\u0000";
    }

    @Override
    public ServiceFrame process (String string, int connectionId, Connections <String> connections, ConnectionHandler<String> handler){
        return null;
    }

    public String toString (){
        return "SEND\n" +
                "destination:" + destination + "\n" +
                "receipt:" + receiptId + "\n" +
                this.body;
    }
}