package bgu.spl.net.Tests;

import bgu.spl.net.impl.stomp.StompEncoderDecoder;
import bgu.spl.net.impl.stomp.Frame.ServiceFrameConnected;
import bgu.spl.net.impl.stomp.Frame.ServiceFrameError;
import bgu.spl.net.impl.stomp.Frame.ServiceFrameMessage;
import bgu.spl.net.impl.stomp.Frame.ServiceFrameReceipt;
import bgu.spl.net.impl.stomp.Frame.ClientFrame;
import bgu.spl.net.impl.stomp.Frame.ClientFrameConnect;
import bgu.spl.net.impl.stomp.Frame.ServiceFrame;

public class TestServerFrame {
    public static void main(String[] args) {

        ServiceFrameConnected connected = new ServiceFrameConnected();
        String tst = connected.toString();
        System.out.println(tst);
        ServiceFrameReceipt receipt = new ServiceFrameReceipt(1);
        System.out.println(receipt.toString());
        ServiceFrameError error = new ServiceFrameError("this is a test", 2345, "this is a pretty long message to est the toString method of error");
        System.out.println(error.toString());
        ServiceFrameMessage message = new ServiceFrameMessage(1, 1, "popo", "this is a test message");
        System.out.println(message.toString());

        // tests the encoderdecoder
        StompEncoderDecoder encdec = new StompEncoderDecoder();
        byte[] tst2 = encdec.encode(message.toString());
        System.out.println(message.toString());
        for (int i = 0; i < tst2.length - 1 ; i++) {
            encdec.decodeNextByte(tst2[i]);
        }
        System.out.println(encdec.decodeNextByte(tst2[tst2.length - 1]));

        // // test process in Frame classes
        // ServiceFrameMessage message1 = new ServiceFrameMessage(message.toString());
        // System.out.println(message1.toString());




}
}
