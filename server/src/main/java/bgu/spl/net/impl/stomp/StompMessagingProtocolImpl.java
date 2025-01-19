package bgu.spl.net.impl.stomp;

import java.security.Provider.Service;
import java.util.concurrent.ConcurrentLinkedDeque;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.impl.stomp.Frame.*;

public class StompMessagingProtocolImpl<T> implements StompMessagingProtocol<T>{
    
    private boolean shouldTerminate = false;
    int connectionId;
    private Connections<T> connections;
    private ConcurrentLinkedDeque<T> messages = new ConcurrentLinkedDeque<>();


    @Override  
    /**
	 * Used to initiate the current client protocol with it's personal connection ID and the connections implementation
	**/ 
    public void start(int connectionId, Connections<T> connections) {
        this.connectionId = connectionId;
        this.connections = connections;
    }

    @Override
    public void process(Object message) {
        String msg = (String) message;
        String type = msg.split("\n")[0];
        shouldTerminate = (type.equals("DISCONNECT") | type.equals("ERROR"));
        // do nothing
    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }

    // protected String getAnswer(){
    //     ServiceFrame frame =(ServiceFrame) messages.pollFirst();
    //     if (frame != null){
    //         return frame.toString();
    //     }
    //     return null;
    // }
}
