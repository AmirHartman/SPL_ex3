// package bgu.spl.net.Tests;

// package bgu.spl.net.srv;

// import bgu.spl.net.api.MessageEncoderDecoder;
// import bgu.spl.net.api.MessagingProtocol;
// import bgu.spl.net.impl.stomp.Auxiliary;
// import bgu.spl.net.impl.stomp.ClientFrameConnect;
// import bgu.spl.net.impl.stomp.StompCommand;
// import bgu.spl.net.impl.stomp.ServerFrame.ServerFrameError;

// import java.io.BufferedInputStream;
// import java.io.BufferedOutputStream;
// import java.io.IOException;
// import java.net.Socket;
// import java.util.concurrent.ConcurrentLinkedDeque;

// public class BlockingConnectionHandler<T> implements Runnable, ConnectionHandler<T> {

//     private final MessagingProtocol<T> protocol;
//     private final MessageEncoderDecoder<T> encdec;
//     private final Socket sock;
//     private BufferedInputStream in;
//     private BufferedOutputStream out;
//     private volatile boolean connected = true;
//     private ConcurrentLinkedDeque<T> messages = new ConcurrentLinkedDeque<>();

//     // הוספתי, צריך?X
//     private String username = null;

//     public BlockingConnectionHandler(Socket sock, MessageEncoderDecoder<T> reader, MessagingProtocol<T> protocol) {
//         this.sock = sock;
//         this.encdec = reader;
//         this.protocol = protocol;
//         this.protocol.setHandler(this);
//         protocol.addClient();
//     }

//     @Override
//     public void run() {
//         System.out.println("Start run method for client: " + sock.getRemoteSocketAddress());
//         try (Socket sock = this.sock) {
//             int read;
//             in = new BufferedInputStream(sock.getInputStream());
//             out = new BufferedOutputStream(sock.getOutputStream());
//             System.out.println("Input and output streams initialized for client: " + sock.getRemoteSocketAddress());

//             while (!protocol.shouldTerminate() && connected && (read = in.read()) >= 0) {
//                 T nextMessage = encdec.decodeNextByte((byte) read);
//                 if (nextMessage != null) {
//                     protocol.process(nextMessage);
//                     T response = messages.poll();
//                     if (response != null) {
//                         out.write(encdec.encode(response));
//                         out.flush();
//                         System.out.println("Response sent to client: " + response);
//                     }
//                     if (protocol.shouldTerminate()) {
//                         System.out.println("Graceful shutdown initiated for client: " + sock.getRemoteSocketAddress());
//                         break; // Exit the loop gracefully
//                     }
//                 }
//             }
//         } catch (IOException ex) {
//             System.err.println("Error in connection handler for client: " + sock.getRemoteSocketAddress() + " - " + ex.getMessage());
//         } finally {
//             System.out.println("Connection closed for client: " + sock.getRemoteSocketAddress());
//             // Explicit cleanup if necessary
//         }
//     }
    
    
//     //         while (!protocol.shouldTerminate() && connected && (read = in.read()) >= 0) {
//     //             T nextMessage = encdec.decodeNextByte((byte) read);
//     //             if (nextMessage != null) {
//     //                 protocol.process(nextMessage);
//     //                 T response = messages.poll();
//     //                 System.out.println("Response to client: " + response);
//     //                 if (response != null) {
//     //                     out.write(encdec.encode(response));
//     //                     // System.out.println("Response sent to client: " + sock.getRemoteSocketAddress());
//     //                     out.flush();
//     //                     System.out.println("Response flushed to client");
//     //                 }
//     //             }
//     //         }
//     //     } catch (IOException ex) {
//     //         System.err.println("Error in connection handler for client: " + sock.getRemoteSocketAddress() + " - " + ex.getMessage());
//     //         ex.printStackTrace();
//     //     } finally {
//     //         System.out.println("Connection closed for client: " + sock.getRemoteSocketAddress());
//     //         try {
//     //             close(); // Ensure resources are released
//     //         } catch (IOException e) {
//     //             System.err.println("Error closing socket: " + e.getMessage());
//     //         }
//     //     }
//     // }


    
//     @Override
//     public void close() throws IOException {
//         if (connected) { // Only close if the socket is still open
//             connected = false;
//             if (sock != null && !sock.isClosed()) {
//                 sock.close(); // Close the socket
//                 System.out.println("Socket closed for client.");
//             }
//         }
//     }
    
    
//     // @Override
//     // public void close() throws IOException {
//     //     connected = false;
//     //     sock.close();
//     // }

//     @Override
//     public void send(T msg) {
//         System.out.println(messages.add(msg));
//         System.out.println("Message added to messages queue: " + msg);
//     }

//     @Override
//     public String getUserName(){
//         return username;
//     }
// }

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// // package bgu.spl.net.srv;

// // import bgu.spl.net.api.MessageEncoderDecoder;
// // import bgu.spl.net.api.MessagingProtocol;
// // import java.io.BufferedInputStream;
// // import java.io.BufferedOutputStream;
// // import java.io.IOException;
// // import java.net.Socket;
// // import java.util.concurrent.ConcurrentLinkedDeque;

// // public class BlockingConnectionHandler<T> implements Runnable, ConnectionHandler<T> {

// //     private final MessagingProtocol<T> protocol;
// //     private final MessageEncoderDecoder<T> encdec;
// //     private final Socket sock;
// //     private BufferedInputStream in;
// //     private BufferedOutputStream out;
// //     private volatile boolean connected = true;
// //     private ConcurrentLinkedDeque<T> messages = new ConcurrentLinkedDeque<>();

// //     // הוספתי, צריך?X
// //     private String username = null;

// //     public BlockingConnectionHandler(Socket sock, MessageEncoderDecoder<T> reader, MessagingProtocol<T> protocol) {
// //         this.sock = sock;
// //         this.encdec = reader;
// //         this.protocol = protocol;
// //         this.protocol.setHandler(this);
// //     }

// //     @Override
// //     public void run() {
// //         try (Socket sock = this.sock) {
// //             int read;
// //             in = new BufferedInputStream(sock.getInputStream());
// //             out = new BufferedOutputStream(sock.getOutputStream());
    
// //             while (!protocol.shouldTerminate() && connected && (read = in.read()) >= 0) {
// //                 T nextMessage = encdec.decodeNextByte((byte) read);
// //                 if (nextMessage != null) {
// //                     protocol.process(nextMessage);
// //                     T response = messages.pollFirst();
// //                     if (response != null) {
// //                         out.write(encdec.encode(response));
// //                         out.flush();
// //                     }
// //                 }
// //             }
// //         } catch (IOException ex) {
// //             ex.printStackTrace();
// //         } finally {
// //             System.out.println("Connection closed for client: " + sock.getRemoteSocketAddress());
// //         }
// //     }
    
// //     @Override
// //     public void close() throws IOException {
// //         connected = false;
// //         sock.close();
// //     }

// //     @Override
// //     public void send(T msg) {
// //         messages.add(msg);
// //     }

// //     @Override
// //     public String getUserName(){
// //         return username;
// //     }
// // }
