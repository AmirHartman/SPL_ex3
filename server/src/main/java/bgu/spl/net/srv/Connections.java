package bgu.spl.net.srv;

import java.util.concurrent.ConcurrentHashMap;


public interface Connections<T> {

    boolean send(int connectionId, T msg);

    void send(String channel, T msg);

    void disconnect(int connectionId);

    boolean isConnected(int connectionId);

    boolean correctPassword(String username, String password);

    void addClient (int connectionId, ConnectionHandler<T> handler);
    
    void removeClient (int connectionId);

    boolean connect(int connectionId, String username, String password);

    void subscribe(int connectionId, String topic, int subscriptionId);

    boolean unsubscribe(int connectionId, String topic);

    int getNextMessageId();

    int getSubscriptionId(String topic, int connectionId);

    ConcurrentHashMap<Integer, Integer> getSubscribers(String topic);

    //טסטים
    ConcurrentHashMap<String, ConcurrentHashMap<Integer, Integer>> getTopics();
    ConcurrentHashMap<String, Integer> getUsers();
    ConcurrentHashMap<String, String> getPasswords();
    ConcurrentHashMap<Integer, ConnectionHandler<T>> getHandlers();
    
}