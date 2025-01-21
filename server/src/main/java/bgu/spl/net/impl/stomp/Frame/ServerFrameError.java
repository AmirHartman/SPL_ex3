package bgu.spl.net.impl.stomp.Frame;


public class ServerFrameError extends ServerFrame {
    private String message;

    public ServerFrameError(String message, int receiptId, String body) {
        super(StompCommand.ERROR);
        this.receiptId = receiptId;
        this.message = message;
        this.body = "\n" + body + "\n\u0000";
    }

    public ServerFrameError(String string) {
        super(StompCommand.ERROR);
        String[] words = string.split(" ");
        if (!words[0].equals("ERROR")){
            throw new IllegalArgumentException("Not an ERROR frame");
        }
        int indexId = -1;
        int indexMessage = -1;
        int indexBody = -1;

        // find the headers and body indexes
        for (int i = 1; i < words.length; i++){
            if (words[i].equals("Receipt-id:")){
                indexId = i + 1;
            }
            else if (words[i].equals("Message:")){
                indexMessage = i + 1;
            }
            else if (words[i].equals("\n\n")){
                indexBody = i + 1;
            }
        }
        this.receiptId = Integer.parseInt(words[indexId], 10);
        this.message = "";
        for (int i = indexMessage; i < (indexBody-1) && !words[i-1].equals("Receipt-id:"); i++){
            message += words[i] + " ";
        }
        message = message.substring(0, message.length()-1);

        this.body = "\n ";
        for (int i = indexBody; i < words.length; i++){
            body += words[i] + " ";
        }
        body = body.substring(0, body.length()-1);
    }

    
    public String toString() {
        return type.name() + "\n"  
                + "Receipt-id:" + this.receiptId + "\n"
                + "Message:" + this.message + "\n" 
                + this.body;
    }    
}
