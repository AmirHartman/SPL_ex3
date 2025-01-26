package bgu.spl.net.impl.stomp;
import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.impl.stomp.ServerFrame.ServerFrameError;

// import java.nio.charset.StandardCharsets;
import java.util.Arrays;



public class StompEncoderDecoder implements MessageEncoderDecoder<String> {
    private byte[] bytes = new byte[1 << 10]; //start with 1k
    private int len = 0;

    @Override
    public String decodeNextByte(byte nextByte) {
        // לשנות ל-==0?
        //malformed frame: no null char at the end of the frame
        if (bytes[bytes.length-1] != '\u0000'){
            ServerFrameError error =  new ServerFrameError("no null char at the end of the frame", -1, "malformed frame:\n" +
                                                                                                "no null char at the end of the frame\n" +
                                                                                                "thus, no way do decode message,\n" +
                                                                                                "no null char at the end of the frame.\n" +
                                                                                                "error in StompEncoderDecoder");
            return error.toString();
        }
        if (nextByte == '\u0000') {
            return popString();
        }

        pushByte(nextByte);
        return null; //not a line yet
    }

    @Override   
    public byte[] encode(String message) {
        return (message).getBytes();
    }

    private void pushByte(byte nextByte) {
        if (len >= bytes.length) {
            bytes = Arrays.copyOf(bytes, len * 2);
        }

        bytes[len++] = nextByte;
    }

    private String popString() {
        String result = new String(bytes, 0, len);
        len = 0;
        return result + "\u0000";
    }
    
    
}
