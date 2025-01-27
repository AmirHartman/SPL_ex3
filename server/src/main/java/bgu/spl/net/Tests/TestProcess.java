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
    testvalidateClientFrameDisconnect();
    testvalidateClientFrameSubscribe();
    testvalidateClientFrameUnsubscribe();
    testvalidateClientFrameSend();

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
        String testWrongVersion = "CONNECT\naccept-version:1.1\nhost:stomp.cs.bgu.ac.il\nlogin:yam\npasscode:123\n\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.CONNECT, testWrongVersion).toString());

        // test correct structure but wrong host: bgn, should be bgu
        String testWrongHost = "CONNECT\naccept-version:1.2\nhost:stomp.cs.bgn.ac.il\nlogin:yam\npasscode:123\nreceipt:1905\n\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.CONNECT, testWrongHost).toString());
    }

    private static void testvalidateClientFrameDisconnect(){
        // test frame without null char
        String testNullChar = "DISCONNECT\ndestination:/topic/dest\nreceipt:1905\n\n";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.DISCONNECT, testNullChar).toString());

        // test fram with a body (body should be empty)
        String testEmptyBody = "DISCONNECT\nreceipt:1905\n\nthis is my message body\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.DISCONNECT, testEmptyBody).toString());

        // test wrong number of headers
        String testNumbeOfHeaders = "DISCONNECT\n\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.DISCONNECT, testNumbeOfHeaders).toString());
        
        // test wrong header name
        String testHeaderName = "DISCONNECT\nreciept:1905\n\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.DISCONNECT, testHeaderName).toString());

        // test receipt id is not an integer
        String testReceiptId = "DISCONNECT\nreceipt:yam\n\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.DISCONNECT, testReceiptId).toString());

    }

    private static void testvalidateClientFrameSubscribe(){
        // test frame without null char
        String testNullChar = "SUBSCRIBE\nid:9\ndestination:dest\nreceipt:1905\n\n";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.SUBSCRIBE, testNullChar).toString());

        // test frame with a body (body should be empty)
        String testEmptyBody = "SUBSCRIBE\nid:9\ndestination:dest\nreceipt:1905\n\nthis is my message body\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.SUBSCRIBE, testEmptyBody).toString());

        // test wrong number of headers
        String testNumbeOfHeaders = "SUBSCRIBE\ndestination:/topic/dest\n\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.SUBSCRIBE, testNumbeOfHeaders).toString());
        
        // test wrong header name
        String testHeaderName = "SUBSCRIBE\nid:9\ndestination:/topic/dest\nreciept:1905\n\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.SUBSCRIBE, testHeaderName).toString());

        // test receipt id is not an integer
        String testReceiptId = "SUBSCRIBE\nid:9\ndestination:/topic/dest\nreceipt:yam\n\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.SUBSCRIBE, testReceiptId).toString());

        // test subscription id is not an integer
        String testSubscriptionId = "SUBSCRIBE\nid:yam\ndestination:/topic/dest\nreceipt:1905\n\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.SUBSCRIBE, testSubscriptionId).toString());

        // test destination starts with /topic/
        String testDestination = "SEND\ndestination:police\nreceipt:1\n\nthis is a message\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.SEND, testDestination).toString());
        
        
    }

    private static void testvalidateClientFrameUnsubscribe(){
        // test frame without null char
        String testNullChar = "UNSUBSCRIBE\nid:9\nreceipt:1905\n\n";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.UNSUBSCRIBE, testNullChar).toString());

        // test frame with a body (body should be empty)
        String testEmptyBody = "UNSUBSCRIBE\nid:9\nreceipt:1905\n\nthis is my message body\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.UNSUBSCRIBE, testEmptyBody).toString());

        // test wrong number of headers
        String testNumbeOfHeaders = "UNSUBSCRIBE\nid:9\n\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.UNSUBSCRIBE, testNumbeOfHeaders).toString());
        
        // test wrong header name
        String testHeaderName = "UNSUBSCRIBE\nid:9\nreciept:1905\n\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.UNSUBSCRIBE, testHeaderName).toString());

        // test receipt id is not an integer
        String testReceiptId = "UNSUBSCRIBE\nid:9\nreceipt:yam\n\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.UNSUBSCRIBE, testReceiptId).toString());

        // test subscription id is not an integer
        String testSubscriptionId = "UNSUBSCRIBE\nid:yam\nreceipt:1905\n\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.UNSUBSCRIBE, testSubscriptionId).toString());
    }

    private static void testvalidateClientFrameSend(){
        // test frame without null char
        String testNullChar = "SEND\ndestination:/topic/police\nreceipt:1905\n\nthis is a message\n";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.SEND, testNullChar).toString());

        // test frame with a body (body shouldn't be empty)
        String testEmptyBody = "SEND\ndestination:/topic/police\nreceipt:1905\n\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.SEND, testEmptyBody).toString());

        // test wrong number of headers
        String testNumbeOfHeaders = "SEND\ndestination:/topic/police\n\nthis is a message\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.SEND, testNumbeOfHeaders).toString());
        
        // test wrong header name
        String testHeaderName = "SEND\ndestenation:/topic/police\nreciept:1905\n\nthis is a message\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.SEND, testHeaderName).toString());

        // test receipt id is not an integer
        String testReceiptId = "SEND\ndestination:/topic/police\nreceipt:yam\n\nthis is a message\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.SEND, testReceiptId).toString());

        // test destination starts with /topic/
        String testDestination = "SEND\ndestination:police\nreceipt:1\n\nthis is a message\n\u0000";
        System.out.println(Auxiliary.validateClientFrame(StompCommand.SEND, testDestination).toString());


    }
} 

    