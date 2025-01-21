package bgu.spl.net.Tests;


import bgu.spl.net.impl.stomp.Frame.*;
import bgu.spl.net.srv.ConnectionsImpl;
import bgu.spl.net.srv.Connections;


public class TestProcess{
    
    public static void main(String[] args) {
        
    /**
     * header version != 1.2
     * header host != stomp.cs.bgu.ac.il
     * structure isnt valid
     */
    testConnectProcess();

    /**
     * the client is already connected
     * wrong password
     * the client isn't connected before but is now - valid frame
     * the client was connected before - valid frame
     */



        //      ClientFrame send = new ClientFrameSend("police", 12, "this is a test message");
        // ClientFrame unsubscribe = new ClientFrameUnsubscribe(12, 121212);
        // ClientFrame subscribe = new ClientFrameSubscribe(12, "police", 121212);
        // ClientFrame disconnect = new ClientFrameDisconnect(12);

    
    }


    /**
     * header version != 1.2
     * header host != stomp.cs.bgu.ac.il
     * structure isnt valid
     */
    private static void testConnectProcess(){
        Connections<String> connections = new ConnectionsImpl<>();
        // test wrong structure
        ClientFrame connect = new ClientFrameConnect("yam", "1234", 13);
        String testWrongStructure1 = "CONNECT\naccept-version:1.2\nhost:stomp.cs.bgu.ac.il\n\n\u0000";
        ServerFrame error1 = connect.process(testWrongStructure1, 0, connections, null);
        System.out.println(error1.toString());

        String testWrongStructure2 = "CONNECT\nlogin:yam\nhost:stomp.cs.bgu.ac.il\npasscode:123\nreceipt:1905\n\n\u0000";
        ServerFrame error2 = connect.process(testWrongStructure2, 0, connections, null);
        System.out.println(error2.toString());


        // test correct structure but wrong host or version

        String testWrongVersion = "CONNECT\naccept-version:1.1\nhost:stomp.cs.bgu.ac.il\nlogin:yam\npasscode:123\nreceipt:1905\n\n\u0000";
        ServerFrame error3 = connect.process(testWrongVersion, 0, connections, null);
        System.out.println(error3.toString());

        String testWrongHost = "CONNECT\naccept-version:1.2\nhost:stomp.cs.bgn.ac.il\nlogin:yam\npasscode:123\nreceipt:1905\n\n\u0000";
        ServerFrame error4 = connect.process(testWrongHost, 0, connections, null);
        System.out.println(error4.toString());

    }
} 


    