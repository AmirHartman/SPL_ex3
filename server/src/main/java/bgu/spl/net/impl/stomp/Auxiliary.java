package bgu.spl.net.impl.stomp;

import bgu.spl.net.impl.stomp.ServerFrame.ServerFrameError;

public class Auxiliary {




    public static StompCommand stringToCommand (String type) {
        switch (type) {
            case "CONNECT":
                return StompCommand.CONNECT;
            case "SUBSCRIBE":
                return StompCommand.SUBSCRIBE;
            case "UNSUBSCRIBE":
                return StompCommand.UNSUBSCRIBE;
            case "SEND":
                return StompCommand.SEND;
            case "DISCONNECT":
                return StompCommand.DISCONNECT;
            default:
                return null;
        }
    }



    public static ClientFrame chooseClientFrame (StompCommand command, String msg){
        switch (command){
            case CONNECT:
                return new ClientFrameConnect(msg);
            case SEND:
                return new ClientFrameSend(msg);    
            case SUBSCRIBE:
                return new ClientFrameSubscribe(msg);
            case UNSUBSCRIBE:
                return new ClientFrameUnsubscribe(msg);
            case DISCONNECT:
                return new ClientFrameDisconnect(msg);
            default:
                return null;
        }
    }

    public static ServerFrameError validateClientFrame (StompCommand command, String toFrame){
        switch (command){
            case CONNECT:
                return validateConnectFrame(toFrame);
            case SEND:
                return validateSendFrame(toFrame);
            case SUBSCRIBE:
                return validateSubscribeFrame(toFrame);
            case UNSUBSCRIBE:
                return validateUnsubscribeFrame(toFrame);
            case DISCONNECT:
                return validateDisconnectFrame(toFrame);
            default:
                return null;
        }
    }

    private static ServerFrameError validateConnectFrame (String toFrame){
        // check the validity of the frame structure
        ServerFrameError error = checkHeadersNumber(toFrame, 4);
        if (error != null) {
            return error;
        }
        error = checkNullChar(toFrame);
        if (error != null) {
            return error;
        }
        error = isFrameBodyEmpty(toFrame);
        if (error != null){
            return error;
        }
        String frame = toFrame.split("\n\n")[0];
        String [] headers = frame.split("\n");
        // int receiptId = -1;
        for (int i = 1; i < headers.length; i++){
            String[] header = headers[i].split(":");
            if (!header[0].equals("accept-version") & !header[0].equals("host") & !header[0].equals("login") & !header[0].equals("passcode")){
                return new ServerFrameError("invalid one or more header names", -1, toFrame);
            }// check the validity accept-version
            if (header[0].equals("accept-version") & !header[1].equals("1.2")){
                return new ServerFrameError("Wrong version", -1, toFrame);
            }// check the validity host
            if (header[0].equals("host") & !header[1].equals("stomp.cs.bgu.ac.il")){
                return new ServerFrameError("Wrong host", -1, toFrame);
            }
        }
    return null;
    }

    private static ServerFrameError validateSendFrame (String toFrame){
        ServerFrameError error = checkHeadersNumber(toFrame, 2);
        if (error != null) {
            return error;
        }
        error = checkNullChar(toFrame);
        if (error != null) {
            return error;
        }
        error = isFrameBodyEmpty(toFrame);
        if (error == null){
            return new ServerFrameError("no body in the frame", -1, toFrame);
        }
        String frame = toFrame.split("\n\n")[0];
        // String [] headers = frame.split("\n");
        // String[] header = headers[1].split(":");
        // if (!header[0].equals("destination")){
        //     return new ServerFrameError("invalid header name", -1, toFrame);
        // }
        String [] headers = frame.split("\n");
        int receiptId = -1;
        for (int i = 1; i < headers.length; i++){
            String[] header = headers[i].split(":");
            // find the receipt id
            if (header[0].equals("receipt")){
                try {
                    receiptId = Integer.parseInt(header[1]);
                } catch (Exception e) {
                    return new ServerFrameError("receipt id is not an integer", -1, toFrame);
                }
            }
            // check the validity of header names
            if (!header[0].equals("destination") & !header[0].equals("receipt")){
                return new ServerFrameError("invalid one or more header names", receiptId, toFrame);
            }
        }
        return null;
    }

    private static ServerFrameError validateSubscribeFrame (String toFrame){// destination & id
        ServerFrameError error = checkHeadersNumber(toFrame, 3);
        if (error != null) {
            return error;
        }
        error = checkNullChar(toFrame);
        if (error != null) {
            return error;
        }
        error = isFrameBodyEmpty(toFrame);
        if (error != null){
            return error;
        }
        String frame = toFrame.split("\n\n")[0];
        String [] headers = frame.split("\n");
        int receiptId = -1;
        for (int i = 1; i < headers.length; i++){
            String[] header = headers[i].split(":");
            // find the receipt id
            if (header[0].equals("receipt")){
                try {
                    receiptId = Integer.parseInt(header[1]);
                } catch (Exception e) {
                    return new ServerFrameError("receipt id is not an integer", -1, toFrame);
                }
            }// check subscription id is a number
            if (header[0].equals("id")){
                try {
                    Integer.parseInt(header[1]);
                } catch (Exception e) {
                    return new ServerFrameError("subscription id is not an integer", -1, toFrame);
                }
            }
            // check the validity of header names
            if (!header[0].equals("destination") & !header[0].equals("id") & !header[0].equals("receipt")){
                return new ServerFrameError("invalid one or more header names", receiptId, toFrame);
            }
        }
        return null;
    }

    private static ServerFrameError validateUnsubscribeFrame (String toFrame){// id 
        ServerFrameError error = checkHeadersNumber(toFrame, 2);
        if (error != null) {
            return error;
        }
        error = checkNullChar(toFrame);
        if (error != null) {
            return error;
        }
        error = isFrameBodyEmpty(toFrame);
        if (error != null){
            return error;
        }
        String frame = toFrame.split("\n\n")[0];
        String [] headers = frame.split("\n");
        int receiptId = -1;
        for (int i = 1; i < headers.length; i++){
            String[] header = headers[i].split(":");
            // find the receipt id
            if (header[0].equals("receipt")){
                try {
                    receiptId = Integer.parseInt(header[1]);
                } catch (Exception e) {
                    return new ServerFrameError("receipt id is not an integer", -1, toFrame);
                }
            }// check subscription id is a number
            if (header[0].equals("id")){
                try {
                    Integer.parseInt(header[1]);
                } catch (Exception e) {
                    return new ServerFrameError("subscription id is not an integer", -1, toFrame);
                }
            }
            // check the validity of header names
            if (!header[0].equals("id") & !header[0].equals("receipt")){
                return new ServerFrameError("invalid one or more header names", receiptId, toFrame);
            }
        }
        return null;
    }

    private static ServerFrameError validateDisconnectFrame (String toFrame){
        ServerFrameError error = checkHeadersNumber(toFrame, 1);
        if (error != null) {
            return error;
        }
        error = checkNullChar(toFrame);
        if (error != null) {
            return error;
        }
        error = isFrameBodyEmpty(toFrame);
        if (error != null){
            return error;
        }
        String headerline = toFrame.split("\n")[1];
        String [] header = headerline.split(":");
        int receiptId = -1;
        if (!header[0].equals("receipt")){
            return new ServerFrameError("invalid header name", receiptId, toFrame);
        } 
        try {
            receiptId = Integer.parseInt(header[1]);
        } catch (Exception e) {
            return new ServerFrameError("receipt id is not an integer", -1, toFrame);
        }
        return null;
    }



    private static ServerFrameError checkHeadersNumber (String toFrame, int headersNumber){
        String [] frame = toFrame.split("\n\n");
        int headersNum = frame[0].split(":").length-1;
        if (headersNum != headersNumber){
            return new ServerFrameError("number of headers is invalid", -1, toFrame);
        }
        return null;
    }


    private static ServerFrameError checkNullChar (String toFrame){
        String [] frame = toFrame.split("\n\n");
        if (frame.length == 1 || !(frame[1].charAt(frame[1].length()-1) == '\u0000')){
            return new ServerFrameError("no null char at the end of the frame", -1, toFrame);
        }
        return null;
    }

    private static ServerFrameError isFrameBodyEmpty (String toFrame){
        String [] frame = toFrame.split("\n\n");
        String body = frame[1];
        if (body.length() > 0 && !body.equals("\u0000")){
            return new ServerFrameError("body isn't empty", -1, toFrame);
        }
        return null;
    }
    
}
