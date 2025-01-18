package bgu.spl.net.srv;

import bgu.spl.net.impl.stomp.Frame.ClientFrameConnect;

// import java.io.IOException;
// import java.util.AbstractMap.SimpleEntry;
// import java.util.HashMap;

public interface Connections<T> {

    boolean send(int connectionId, T msg);

    void send(String channel, T msg);

    void disconnect(int connectionId);

    boolean isConnected(int connectionId);

    boolean correctPassword(String username, String password);

    void connectClient(int connectionId, ConnectionHandler<T> handler, ClientFrameConnect connectFrame);

    void subscribeClient(int connectionId, String topic, ConnectionHandler<T> handler);

}