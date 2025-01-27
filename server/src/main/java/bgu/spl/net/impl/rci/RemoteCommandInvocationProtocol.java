// package bgu.spl.net.impl.rci;

import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionHandler;

import java.io.Serializable;

// public class RemoteCommandInvocationProtocol<T> implements MessagingProtocol<Serializable> {

//     private T arg;

    public RemoteCommandInvocationProtocol(T arg) {
        this.arg = arg;
    }

    @Override
    public void start(int connectionId, Connections<Serializable> connections) {
        // nothing
    }

    @Override
    // public Serializable process(Serializable msg) {
    public void process(Serializable msg) {
        // return ((Command<T>) msg).execute(arg);
    }

    @Override
    public boolean shouldTerminate() {
        return false;
    }

    @Override
    public void setHandler(ConnectionHandler<Serializable> handler) {
        // nothing
    }

    @Override
    public void addClient(){
        // do nothing
    }




}
