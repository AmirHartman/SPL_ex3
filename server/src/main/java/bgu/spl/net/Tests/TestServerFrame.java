package bgu.spl.net.Tests;

import bgu.spl.net.impl.stomp.ServerFrame.*;

public class TestServerFrame {
    public static void main(String[] args) {
        testServerFrameConnected();
        testServerFrameError();
        testServerFrameMessage();
        testServerFrameReceipt();

        testNullChar();

    }

    private static void testServerFrameConnected() {
        ServerFrame testFrame = new ServerFrameConnected(12);   
        String tst = testFrame.toString();
        System.out.println(tst);
    }

    private static void testServerFrameError() {
        ServerFrameError error = new ServerFrameError("this is a test", 2345, "this is the body.\nthis should contain the original frame that triggered the error frame");
        System.out.println(error.toString());

    }

    private static void testServerFrameMessage() {
        ServerFrameMessage message = new ServerFrameMessage(1, 1, "popo", "this is the body.\nit should contain the body of client frame send");
        System.out.println(message.toString());

    }

    private static void testServerFrameReceipt() {
        ServerFrameReceipt receipt = new ServerFrameReceipt(1);
        System.out.println(receipt.toString());
    }

    private static void testNullChar() {
        // initialize frames for test
        ServerFrameReceipt receipt = new ServerFrameReceipt(1);
        ServerFrameMessage message = new ServerFrameMessage(1, 1, "popo", "this is the body.\nit should contain the body of client frame send");
        ServerFrame connected = new ServerFrameConnected(12);
        ServerFrameError error = new ServerFrameError("this is a test", 2345, "this is the body.\nthis should contain the original frame that triggered the error frame");

        // get the last char of each frame
        char charReceipt = receipt.toString().charAt(receipt.toString().length() - 1);
        char charMessage = message.toString().charAt(message.toString().length() - 1);
        char charConnected = connected.toString().charAt(connected.toString().length() - 1);
        char charError = error.toString().charAt(error.toString().length() - 1);

        // print the last char of each frame
        System.out.println("Is last char of receipt frame is null char? " + (charReceipt == '\u0000'));
        System.out.println("Is last char of message frame is null char? " + (charMessage == '\u0000'));
        System.out.println("Is last char of connected frame is null char? " + (charConnected == '\u0000'));
        System.out.println("Is last char of error frame is null char? " + (charError == '\u0000'));
    }



}
