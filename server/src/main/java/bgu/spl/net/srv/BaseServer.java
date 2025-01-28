package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.function.Supplier;

public abstract class BaseServer<T> implements Server<T> {

    private final int port;
    private final Supplier<MessagingProtocol<T>> protocolFactory;
    private final Supplier<MessageEncoderDecoder<T>> encdecFactory;
    private ServerSocket sock;

    public BaseServer(
            int port,
            Supplier<MessagingProtocol<T>> protocolFactory,
            Supplier<MessageEncoderDecoder<T>> encdecFactory) {

        this.port = port;
        this.protocolFactory = protocolFactory;
        this.encdecFactory = encdecFactory;
		this.sock = null;


    }

    @Override
    public void serve() {

        try (ServerSocket serverSock = new ServerSocket(port)) {
			System.out.println("TPC: Server started");
            
            System.out.println("TPC: Server started on port: " + port);

            this.sock = serverSock; //just to be able to close

            while (!Thread.currentThread().isInterrupted()) {
                System.out.println("TPC: Waiting for a client to connect...");

                Socket clientSock = serverSock.accept();
                System.out.println("TPC: Client connected: " + clientSock.getRemoteSocketAddress());

                BlockingConnectionHandler<T> handler = new BlockingConnectionHandler<>(
                        clientSock,
                        encdecFactory.get(),
                        protocolFactory.get());
                        
                System.out.println("TPC: Handler created for client: " + clientSock.getRemoteSocketAddress());
                execute(handler);
                System.out.println("TPC: Handler executed for client: " + clientSock.getRemoteSocketAddress());
            }
        } catch (IOException ex) {
            System.err.println("TPC: Error in serve(): " + ex.getMessage());
            ex.printStackTrace();
        }

        System.out.println("server closed!!!");
    }

    @Override
    public void close() throws IOException {
		if (sock != null)
			sock.close();
    }

    protected abstract void execute(BlockingConnectionHandler<T>  handler);

}
