package bgu.spl.net.impl.stomp;

import java.util.concurrent.ConcurrentHashMap;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.impl.stomp.ServerFrame.*;


public class StompMessagingProtocolImpl implements StompMessagingProtocol<String>{
    private boolean shouldTerminate = false;
    //צריך מקביליות? אולי בריאקטור?
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
        System.out.println("Entering process for message: " + message);
        String commandtmp = message.split("\n")[0];
        StompCommand command = Auxiliary.stringToCommand(commandtmp);
        if (command == null){
            System.out.println("command is null");
            //reciept id -1 indicates an invalid frame structure
            ServerFrameError error = new ServerFrameError("Invalid Command", -1, message);
            connections.send(connectionId, error.toString());
        }
        else {
            System.out.println("command is not null");
        }
        // check client frame structure: header names, null char, body, etc.
        ServerFrameError error = Auxiliary.validateClientFrame(command, message);
        // malformed frame, disconnect the client due to protocol violation
        if (error != null){
            System.out.println("client frame isn't valid according to Auxiliary.validateClientFrame");
            System.out.println("error is:\n" + error.toString());
            connections.send(connectionId, error.toString());
            shouldTerminate = true;
            connections.disconnect(connectionId);
        }
        else {// valid frame
            System.out.println("client frame is valid according to Auxiliary.validateClientFrame");
            ClientFrame clientFrame = Auxiliary.chooseClientFrame(command, message); 
            System.out.println("clientFrame:\n" + clientFrame.toString());
            //case of disconnect
            if (clientFrame instanceof ClientFrameDisconnect){
                clientFrame.process(connectionId, connections, handler, this);
                connections.disconnect(connectionId);
                this.shouldTerminate = true;
                System.out.println("clientFrame is instance of ClientFrameDisconnect");
            }
            else{
                ServerFrame serverFrame = clientFrame.process(connectionId, connections, handler, this);
                System.out.println("serverFrame:\n" + serverFrame.toString());
                connections.send(connectionId, serverFrame.toString());
                System.out.println("serverFrame sent to client from process, first send funtion in Connections"); 
                if (serverFrame instanceof ServerFrameError){
                    System.out.println("error frame, should terminate");
                    shouldTerminate = true;
                    connections.disconnect(connectionId);
                }
            }}}
        

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

    