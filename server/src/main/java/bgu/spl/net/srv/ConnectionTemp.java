
package bgu.spl.net.srv;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;



import java.util.AbstractMap.SimpleEntry;
import java.util.concurrent.ConcurrentLinkedQueue;

public class ConnectionTemp<T> implements Connections<T> {
    private ConcurrentHashMap <String, ConcurrentLinkedQueue<Integer>> topics = new ConcurrentHashMap<>();
    private ConcurrentHashMap <Integer, SimpleEntry<String, ConnectionHandler<T>>> connectionsIds = new ConcurrentHashMap<>();
    // username as key, value is a pair of password and boolean if the user is connected
    private ConcurrentHashMap <String, SimpleEntry<String, Boolean>> users = new ConcurrentHashMap<>();
    private AtomicInteger messageIdCounter = new AtomicInteger(0);
    
    
    @Override
    public boolean send(int connectionId, T msg){
        return true;
    }
    @Override
    public void send(String channel, T msg){

    }

    @Override
    public void disconnect(int connectionId){

    }

    @Override
    public boolean isConnected(String username){
        if (users.containsKey(username)){
            synchronized (users.get(username)){
                return users.get(username).getValue();
            }
        }
        return false;
    }

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
    public boolean connect(int connectionId, ConnectionHandler<T> handler, String username, String password){
        if (users.containsKey(username)){
            synchronized (users.get(username)){
                SimpleEntry<String, Boolean> user = users.get(username);
                // user is already logged in
                if (user.getValue()){
                    return false;
                } // password correctness is checked in protocol
                user.setValue(true);
            }}
        else {
            users.put(username, new SimpleEntry<>(password, true));
        }
        connectionsIds.put(connectionId, new SimpleEntry<>(username, handler));
        return true;
    }

    @Override
    public void subscribe(int connectionId, String topic){
        topics.putIfAbsent(topic, new ConcurrentLinkedQueue<>());
        synchronized (topics.get(topic)){
            topics.get(topic).add(connectionId);
        }}

    @Override
    public boolean unsubscribe(int connectionId, String topic){
        if (topics.containsKey(topic)){
            synchronized (topics.get(topic)){
                topics.get(topic).remove(connectionId);
                return true;
            }
        }// topic doesn't exist, thus unable to unsubscribe user
        return false;
    }

    @Override
    public int getNextMessageId(){
        return messageIdCounter.getAndIncrement();
    }




}