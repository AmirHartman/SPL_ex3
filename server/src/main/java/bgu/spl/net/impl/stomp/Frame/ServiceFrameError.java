package bgu.spl.net.impl.stomp.Frame;


public class ServiceFrameError extends ServiceFrame {
    private int receiptId;
    private String message;

    public ServiceFrameError(String message, int receiptId, String body) {
        super(ServiceStompCommand.ERROR);
        this.receiptId = receiptId;
        this.message = message;
        this.body = "\n " + body + " \u0000";
    }

    public ServiceFrameError(String string) {
        super(ServiceStompCommand.ERROR);
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
        return type.name() + " \n "  
                + "Receipt-id: " + this.receiptId + " \n "
                + "Message: " + this.message + " \n" + this.body;
    }    

    
}
        // public ServiceFrame process(String string){
            // String[] words = string.split(" ");
            // if (!words[0].equals("ERROR")){
            //     return null;
            // }
            // int indexId = -1;
            // int indexMessage = -1;
            // int indexBody = -1;
    
            // // find the headers and body indexes
            // for (int i = 1; i < words.length; i++){
            //     if (words[i].equals("Receipt-id:")){
            //         indexId = i + 1;
            //     }
            //     else if (words[i].equals("Message:")){
            //         indexMessage = i + 1;
            //     }
            //     else if (words[i].equals("\n\n")){
            //         indexBody = i + 1;
            //     }
            // }
            // int id = -1;
            // try {
            //     id = Integer.parseInt(words[indexId], 10);
            // } catch (NumberFormatException e) {
            //     System.out.println("messageId is not an integer");
            //     return null;
            // }
            // String message = "";
            // for (int i = indexMessage; i < (indexBody-1) && !words[i-1].equals("Receipt-id:"); i++){
            //     message += words[i] + " ";
            // }
            // message = message.substring(0, message.length()-1);
    
            // String body = "";
            // for (int i = indexBody; i < words.length; i++){
            //     body += words[i] + " ";
            // }
            // body = body.substring(0, body.length()-1);
            // return new ServiceFrameError(message, id, body);
        // }
    
        // public String toString() {
        //     return type.name() + " Receipt-id: " + this.receiptId + " Message: " + this.message + " " + this.body + "\u0000";
        // }    
