package bgu.spl.net.impl.stomp.serverFrame;


public class ServiceFrameConnected extends ServiceFrame {
    private final String version;

    public ServiceFrameConnected() {
        super(ServiceStompCommand.CONNECTED);
        this.version = "1.2";
    }

    public String toString() {
        return type.name() + "\n" 
                // + "Headers:\n" // צריך הבדל? בשביל קידוד ופענוח מזהה את הסוף עם פעמיים סלש ואות אן
                + "Version: " + this.version + "\n"
                + "\n" + this.nullChar;
    }

    
}
