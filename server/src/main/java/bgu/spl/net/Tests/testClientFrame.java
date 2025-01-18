package bgu.spl.net.Tests;

import bgu.spl.net.impl.stomp.Frame.ClientFrame;
import bgu.spl.net.impl.stomp.Frame.ClientFrameConnect;
import bgu.spl.net.impl.stomp.Frame.ClientFrameDisconnect;
import bgu.spl.net.impl.stomp.Frame.ClientFrameSend;
import bgu.spl.net.impl.stomp.Frame.ClientFrameSubscribe;
import bgu.spl.net.impl.stomp.Frame.ClientFrameUnsubscribe;
import bgu.spl.net.impl.stomp.Frame.ServiceFrameMessage;

public class testClientFrame {

        public static void main(String[] args) {

        /**
         * test the toString method of ClientFrame classes
         * tests coth constructors
         */
        testClientFrameConnect();
        testClientFrameDisconnect();
        testClientFrameSend();
        testClientFrameSubscribe();
        testClientFrameUnsubscribe();

        /**
         * test headers in incorrect order
         */


    }


    private static void testClientFrameConnect() {
        ClientFrame testFrame = new ClientFrameConnect("yam", "1234", 13);
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

    private static void testClientFrameSend() {
        ClientFrame testFrame = new ClientFrameSend("police", 12, "this is a test message");
        String tst = testFrame.toString();
        System.out.println(tst);
        ClientFrame testFrame2 = new ClientFrameSend(tst);
        String tst2 = testFrame2.toString();
        System.out.println(tst2);
        ClientFrame testFrame3 = new ClientFrameSend(tst2);
        System.out.println(testFrame3.toString());
    }

    private static void testClientFrameSubscribe() {
        ClientFrame testFrame = new ClientFrameSubscribe(12, "police");
        String tst = testFrame.toString();
        System.out.println(tst);
        ClientFrame testFrame2 = new ClientFrameSubscribe(tst);
        String tst2 = testFrame2.toString();
        System.out.println(tst2);
        ClientFrame testFrame3 = new ClientFrameSubscribe(tst2);
        System.out.println(testFrame3.toString());
    }

    private static void testClientFrameUnsubscribe() {
        ClientFrame testFrame = new ClientFrameUnsubscribe(12);
        String tst = testFrame.toString();
        System.out.println(tst);
        ClientFrame testFrame2 = new ClientFrameUnsubscribe(tst);
        String tst2 = testFrame2.toString();
        System.out.println(tst2);
        ClientFrame testFrame3 = new ClientFrameUnsubscribe(tst2);
        System.out.println(testFrame3.toString());
    }



}
