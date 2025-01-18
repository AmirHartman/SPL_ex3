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

    public ClientFrameConnect(String toFrame){
        super(toFrame);
        String[] lines = toFrame.split("\n");
        // initialize headers
        for (int i = 1; i < lines.length && lines[i] != "\u0000"; i++){
            String[] header = lines[i].split(":");
            switch (header[0]){
                case "login":
                    this.login = header[1];
                case "passcode":
                    this.passcode = header[1];
            }
        }
    }


    @Override
    public void process (String string, Connections <T> connections){

    }
}

