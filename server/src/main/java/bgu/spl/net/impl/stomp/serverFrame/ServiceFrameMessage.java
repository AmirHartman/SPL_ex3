package bgu.spl.net.impl.stomp.serverFrame;

public class ServiceFrameMessage extends ServiceFrame {
    private int messageID;
    private int subscribtion;
    private String destination;

    public ServiceFrameMessage (int messageID, int subscribtion, String destination, String body) {
        super(ServiceStompCommand.MESSAGE);
        this.messageID = messageID;
        this.subscribtion = subscribtion;
        this.destination = destination;
        this.body = "\n " + body + " \u0000";
    }

    public ServiceFrame process(String string){
        String[] words = string.split(" ");
        if (!words[0].equals("RECEIPT")){
            return null;
        }
        int indexMessageId = -1;
        int indexSubscribtion = -1;
        int indexDestination = -1;
        int indexBody = -1;

        // find the headers and body indexes
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

        int messageID = -1;
        int subscribtion = -1;
        try {
            messageID = Integer.parseInt(words[indexMessageId], 10);
        } catch (NumberFormatException e) {
            System.out.println("messageId is not an integer");
            return null;
        }
        try {
            subscribtion = Integer.parseInt(words[indexSubscribtion], 10);
        } catch (NumberFormatException e) {
            System.out.println("subscribtion is not an integer");
            return null;
        }

        String destination = "";
        for (int i = indexDestination; i < (indexBody-1) && !words[i-1].equals("Message-id") && !words[i-1].equals("subscribtion:"); i++){
            destination += words[i] + " ";
        }


    }
    // public String toString() {
    //     return type.name() + " Message-id: " + this.messageID + " subscribtion: " + this.subscribtion + 
    //             " Destination: " + this.destination + " " + this.body + "\u0000";
    // }

    public String toString() {
        return type.name() + "\n"  
                + " Message-id: " + this.messageID + "\n" 
                + " subscribtion: " + this.subscribtion + "\n" 
                + " Destination: " + this.destination + " \n" + this.body;
    }

}
