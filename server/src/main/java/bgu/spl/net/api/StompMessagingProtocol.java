package bgu.spl.net.api;

import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionHandler;

public interface StompMessagingProtocol<T>  {
	/**
	 * Used to initiate the current client protocol with it's personal connection ID and the connections implementation
     * מותר לשנות את חתימה כדי לאתחל עם האנדלר?
	**/
    void start(int connectionId, Connections<T> connections, ConnectionHandler<T> handler);
    
    void process(T message);
	
	/**
     * @return true if the connection should be terminated
     */
    boolean shouldTerminate();

    // private Connections<T> connections;

}
