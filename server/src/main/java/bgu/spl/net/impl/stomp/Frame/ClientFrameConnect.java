package bgu.spl.net.impl.stomp.Frame;

import bgu.spl.net.srv.Connections;

public class ClientFrameConnect<T> extends ClientFrame<T> {
    private String acceptVersion = "1.2";
    private String host = "stomp.cs.bgu.ac.il";
    private String login;
    private String passcode;

    public ClientFrameConnect(String login, String passcode) {
        super(ServiceStompCommand.CONNECT);
        this.login = login;
        this.passcode = passcode;
    }

    @Override
    public void process (String string, Connections <T> connections){

    }
}

