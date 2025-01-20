package bgu.spl.net.impl.stomp;

import java.security.Provider.Service;
import java.util.AbstractMap.SimpleEntry;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedDeque;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.impl.stomp.Frame.*;


public class StompMessagingProtocolImplTemp implements StompMessagingProtocol<String>{
    private boolean shouldTerminate = false;
    private ConcurrentHashMap<Integer, SimpleEntry<ClientFrame, ServiceFrame>> receipts = new ConcurrentHashMap<>();
    private int connectionId;
    private Connections<String> connections;
    


    @Override
    public void start(int connectionId, Connections<String> connections){
        this.connectionId = connectionId;
        this.connections = connections;
    }
    

    public void process(String message){
        String commandtmp = message.split("\n")[0];
        StompCommand command = Auxiliary.stringToCommand(commandtmp);
        if (command == null){
            //reciept id -1 indicates an invalid frame structure
            ServiceFrameError error = new ServiceFrameError("Invalid Command", -1, message);
            connections.send(connectionId, error.toString());
        }
        // check client frame structure: header names, null char, body, etc.
        ServiceFrameError error = Auxiliary.validateClientFrame(command, message);
        if (error != null){
            connections.send(connectionId, error.toString());
        }
        else {// valid frame
            ClientFrame clientFrame = Auxiliary.chooseClientFrame(command, message); 
            if (receipts.containsKey(clientFrame.getReceiptId())){
                error = new ServiceFrameError("receipt id isn't unique", clientFrame.getReceiptId(), message);
                receipts.put(clientFrame.getReceiptId(), new SimpleEntry<>(clientFrame, error));
                connections.send(connectionId, error.toString());
            }
            else{
                receipts.put(clientFrame.getReceiptId(), new SimpleEntry<>(clientFrame, null));
                // אולי התייחסות שונה אם זה רישום או יציאה מערוץ
                ServiceFrame serverFrame = clientFrame.process(connectionId, connections);
                // if (serviceFrame.getClass().isAssignableFrom(ServiceFrameMessage.class))
                // send the message to all the subscribers to that topic
                if (serverFrame instanceof ServiceFrameMessage){
                    String topic = ((ServiceFrameMessage) serverFrame).getTopic();
                    connections.send(topic, serverFrame.toString());
                    serverFrame = new ServiceFrameReceipt(clientFrame.getReceiptId());
                }
                connections.send(connectionId, serverFrame.toString());
                SimpleEntry<ClientFrame,ServiceFrame> entry = receipts.get(clientFrame.getReceiptId());
                // האם יכול להיות שלא יהיה NULL?
                entry.setValue(serverFrame);
            }


                
        }

   }
	
   public boolean shouldTerminate(){
    return shouldTerminate;
   }


}

    