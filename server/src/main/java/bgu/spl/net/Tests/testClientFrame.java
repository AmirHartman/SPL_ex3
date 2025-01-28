package bgu.spl.net.Tests;

import bgu.spl.net.impl.stomp.ClientFrame;
import bgu.spl.net.impl.stomp.ClientFrameConnect;
import bgu.spl.net.impl.stomp.ClientFrameDisconnect;
import bgu.spl.net.impl.stomp.ClientFrameSubscribe;
import bgu.spl.net.impl.stomp.ClientFrameUnsubscribe;

public class testClientFrame {

        public static void main(String[] args) {

        /**
         * test the toString method of ClientFrame classes
         * tests coth constructors
         */
        testClientFrameConnect();
        testClientFrameDisconnect();
        // testClientFrameSend();
        testClientFrameSubscribe();
        testClientFrameUnsubscribe();

        testNullChar();
        /**
         * test headers in incorrect order
         */


    }


    private static void testClientFrameConnect() {
        ClientFrame testFrame = new ClientFrameConnect("yam", "1234");
        String tst = testFrame.toString();
        System.out.println(tst);
        ClientFrame testFrame2 = new ClientFrameConnect(tst);
        String tst2 = testFrame2.toString();
        System.out.println(tst2);
        ClientFrame testFrame3 = new ClientFrameConnect(tst2);
        System.out.println(testFrame3.toString());
    }

    private static void testClientFrameDisconnect() {
        ClientFrame testFrame = new ClientFrameDisconnect(12);
        String tst = testFrame.toString();
        System.out.println(tst);
        ClientFrame testFrame2 = new ClientFrameDisconnect(tst);
        String tst2 = testFrame2.toString();
        System.out.println(tst2);
        ClientFrame testFrame3 = new ClientFrameDisconnect(tst2);
        System.out.println(testFrame3.toString());
    }

    // private static void testClientFrameSend() {
    //     ClientFrame testFrame = new ClientFrameSend("police", 12, "this is a test message");
    //     String tst = testFrame.toString();
    //     System.out.println(tst);
    //     ClientFrame testFrame2 = new ClientFrameSend(tst);
    //     String tst2 = testFrame2.toString();
    //     System.out.println(tst2);
    //     ClientFrame testFrame3 = new ClientFrameSend(tst2);
    //     System.out.println(testFrame3.toString());
    // }

    private static void testClientFrameSubscribe() {
        ClientFrame testFrame = new ClientFrameSubscribe(12, "police", 121212);
        String tst = testFrame.toString();
        System.out.println(tst);
        ClientFrame testFrame2 = new ClientFrameSubscribe(tst);
        String tst2 = testFrame2.toString();
        System.out.println(tst2);
        ClientFrame testFrame3 = new ClientFrameSubscribe(tst2);
        System.out.println(testFrame3.toString());
    }

    private static void testClientFrameUnsubscribe() {
        ClientFrame testFrame = new ClientFrameUnsubscribe(12, 121212);
        String tst = testFrame.toString();
        System.out.println(tst);
        ClientFrame testFrame2 = new ClientFrameUnsubscribe(tst);
        String tst2 = testFrame2.toString();
        System.out.println(tst2);
        ClientFrame testFrame3 = new ClientFrameUnsubscribe(tst2);
        System.out.println(testFrame3.toString());
    }



    private static void testNullChar() {
        // initialize frames for test
        // ClientFrame send = new ClientFrameSend("police", 12, "this is a test message");
        ClientFrame unsubscribe = new ClientFrameUnsubscribe(12, 121212);
        ClientFrame subscribe = new ClientFrameSubscribe(12, "police", 121212);
        ClientFrame disconnect = new ClientFrameDisconnect(12);
        ClientFrame connect = new ClientFrameConnect("yam", "1234");

        // get the last char of each frame
        // char charSend = send.toString().charAt(send.toString().length() - 1);
        char charUnsubscribe = unsubscribe.toString().charAt(unsubscribe.toString().length() - 1);
        char charSubscribe = subscribe.toString().charAt(subscribe.toString().length() - 1);
        char charDisconnect = disconnect.toString().charAt(disconnect.toString().length() - 1);
        char charConnect = connect.toString().charAt(connect.toString().length() - 1);


        // print the last char of each frame
        // System.out.println("Is last char of send frame is null char? " + (charSend == '\u0000'));
        System.out.println("Is last char of unsubscribe frame is null char? " + (charUnsubscribe == '\u0000'));
        System.out.println("Is last char of subscribe frame is null char? " + (charSubscribe == '\u0000'));
        System.out.println("Is last char of disconnect frame is null char? " + (charDisconnect == '\u0000'));
        System.out.println("Is last char of connect frame is null char? " + (charConnect == '\u0000'));

    }


}
