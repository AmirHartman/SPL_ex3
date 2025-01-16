package bgu.spl.net.srv;

public class ConnectionsImpl<T> implements Connections<T> {
   
    private int connectionsCounter = 0;


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

    @Override
    public int getConnectionsCounter() {
        return connectionsCounter;
    }

    @Override
    public void getTheIntcrementConnectionsCounter() {
        connectionsCounter++;
    }
    
    
}
