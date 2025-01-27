
package bgu.spl.net.srv;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;



import java.util.AbstractMap.SimpleEntry;

public class ConnectionsImpl<T> implements Connections<T> {
    // topic as key, value is a hash map of connectionId as key and their subscription id as value
    private ConcurrentHashMap <String, ConcurrentHashMap<Integer, Integer>> topics = new ConcurrentHashMap<>();
    // connectionId as key, value is a pair of username and connectionHandler
    private ConcurrentHashMap <Integer, SimpleEntry<String, ConnectionHandler<T>>> connectionsIds = new ConcurrentHashMap<>();
    // username as key, value is a pair of password and boolean if the user is connected
    private ConcurrentHashMap <String, SimpleEntry<String, Boolean>> users = new ConcurrentHashMap<>();
    private AtomicInteger messageIdCounter = new AtomicInteger(0);
    
    
    @Override
    public boolean send(int connectionId, T msg){
        if (connectionsIds.containsKey(connectionId)){
            synchronized (connectionsIds.get(connectionId)){
                connectionsIds.get(connectionId).getValue().send(msg);
                return true;
        }}
        return false;
    }

    @Override
    public void send(String channel, T msg){
        synchronized(topics.get(channel)){
            for (Integer connectionId : topics.get(channel).keySet()){
                if (connectionsIds.get(connectionId) != null){
                connectionsIds.get(connectionId).getValue().send(msg);
                }
            }
        }
            

    }

    @Override
    public void disconnect(int connectionId){
        // disconnect user from the system
        SimpleEntry<String, ConnectionHandler<T>> user = null;
        if (connectionsIds.containsKey(connectionId)){
        synchronized (connectionsIds.get(connectionId)){
        user = connectionsIds.remove(connectionId);
        }}if (user != null && user.getKey() != null){
            synchronized (users.get(user.getKey())){
                users.get(user.getKey()).setValue(false);
                // unsubscribe user from all topics
                for (String topic : topics.keySet()){
                    synchronized (topics.get(topic)){
                        topics.get(topic).remove(connectionId);
                    }
                }
            }
        }
    }

    // @Override
    // public boolean isConnected(int connectionId){
    //     return connectionsIds.containsKey(connectionId);
    // }

    @Override
    public boolean correctPassword(String username, String password){
        if (users.containsKey(username)){
            synchronized (users.get(username)){
                return users.get(username).getKey().equals(password);
            }
        } // user doesn't exist, thus password is irrelevant
        return true;
    }

    @Override
    public void addClient (int connectionId, ConnectionHandler<T> handler){
        connectionsIds.put(connectionId, new SimpleEntry<>(null, handler));
    }

    @Override
    public boolean connect(int connectionId, ConnectionHandler<T> handler, String username, String password){
        if (users.containsKey(username)){
            synchronized (users.get(username)){
                SimpleEntry<String, Boolean> user = users.get(username);
                // user is already logged in
                if (user.getValue()){
                    return false;
                } // password correctness is checked before hand
                user.setValue(true);
            }}
        else {
            users.put(username, new SimpleEntry<>(password, true));
        }// update the username (handler is already registered)
        connectionsIds.put(connectionId, new SimpleEntry<>(username, handler));
        return true;
    }



    @Override
    public void subscribe(int connectionId, String topic, int subscriptionId){
        topics.putIfAbsent(topic, new ConcurrentHashMap<>());
        synchronized (topics.get(topic)){
            topics.get(topic).put(connectionId, subscriptionId);
        }}

    @Override
    public boolean unsubscribe(int connectionId, String topic){
        if (topics.containsKey(topic)){
            synchronized (topics.get(topic)){
                ConcurrentHashMap<Integer, Integer> topicMap = topics.get(topic);
                if (topicMap.remove(connectionId) != null){
                    return true;
                }}
        }// topic doesn't exist, thus unable to unsubscribe user
        return false;
    }

    @Override
    public int getNextMessageId(){
        return messageIdCounter.getAndIncrement();
    }

    @Override
    public int getSubscriptionId(String topic, int connectionId){
        if (topics.containsKey(topic)){
            synchronized (topics.get(topic)){
                ConcurrentHashMap<Integer, Integer> topicMap = topics.get(topic);
                if (topicMap.containsKey(connectionId)){
                    return topicMap.get(connectionId);
                }}
        }// topic doesn't exist, thus unable to get subscription id
        return -1;
    }

    @Override
    public ConcurrentHashMap<Integer, Integer> getSubscribers(String topic){
        return topics.get(topic);
    }








}