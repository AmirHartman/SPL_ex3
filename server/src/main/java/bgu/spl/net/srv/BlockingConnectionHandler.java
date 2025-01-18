package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.impl.stomp.Frame.ClientFrame;
import bgu.spl.net.impl.stomp.Frame.ClientFrameConnect;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.Socket;
import bgu.spl.net.impl.stomp.Frame.*;

public class BlockingConnectionHandler<T> implements Runnable, ConnectionHandler<T> {

    private final MessagingProtocol<T> protocol;
    private final MessageEncoderDecoder<T> encdec;
    private final Socket sock;
    private BufferedInputStream in;
    private BufferedOutputStream out;
    private volatile boolean connected = true;

    //הוספתי, צריך?
    String username = null;

    public BlockingConnectionHandler(Socket sock, MessageEncoderDecoder<T> reader, MessagingProtocol<T> protocol) {
        this.sock = sock;
        this.encdec = reader;
        this.protocol = protocol;
    }

    @Override
    public void run() {
        try (Socket sock = this.sock) { //just for automatic closing
            int read;

            in = new BufferedInputStream(sock.getInputStream());
            out = new BufferedOutputStream(sock.getOutputStream());

            while (!protocol.shouldTerminate() && connected && (read = in.read()) >= 0) {
                T nextMessage = encdec.decodeNextByte((byte) read);
                if (nextMessage != null) {
                    // שינויים שלנו
                    ClientFrame clientFrame = chooseClientFrame((String) nextMessage);
                    //מאתחל את USERNAME
                    if (username == null & nextMessage instanceof ClientFrameConnect) {
                            this.username = ((ClientFrameConnect) clientFrame).getUsername();
                        }
                    protocol.process(nextMessage);
                    T response = protocol.process(nextMessage);
                    if (response != null) {
                        out.write(encdec.encode(response));
                        out.flush();
                    }
                }
            }

        } catch (IOException ex) {
            ex.printStackTrace();
        }

    }

    @Override
    public void close() throws IOException {
        connected = false;
        sock.close();
    }

    @Override
    public void send(T msg) {
        //IMPLEMENT IF NEEDED
    }

    @Override   
    public String getUserName(){
        return username;
    }

    private ClientFrame chooseClientFrame(String nextMessage){
        String frameType = nextMessage.substring(0, nextMessage.indexOf('\n'));
        switch (frameType){
            case "CONNECT":
                return new ClientFrameConnect(nextMessage);
            case "SEND":
                return new ClientFrameSend(nextMessage);
            case "SUBSCRIBE":
                return new ClientFrameSubscribe(nextMessage);
            case "UNSUBSCRIBE":
                return new ClientFrameUnsubscribe(nextMessage);
            case "DISCONNECT":
                return new ClientFrameDisconnect(nextMessage);
        }

        return null;
    }
}
