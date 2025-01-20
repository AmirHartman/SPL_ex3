package bgu.spl.net.impl.stomp;

import java.security.Provider.Service;
import java.util.concurrent.ConcurrentLinkedDeque;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.impl.stomp.Frame.*;


public class StompMessagingProtocolImplTemp<String> implements StompMessagingProtocol<String>{
    private boolean shouldTerminate = false;
    int connectionId;
    private Connections<String> connections;


    @Override
    public void start(int connectionId, Connections<String> connections){
        this.connectionId = connectionId;
        this.connections = connections;
    }
    

    public void process(String message){

   }
	
   public boolean shouldTerminate(){
    return shouldTerminate;
   }


}

    