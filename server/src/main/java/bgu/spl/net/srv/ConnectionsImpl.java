package bgu.spl.net.srv;

import java.util.concurrent.ConcurrentHashMap;


import bgu.spl.net.impl.stomp.Frame.ClientFrameConnect;

import java.util.AbstractMap.SimpleEntry;
import java.util.concurrent.ConcurrentLinkedQueue;

public class ConnectionsImpl<T> implements Connections<T> {
   //צריך תומך מקביליות?
    private ConcurrentHashMap <String, ConcurrentLinkedQueue<Integer>> topics = new ConcurrentHashMap<>();
    private ConcurrentHashMap <Integer, ConnectionHandler<T>> connectionHandlerId = new ConcurrentHashMap<>();
    private ConcurrentHashMap <String, SimpleEntry<String, ConnectionHandler<T>>> userNames = new ConcurrentHashMap<>();

    @Override
    public boolean send(int connectionId, T msg) {
        if (connectionHandlerId.containsKey(connectionId)) {
            ConnectionHandler<T> connectionHandler = connectionHandlerId.get(connectionId);
            if (connectionHandler != null) {
                
                connectionHandler.send(msg);
                return true;    
            }
        }
        return false;
    }

    @Override
    public void send (String channel, T msg) {
        if (!topics.containsKey(channel)) {
            return;
        }
        ConcurrentLinkedQueue<Integer> channelSubscribers = topics.get(channel);
        for (Integer connectionId : channelSubscribers) {
            send(connectionId, msg);
        }
    }

    // מתי סוגרים את הסוקט? צריך לחכות שהמשימות יושלמו ואז לסגור
    @Override
    public void disconnect(int connectionId) {
        for (String channel : topics.keySet()) {
            topics.get(channel).remove(connectionId);
        }
        if (connectionHandlerId.containsKey(connectionId)) {
            String connectionHandlerUserName = connectionHandlerId.get(connectionId).getUserName();
            if (connectionHandlerUserName != null) {
                // remove the refference to connectionHandler from UserNames
                userNames.get(connectionHandlerUserName).setValue(null);
            }                
        }
        connectionHandlerId.remove(connectionId);
    }

    @Override
    public boolean isConnected(String username) {
        return userNames.get(username).getValue() != null;
    }

    @Override
    public boolean correctPassword(String username, String password) {
        if (userNames.containsKey(username)) {
            SimpleEntry<String, ConnectionHandler<T>> user = userNames.get(username);
            return user.getKey().equals(password);
        }
        return true;
    }

    @Override
    public boolean connectClient(int connectionId, ConnectionHandler<T> handler, ClientFrameConnect connectFrame){
        synchronized (userNames){
            if (!userNames.containsKey(connectFrame.getUsername())) {
                userNames.put(connectFrame.getUsername(), new SimpleEntry<>(connectFrame.getPasscode(), handler));
                connectionHandlerId.put(connectionId, handler);
                return true;
            } else {
                SimpleEntry<String, ConnectionHandler<T>> user = userNames.get(connectFrame.getUsername());
                if (user.getValue() != null) {
                    return false;
                } else {
                    user.setValue(handler);
                    connectionHandlerId.put(connectionId, handler);
                    return true;
                }
        }}}

    @Override
    public void subscribeClient(int connectionId, String topic, ConnectionHandler<T> handler){
        topics.putIfAbsent(topic, new ConcurrentLinkedQueue<>());
        topics.get(topic).add(connectionId);
    }

    // // צריך??
    // protected HashMap <String, SimpleEntry<String, ConnectionHandler<T>>> getUserNames() {
    //     return userNames;
    // }
    
}
