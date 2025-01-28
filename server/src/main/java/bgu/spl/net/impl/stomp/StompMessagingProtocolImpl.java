package bgu.spl.net.impl.stomp;

import java.util.concurrent.ConcurrentHashMap;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.impl.stomp.ServerFrame.*;


public class StompMessagingProtocolImpl implements StompMessagingProtocol<String>{
    private boolean shouldTerminate = false;
    protected ConcurrentHashMap<Integer, String> subscriberIds = new ConcurrentHashMap<>();
    private int connectionId;
    private Connections<String> connections;
    private ConnectionHandler<String> handler = null;
    private boolean crushed = false;
    


    @Override
    public void start(int connectionId, Connections<String> connections) {
        this.connectionId = connectionId;
        this.connections = connections;
    }
    
    @Override
    public void process(String message){
        String commandtmp = message.split("\n")[0];
        StompCommand command = Auxiliary.stringToCommand(commandtmp);
        if (command == null){
            System.out.println("PROTOCOL: command is null");
            //reciept id -1 indicates an invalid frame structure
            ServerFrameError error = new ServerFrameError("Invalid Command", -1, message);
            connections.send(connectionId, error.toString());
        }
        else {
            System.out.println("PROTOCOL: command is not null");
        }
        // check client frame structure: header names, null char, body, etc.
        ServerFrameError error = Auxiliary.validateClientFrame(command, message);
        // malformed frame, disconnect the client due to protocol violation
        if (error != null){
            System.out.println("PROTOCOL: client frame isn't valid according to Auxiliary.validateClientFrame");
            System.out.println("PROTOCOL: error is:\n" + error.toString());
            connections.send(connectionId, error.toString());
            shouldTerminate = true;
            connections.disconnect(connectionId);
        }
        else {// valid frame
            System.out.println("PROTOCOL: client frame is valid according to Auxiliary.validateClientFrame");
            ClientFrame clientFrame = Auxiliary.chooseClientFrame(command, message); 
            System.out.println("PROTOCOL: clientFrame:\n" + clientFrame.toString());
            //case of disconnect
            if (clientFrame instanceof ClientFrameDisconnect){
                clientFrame.process(connectionId, connections, this);
                connections.disconnect(connectionId);
                this.shouldTerminate = true;
                System.out.println("PROTOCOL: clientFrame is instance of ClientFrameDisconnect");
            }
            else{
                ServerFrame serverFrame = clientFrame.process(connectionId, connections, this);
                if (serverFrame != null){ // null for client frame send
                    System.out.println("PROTOCOL: serverFrame:\n" + serverFrame.toString());
                    connections.send(connectionId, serverFrame.toString());
                    System.out.println("PROTOCOL: serverFrame sent to client from process, first send funtion in Connections"); 
                    if (serverFrame instanceof ServerFrameError){
                        System.out.println("PROTOCOL: error frame, should terminate");
                        shouldTerminate = true;
                        connections.disconnect(connectionId);
                        connections.removeClient(connectionId);
                    }
            }}}}
        

	@Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }

    @Override
    public void setHandler(ConnectionHandler<String> handler) {
        this.handler = handler;
    }

    public boolean Crushed() {
        return this.crushed;
    }

    @Override
    public void addClient(){
        connections.addClient(this.connectionId, this.handler);
    }

}

    