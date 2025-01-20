
package bgu.spl.net.srv;

import java.util.concurrent.ConcurrentHashMap;


import bgu.spl.net.impl.stomp.Frame.ClientFrameConnect;

import java.util.AbstractMap.SimpleEntry;
import java.util.concurrent.ConcurrentLinkedQueue;

public class ConnectionTemp<T> implements Connections<T> {
    private ConcurrentHashMap <String, ConcurrentLinkedQueue<Integer>> topics = new ConcurrentHashMap<>();
    private ConcurrentHashMap <Integer, ConnectionHandler<T>> connectionHandlerId = new ConcurrentHashMap<>();
    private ConcurrentHashMap <String, SimpleEntry<String, ConnectionHandler<T>>> userNames = new ConcurrentHashMap<>();
    private ConcurrentHashMap <Integer, SimpleEntry<Integer, String>> subscriptionsID = new ConcurrentHashMap<>();
    
    
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



}