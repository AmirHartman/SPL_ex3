package bgu.spl.net.impl.stomp.serverFrame;


public class ServiceFrameConnected extends ServiceFrame {
    private final String version;

    public ServiceFrameConnected() {
        super(ServiceStompCommand.CONNECTED);
        this.version = "1.2";
    }

    public ServiceFrame process(String string){
        String[] words = string.split(" ");
        if (!words[0].equals("CONNECTED")){
            return null;
        }
        return new ServiceFrameConnected();
    }

    // public String toString() {
    //     return type.name() + " Version: " + this.version + "\u0000";
    // }

    public String toString() {
        return type.name() + "\n" 
                + " Version: " + this.version + "\n" + this.body + " \u0000";
    }


    
}
