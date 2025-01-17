package bgu.spl.net;

import bgu.spl.net.impl.stomp.StompEncoderDecoder;
import bgu.spl.net.impl.stomp.serverFrame.ServiceFrameConnected;
import bgu.spl.net.impl.stomp.serverFrame.ServiceFrameError;
import bgu.spl.net.impl.stomp.serverFrame.ServiceFrameMessage;
import bgu.spl.net.impl.stomp.serverFrame.ServiceFrameReceipt;

public class mainTests {
    // test the toString method of the serverFrame classes
    public static void main(String[] args) {
        // ServiceFrameConnected connected = new ServiceFrameConnected();
        // String tst = connected.toString();
        // System.out.println(tst);
        // ServiceFrameReceipt receipt = new ServiceFrameReceipt(1);
        // System.out.println(receipt.toString());
        // ServiceFrameError error = new ServiceFrameError("this is a test", 2345, "this is a pretty long message to est the toString method of error");
        // System.out.println(error.toString());
        ServiceFrameMessage message = new ServiceFrameMessage(1, 1, "popo", "this is a test message");
        System.out.println(message.toString());

        // tests the encoderdecoder
        // StompEncoderDecoder encdec = new StompEncoderDecoder();
        // byte[] tst2 = encdec.encode(message.toString());
        // System.out.println(message.toString());
        // for (int i = 0; i < tst2.length - 1 ; i++) {
        //     encdec.decodeNextByte(tst2[i]);
        // }
        // System.out.println(encdec.decodeNextByte(tst2[tst2.length - 1]));

        // test process in Frame classes
        ServiceFrameMessage message1 = new ServiceFrameMessage(message.toString());
        System.out.println(message1.toString());

        

        //test connection implementation
        // ConnectionsImpl<String> connections = new ConnectionsImpl<>();


    }

    
}


