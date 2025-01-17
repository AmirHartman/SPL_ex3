package bgu.spl.net.srv;

// import java.io.IOException;
// import java.util.AbstractMap.SimpleEntry;
// import java.util.HashMap;

public interface Connections<T> {

    boolean send(int connectionId, T msg);

    void send(String channel, T msg);

    void disconnect(int connectionId);

}