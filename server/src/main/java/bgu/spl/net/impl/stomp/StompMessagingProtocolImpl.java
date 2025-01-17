package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Connections;

public class StompMessagingProtocolImpl<T> implements StompMessagingProtocol<T>{
    
    private boolean shouldTerminate = false;
    int connectionId;
    private Connections<T> connections;


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
        // do nothing
    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }
}
