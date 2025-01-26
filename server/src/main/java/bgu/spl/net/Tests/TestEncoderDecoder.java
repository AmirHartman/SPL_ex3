package bgu.spl.net.Tests;
import bgu.spl.net.impl.stomp.StompEncoderDecoder;
import bgu.spl.net.impl.stomp.ClientFrame;
import bgu.spl.net.impl.stomp.ClientFrameConnect;

public class TestEncoderDecoder {
    public static void main(String[] args) {
        // create a test frame and convet it to string
        ClientFrame testFrame = new ClientFrameConnect("yam", "1234");
        String message = testFrame.toString();

        // test encoder
        StompEncoderDecoder encdec = new StompEncoderDecoder();
        byte[] tst2 = encdec.encode(message.toString());

        // test decoder (should print the same message twice)
        System.out.println(message.toString());
        for (int i = 0; i < tst2.length - 1 ; i++) {
            encdec.decodeNextByte(tst2[i]);
        }
        System.out.println(encdec.decodeNextByte(tst2[tst2.length - 1]));
    }
    
}
