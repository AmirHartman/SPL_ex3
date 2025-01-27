// package bgu.spl.net.impl.echo;

import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionHandler;

// import java.io.Serializable;
// import java.time.LocalDateTime;

// public class EchoProtocol implements MessagingProtocol<String> {

//     private boolean shouldTerminate = false;

//     @Override
//     public void start(int connectionId, Connections<String> connections) {
//         // do nothing
//     }

    @Override
    public void process(String msg) {
        shouldTerminate = "bye".equals(msg);
        System.out.println("[" + LocalDateTime.now() + "]: " + msg);
        createEcho(msg) ;
        // return createEcho(msg);
    }

//     private String createEcho(String message) {
//         String echoPart = message.substring(Math.max(message.length() - 2, 0), message.length());
//         return message + " .. " + echoPart + " .. " + echoPart + " ..";
//     }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }

    @Override
    public void setHandler(ConnectionHandler<String> handler) {
        // do nothing
    }

    @Override
    public void addClient(){
        // do nothing
    }

}
