package bgu.spl.net.impl.stomp;

// import java.util.AbstractMap.SimpleEntry;
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
                shouldTerminate = true;
            }
                ServerFrame serverFrame = clientFrame.process(connectionId, connections, handler, this);
                connections.send(connectionId, serverFrame.toString());
            }}

	@Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }

//    public boolean shouldTerminate(ClientFrame clientFrame, ServerFrame serverFrame){
//          if (clientFrame instanceof ClientFrameDisconnect){
//               shouldTerminate = true;
//          }
//          if (clientFrame instanceof ClientFrameConnect & serverFrame instanceof ServerFrameError){

//          }
//     return shouldTerminate;
//    }

    @Override
    public void setHandler(ConnectionHandler<String> handler) {
        this.handler = handler;
    }

    public boolean Crushed() {
        return this.crushed;
    }
}

    