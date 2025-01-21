package bgu.spl.net.impl.stomp.Frame;

public class ServerFrameMessage extends ServerFrame {
    private int messageID;
    private int subscribtion;
    private String destination;

    public ServerFrameMessage (int messageID, int subscribtion, String destination, String body) {
        super(StompCommand.MESSAGE);
        this.messageID = messageID;
        this.subscribtion = subscribtion;
        this.destination = destination;
        this.body = "\n" + body + "\n\u0000";
    }

    public ServerFrameMessage (String string){
        super(StompCommand.MESSAGE);
        String[] words = string.split(" ");
        if (!words[0].equals("MESSAGE")){
            throw new IllegalArgumentException("Not a MESSAGE frame");
        }
        // find the headers and body indexes
        int indexMessageId = -1;
        int indexSubscribtion = -1;
        int indexDestination = -1;
        int indexBody = -1;

        for (int i = 1; i < words.length; i++){
            if (words[i].equals("Message-id:")){
                indexMessageId = i + 1;
            }
            else if (words[i].equals("subscribtion:")){
                indexSubscribtion = i + 1;
            }
            else if (words[i].equals("Destination:")){
                indexDestination = i + 1;
            }
            else if (words[i].equals("\n\n")){
                indexBody = i + 1;
            }
        }
        this.messageID = Integer.parseInt(words[indexMessageId], 10);
        this.subscribtion = Integer.parseInt(words[indexSubscribtion], 10);

        // in case topic is more than one word
        this.destination = "";
        for (int i = indexDestination; i < (indexBody-1) && !words[i-1].equals("Message-id:") && !words[i-1].equals("subscribtion:"); i++){
            destination += words[i] + " ";
        }
        destination = destination.substring(0, destination.length()-1);
        this.body = "\n ";
        for (int i = indexBody; i < words.length; i++){
            body += words[i] + " ";
        }
        body = body.substring(0, body.length()-1);
    }

    public String getTopic() {
        return destination;
    }

    public String toString() {
        return type.name() + "\n"  
                + "Message-id:" + this.messageID + "\n" 
                + "subscribtion:" + this.subscribtion + "\n" 
                + "Destination:" + this.destination + "\n" + this.body;
    }

}
