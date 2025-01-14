package bgu.spl.net.impl.stomp.serverFrame;


public class ServiceFrameConnected extends ServiceFrame {
    private final String version;

    public ServiceFrameConnected() {
        super(ServiceStompCommand.CONNECTED);
        this.version = "1.2";
    }

    public String toString() {
        String result = "Stomp Command: CONNECTED\n" 
                + "Headers:\n" // צריך הבדל? בשביל קידוד ופענוח מזהה את הסוף עם פעמיים סלש ואות אן
                + "Version: " + this.version + "\n";
        if (this.body == "\n"){
            return result + "\n" + this.nullChar;
        } else {
            return result + "\n" + this.body + "\n" + this.nullChar;
        }
    }

    
}
