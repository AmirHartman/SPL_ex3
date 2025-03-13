package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.concurrent.ConcurrentLinkedDeque;

public class BlockingConnectionHandler<T> implements Runnable, ConnectionHandler<T> {

    private final MessagingProtocol<T> protocol;
    private final MessageEncoderDecoder<T> encdec;
    private final Socket sock;
    private BufferedInputStream in;
    private BufferedOutputStream out;
    private volatile boolean connected = true;
    private ConcurrentLinkedDeque<T> messages = new ConcurrentLinkedDeque<>();

    public BlockingConnectionHandler(Socket sock, MessageEncoderDecoder<T> reader, MessagingProtocol<T> protocol) {
        this.sock = sock;
        this.encdec = reader;
        this.protocol = protocol;
        this.protocol.setHandler(this);
        protocol.addClient();
    }

    @Override
    public void run() {
        try (Socket sock = this.sock) {
            int read = -1;
           
            in = new BufferedInputStream(sock.getInputStream());
            out = new BufferedOutputStream(sock.getOutputStream());

            while (!protocol.shouldTerminate() && connected && (read = in.read()) >= 0) {
                T nextMessage = encdec.decodeNextByte((byte) read);
                if (nextMessage != null) {
                    protocol.process(nextMessage);
                    synchronized (messages) {
                        while (!messages.isEmpty()) {
                            T response = messages.poll();
                            if (response != null) {
                                out.write(encdec.encode(response));
                                out.flush();
                            }}}}}
        } catch (IOException ex) {
            ex.printStackTrace();
        }
        finally {
            protocol.close();
        }
    }   
    
    @Override
    public void close() throws IOException {
        if (connected) { // Only close if the socket is still open
            connected = false;
            if (sock != null && !sock.isClosed()) {
                sock.close(); // Close the socket
            }
        }
    }

    

    @Override
    public void send(T msg) {
        synchronized (messages) {
            messages.add(msg);
        }
    }
    
}





