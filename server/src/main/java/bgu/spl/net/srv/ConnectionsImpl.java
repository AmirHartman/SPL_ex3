package bgu.spl.net.srv;

public class ConnectionsImpl<T> implements Connections<T> {
   
    @Override
    public boolean send(int connectionId, T msg) {
        return false;
    }

    @Override
    public void send (String channel, T msg) {

    }

    @Override
    public void disconnect(int connectionId) {

    }

    
    
}
