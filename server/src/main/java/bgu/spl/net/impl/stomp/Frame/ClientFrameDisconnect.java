package bgu.spl.net.impl.stomp.Frame;

import bgu.spl.net.srv.Connections;

public class ClientFrameDisconnect<T> extends ClientFrame<T> {
    private String acceptVersion = "1.2";
    private String host = "stomp.cs.bgu.ac.il";
    private String login;
    private String passcode;


    @Override
    public void process (String string, Connections <T> connections){

    }
}