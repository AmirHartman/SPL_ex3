package bgu.spl.net.srv;


public interface Connections<T> {

    boolean send(int connectionId, T msg);

    void send(String channel, T msg);

    void disconnect(int connectionId);

    boolean isConnected(String username);

    boolean correctPassword(String username, String password);

    boolean connect(int connectionId, ConnectionHandler<T> handler, String username, String password);

    void subscribe(int connectionId, String topic);

    boolean unsubscribe(int connectionId, String topic);

    int getNextMessageId();

}