package bgu.spl.net.impl.stomp.Frame;

import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;

public class ClientFrameSubscribe extends ClientFrame {
    private int subscription;
    private String destination;
    private int receiptId;

    public ClientFrameSubscribe(int subscription, String destination, int receiptId) {
        super(ServiceStompCommand.SUBSCRIBE);
        this.subscription = subscription;
        this.destination = destination;
        this.receiptId = receiptId;
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
                case "receipt":
                    try {
                        this.receiptId = Integer.parseInt(header[1]);
                    } catch (Exception e) {
                        System.out.println("unable to create frameSubscribe, invalid receipt id");
                    } 
                    break;
        }}}

    @Override
    public ServiceFrame process (String string, int connectionId, Connections <String> connections, ConnectionHandler<String> handler){
        if (!validFrame(string)){
            return new ServiceFrameError("subscribe frame is invalid", receiptId, "frame structure or headers or both are invalid");
        }
        if (!connections.isConnected(handler.getUserName())){
            return new ServiceFrameError("user is not connected", receiptId, "user is not connected to the server");
        }
        ClientFrameSubscribe clientFrame = new ClientFrameSubscribe(string);
        connections.subscribeClient(connectionId, clientFrame.destination, handler);
        return new ServiceFrameReceipt(receiptId);
    }

    protected boolean validFrame(String toFrame){
        // check the validity of the frame structure
        int headers = toFrame.split(":").length-1;
        String [] frame = toFrame.split("\n\n");
        String body = frame[1];
        if (headers != 3 | !body.equals("\u0000")){
            return false;
        }
        // check the validity of the headers
        String[] lines = frame[1].split("\n");
        for (int i = 1; i < lines.length; i++){
            String[] header = lines[i].split(":");
            if (!header[0].equals("destination") & !header[0].equals("id") & !header[0].equals("receipt")){
                return false;
            }
        }
        return true;
    }

    public String toString (){
        return "SUBSCRIBE\n" +
                "destination:" + destination + "\n" +
                "id:" + subscription + "\n" +
                "receipt:" + receiptId + "\n" +
                this.body;
    }

}