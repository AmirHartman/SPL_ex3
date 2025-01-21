package bgu.spl.net.impl.stomp;

import java.util.AbstractMap.SimpleEntry;
import java.util.concurrent.ConcurrentHashMap;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.impl.stomp.ServerFrame.*;


public class StompMessagingProtocolImpl implements StompMessagingProtocol<String>{
    private boolean shouldTerminate = false;
    private ConcurrentHashMap<Integer, SimpleEntry<ClientFrame, ServerFrame>> receipts = new ConcurrentHashMap<>();
    //צריך מקביליות? אולי בריאקטור?
    protected ConcurrentHashMap<Integer, String> subscriberIds = new ConcurrentHashMap<>();
    private int connectionId;
    private Connections<String> connections;
    private ConnectionHandler<String> handler;
    


    @Override
    public void start(int connectionId, Connections<String> connections, ConnectionHandler<String> handler) {
        this.connectionId = connectionId;
        this.connections = connections;
        this.handler = handler;
    }
    

    public void process(String message){
        String commandtmp = message.split("\n")[0];
        StompCommand command = Auxiliary.stringToCommand(commandtmp);
        if (command == null){
            //reciept id -1 indicates an invalid frame structure
            ServerFrameError error = new ServerFrameError("Invalid Command", -1, message);
            connections.send(connectionId, error.toString());
        }
        // check client frame structure: header names, null char, body, etc.
        ServerFrameError error = Auxiliary.validateClientFrame(command, message);
        if (error != null){
            connections.send(connectionId, error.toString());
        }
        else {// valid frame
            ClientFrame clientFrame = Auxiliary.chooseClientFrame(command, message); 
            if (receipts.containsKey(clientFrame.getReceiptId())){
                error = new ServerFrameError("receipt id isn't unique", clientFrame.getReceiptId(), message);
                receipts.put(clientFrame.getReceiptId(), new SimpleEntry<>(clientFrame, error));
                connections.send(connectionId, error.toString());
            }
            else{
                receipts.put(clientFrame.getReceiptId(), new SimpleEntry<>(clientFrame, null));
                ServerFrame serverFrame = clientFrame.process(connectionId, connections, handler, this);
                // send the message to all subscribers of that topic
                if (serverFrame instanceof ServerFrameMessage){
                    String topic = ((ServerFrameMessage) serverFrame).getTopic();
                    connections.send(topic, serverFrame.toString());
                    serverFrame = new ServerFrameReceipt(clientFrame.getReceiptId());
                }
                connections.send(connectionId, serverFrame.toString());
                SimpleEntry<ClientFrame,ServerFrame> entry = receipts.get(clientFrame.getReceiptId());
                // האם יכול להיות שלא יהיה NULL?
                entry.setValue(serverFrame);
            }}}
	
   public boolean shouldTerminate(){
    return shouldTerminate;
   }
}

    