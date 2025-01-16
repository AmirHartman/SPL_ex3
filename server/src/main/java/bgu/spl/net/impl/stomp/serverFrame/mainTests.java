package bgu.spl.net.impl.stomp.serverFrame;

import bgu.spl.net.impl.stomp.StompEncoderDecoder;

public class mainTests {
    public static void main(String[] args) {
        ServiceFrameConnected connected = new ServiceFrameConnected();
        String tst = connected.toString();
        System.out.print(tst);
        ServiceFrameReceipt receipt = new ServiceFrameReceipt("1");
        System.out.print(receipt.toString());
        ServiceFrameError error = new ServiceFrameError("this is a test", 2345, "this is a pretty long message to est the toString method of error");
        System.out.print(error.toString());
        ServiceFrameMessage message = new ServiceFrameMessage(1, 1, "popo", "this is a test message");
        System.out.print(message.toString());

        StompEncoderDecoder encdec = new StompEncoderDecoder();
        byte[] tst2 = encdec.encode(message.toString());
        System.out.println(message.toString());
        for (int i = 0; i < tst2.length - 1 ; i++) {
            encdec.decodeNextByte(tst2[i]);
        }
        System.out.print(encdec.decodeNextByte(tst2[tst2.length - 1]));
    }   
}


