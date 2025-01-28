package bgu.spl.net.Tests;


import java.net.Socket;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.impl.stomp.*;
import bgu.spl.net.srv.BlockingConnectionHandler;
import bgu.spl.net.srv.ConnectionsImpl;

public class TestProcessInTerminal{
    
    public static void main(String[] args) {
        String connect1 = "CONNECT\naccept-version:1.2\nhost:stomp.cs.bgu.ac.il\nlogin:yam\npasscode:123\n\n\u0000";
        System.out.println(connect1);
        String connect2 = "CONNECT\naccept-version:1.2\nhost:stomp.cs.bgu.ac.il\nlogin:amir\npasscode:123\n\n\u0000";


        ConnectionsImpl<String> connections = new ConnectionsImpl<>();
        MessagingProtocol<String> protocol = new StompMessagingProtocolImpl();
        protocol.start(1, connections);
        MessageEncoderDecoder<String> encdec = new StompEncoderDecoder();
        Socket sock = new Socket();

        BlockingConnectionHandler<String> handler = new BlockingConnectionHandler<>(sock, encdec, protocol);

        protocol.process(connect1);
        protocol.process(connect2);

        try {
            handler.close();
        } catch (Exception e) {
            e.printStackTrace();
        }

    }
}