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
            //reciept id -1 indicates an invalid frame structure
            ServerFrameError error = new ServerFrameError("Invalid Command", -1, message);
            connections.send(connectionId, error.toString());
        }
        // check client frame structure: header names, null char, body, etc.
        ServerFrameError error = Auxiliary.validateClientFrame(command, message);
        // malformed frame, disconnect the client due to protocol violation
        if (error != null){
            connections.send(connectionId, error.toString());
            shouldTerminate = true;
            connections.disconnect(connectionId);
        }
        else {// valid frame
            ClientFrame clientFrame = Auxiliary.chooseClientFrame(command, message); 
            //case of disconnect
            if (clientFrame instanceof ClientFrameDisconnect){
                clientFrame.process(connectionId, connections, this);
                connections.disconnect(connectionId);
                this.shouldTerminate = true;
            }
            else{
                ServerFrame serverFrame = clientFrame.process(connectionId, connections, this);
                if (serverFrame != null){ // should never happen
                    connections.send(connectionId, serverFrame.toString());
                    if (serverFrame instanceof ServerFrameError){
                        shouldTerminate = true;
                        connections.disconnect(connectionId);
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

    @Override
    public void addClient(){
        connections.addClient(this.connectionId, this.handler);
    }

    @Override
    public void close(){
        connections.disconnect(this.connectionId);
    }


}

    