package bgu.spl.net.Tests;


import bgu.spl.net.impl.stomp.*;

public class TestProcess{
    
    public static void main(String[] args) {
        
    /**
     * header version != 1.2
     * header host != stomp.cs.bgu.ac.il
     * structure isnt valid
     */
    testvalidateClientFrameConnect();

    /**
     * the client is already connected
     * wrong password
     * the client isn't connected before but is now - valid frame
     * the client was connected before - valid frame
     */
    
    }


    /**
     * header version != 1.2
     * header host != stomp.cs.bgu.ac.il
     * structure isnt valid
     */
    private static void testvalidateClientFrameConnect(){

        // test frame without null char
        String testNullChar = "CONNECT\naccept-version:1.1\nhost:stomp.cs.bgu.ac.il\nlogin:yam\npasscode:123\nreceipt:1905\n\n";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.CONNECT, testNullChar).toString());

        // test fram with a body (body should be empty)
        String testEmptyBody = "CONNECT\naccept-version:1.2\nhpst:stomp.cs.bgu.ac.il\nlogin:yam\npasscode:123\nreceipt:1905\n\nthis is my message body\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.CONNECT, testEmptyBody).toString());

        // test wrong number of headers
        String testNumbeOfHeaders = "CONNECT\naccept-version:1.2\nhost:stomp.cs.bgu.ac.il\n\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.CONNECT, testNumbeOfHeaders).toString());
        
        // test wrong header name
        String testHeaderName = "CONNECT\naccept-version:1.2\nhpst:stomp.cs.bgu.ac.il\nlogin:yam\npasscode:123\nreceipt:1905\n\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.CONNECT, testHeaderName).toString());

        // test receipt id is not an integer
        String testReceiptId = "CONNECT\naccept-version:1.2\nhost:stomp.cs.bgu.ac.il\nlogin:yam\npasscode:123\nreceipt:yam\n\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.CONNECT, testReceiptId).toString());
        
        // test correct structure but wrong version: 1.1, should be 1.2
        String testWrongVersion = "CONNECT\naccept-version:1.1\nhost:stomp.cs.bgu.ac.il\nlogin:yam\npasscode:123\nreceipt:1905\n\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.CONNECT, testWrongVersion).toString());

        // test correct structure but wrong host: bgn, should be bgu
        String testWrongHost = "CONNECT\naccept-version:1.2\nhost:stomp.cs.bgn.ac.il\nlogin:yam\npasscode:123\nreceipt:1905\n\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.CONNECT, testWrongHost).toString());



    }

    private static void testvalidateClientFrameDisconnect(){

    }

    private static void testvalidateClientFrameSubscribe(){

    }

    private static void testvalidateClientFrameUnsubscribe(){

    }

    private static void testvalidateClientFrameSend(){

    }

} 


    