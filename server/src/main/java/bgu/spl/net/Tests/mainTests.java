
package bgu.spl.net.Tests;

import java.net.Socket;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.impl.stomp.StompEncoderDecoder;
import bgu.spl.net.impl.stomp.StompMessagingProtocolImpl;
import bgu.spl.net.srv.ConnectionsImpl;
import bgu.spl.net.srv.BlockingConnectionHandler;


public class mainTests {
    // test the toString method of the serverFrame classes
    public static void main(String[] args) {
        String test = "CONNECT\naccept-version:1.2\nhost:stomp.cs.bgu.ac.il\nlogin:yam\npasscode:123\n\n\u0000";
        System.out.println(test);


        ConnectionsImpl<String> connections = new ConnectionsImpl<>();
        MessagingProtocol<String> protocol = new StompMessagingProtocolImpl();
        protocol.start(1, connections);
        MessageEncoderDecoder<String> encdec = new StompEncoderDecoder();
        Socket sock = new Socket();

        BlockingConnectionHandler<String> handler = new BlockingConnectionHandler<>(sock, encdec, protocol);

        protocol.process(test);


        }

    }

