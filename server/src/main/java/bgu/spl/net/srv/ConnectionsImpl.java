
package bgu.spl.net.srv;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

public class ConnectionsImpl<T> implements Connections<T> {
    // topic as key, value is a hash map of connectionId as key and their subscription id as value
    private ConcurrentHashMap <String, ConcurrentHashMap<Integer, Integer>> topics = new ConcurrentHashMap<>();
    // key is connection ID
    private ConcurrentHashMap <Integer, ConnectionHandler<T>> handlers = new ConcurrentHashMap<>();
    // key is username - indicates whether user is connected or not
    private ConcurrentHashMap <String, Integer> users = new ConcurrentHashMap<>();
    private ConcurrentHashMap <String, String> passwords = new ConcurrentHashMap<>();

    private AtomicInteger messageIdCounter = new AtomicInteger(0);
    
    
    @Override
    public boolean send(int connectionId, T msg){
        // synchronized topics so that a user cannot sign into a channel while sending a message to all subscribers
        synchronized (topics){
            synchronized(handlers){
                if (handlers.containsKey(connectionId)){
                    handlers.get(connectionId).send(msg);
                    return true;
            }}}
        return false;
    }
    @Override
    public void send(String channel, T msg){
        if (topics.get(channel) == null){
            return;
        }
        synchronized(topics){
            synchronized (topics.get(channel)){
            // iterate over all users subscribed to the channel
            for (Integer connectionId : topics.get(channel).keySet()){
                send(connectionId, msg);
                }
            }}}

    @Override
    public void disconnect(int connectionId){
            synchronized (users){
                for (String username : users.keySet()){
                    if (users.get(username) == connectionId){
                        users.remove(username);
                        break;
                    }
            }}
        handlers.remove(connectionId);
        synchronized (topics){
                for (String topic : topics.keySet()){
                    topics.get(topic).remove(connectionId);
            }}}   

    @Override
    public boolean isConnected(int connectionId){
        synchronized (users){
            for (String username : users.keySet()){
                if (users.get(username) == connectionId){
                    return true;
                }}
            return false;
    }}

    @Override
    public boolean correctPassword(String username, String password){
        synchronized (passwords){
            if (!passwords.containsKey(username)){
                return true;
            }
        return passwords.get(username).equals(password);
    }}

    @Override
    public void addClient (int connectionId, ConnectionHandler<T> handler){
        handlers.put(connectionId, handler);
    }

    @Override
    public boolean connect(int connectionId, String username, String password){
        // user is already connected
        if (users.containsKey(username)){
            return false;
        }// user isn't connected yet
        users.putIfAbsent(username, connectionId);
        passwords.putIfAbsent(username, password);
        return true;
    }

    @Override
    public void subscribe(int connectionId, String topic, int subscriptionId){
        synchronized (topics){
            topics.putIfAbsent(topic, new ConcurrentHashMap<>());
            if (!topics.get(topic).containsKey(connectionId)){
                topics.get(topic).put(connectionId, subscriptionId);
            }}}

    @Override
    public boolean unsubscribe(int connectionId, String topic){
        synchronized (topics){
            ConcurrentHashMap<Integer, Integer> subscribers = topics.get(topic);
            if (subscribers == null){
                return false;
        }// return false if the user isn't subscribed to the topic
        return subscribers.remove(connectionId) != null;
    }}

    @Override
    public int getNextMessageId(){
        return messageIdCounter.getAndIncrement();
    }

    @Override
    public int getSubscriptionId(String topic, int connectionId){
        synchronized (topics){
            if (topics.containsKey(topic)){
                synchronized (topics.get(topic)){
                    ConcurrentHashMap<Integer, Integer> topicMap = topics.get(topic);
                    if (topicMap.containsKey(connectionId)){
                        return topicMap.get(connectionId);
                }}}
        return -1;
    }}

    @Override
    public ConcurrentHashMap<Integer, Integer> getSubscribers(String topic){
        return topics.get(topic);
    }


}














