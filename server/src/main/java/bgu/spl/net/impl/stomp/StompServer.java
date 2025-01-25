package bgu.spl.net.impl.stomp;

import java.util.function.Supplier;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.srv.*;
import java.util.concurrent.atomic.AtomicInteger;


public class StompServer {

    public static void main(String[] args) {
        if (args.length != 2) {
            System.out.println("you must supply two arguments: <port>, <type_of_server> - tpc / reactor");
            return;
        }
        ConnectionsImpl<String> connections = new ConnectionsImpl<>();
        AtomicInteger idCounter = new AtomicInteger(0);
        int port;
        try {
            port = Integer.parseInt(args[0]);
        } catch (Exception e) {
            System.out.println("you must supply on the first argument a valid port number");
            return;
        }
        if (args[1].equals("tpc")) {
            Server.threadPerClient(port,
                    (Supplier<MessagingProtocol<String>>) () -> {
                        MessagingProtocol<String> protocol = new StompMessagingProtocolImpl();
                        protocol.start(idCounter.getAndIncrement(), connections);
                        return protocol;
                    },
                    (Supplier<MessageEncoderDecoder<String>>) () -> new StompEncoderDecoder()
            ).serve();
        } else if (args[1].equals("reactor")) {
            Server.reactor(
                2, 
                port,
                (Supplier<MessagingProtocol<String>>) () -> {
                    MessagingProtocol<String> protocol = new StompMessagingProtocolImpl();
                    protocol.start(idCounter.getAndIncrement(), connections);
                    return protocol;
                },
                (Supplier<MessageEncoderDecoder<String>>) () -> new StompEncoderDecoder()
                ).serve();
        } else {
            System.out.println("you must supply on the second argument: <type_of_server> - tpc / reactor");
        }
    }
}
