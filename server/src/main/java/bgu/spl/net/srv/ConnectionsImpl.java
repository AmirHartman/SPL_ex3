package bgu.spl.net.srv;

import java.util.HashMap;
import java.util.LinkedList;

import bgu.spl.net.impl.stomp.Frame.ClientFrameConnect;

import java.util.AbstractMap.SimpleEntry;

public class ConnectionsImpl<T> implements Connections<T> {
   //צריך תומך מקביליות?
    private HashMap <String, LinkedList<Integer>> topics = new HashMap<>();
    private HashMap <Integer, ConnectionHandler<T>> connectionHandlerId = new HashMap<>();
    private HashMap <String, SimpleEntry<String, ConnectionHandler<T>>> userNames = new HashMap<>();

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
        LinkedList<Integer> channelSubscribers = topics.get(channel);
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
    public boolean isConnected(int connectionId) {
        return connectionHandlerId.containsKey(connectionId);
    }

    @Override
    public boolean correctPassword(String username, String password) {
        if (userNames.containsKey(username)) {
            SimpleEntry<String, ConnectionHandler<T>> user = userNames.get(username);
            return user.getKey().equals(password);
        }
        return false;
    }

    @Override
    public void connectClient(int connectionId, ConnectionHandler<T> handler, ClientFrameConnect connectFrame){
        if (!isConnected(connectionId)) {
            connectionHandlerId.put(connectionId, handler);
            userNames.putIfAbsent(connectFrame.getUsername(), new SimpleEntry<>(connectFrame.getPasscode(), handler));
        }
    }

    // // צריך??
    // protected HashMap <String, SimpleEntry<String, ConnectionHandler<T>>> getUserNames() {
    //     return userNames;
    // }
    
}
