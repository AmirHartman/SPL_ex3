package bgu.spl.net.impl.stomp.Frame;


public class ServiceFrameConnected extends ServiceFrame {
    private final String version;

    public ServiceFrameConnected() {
        super(ServiceStompCommand.CONNECTED);
        this.version = "1.2";
    }

    public ServiceFrameConnected(String string) {
        super(ServiceStompCommand.CONNECTED);
        this.version = "1.2";
    }


    // public String toString() {
    //     return type.name() + " Version: " + this.version + "\u0000";
    // }

    public String toString() {
        return type.name() + "\n" 
                + " Version: " + this.version + "\n" + this.body;
    }

    public void test () {
        super.test();
        System.out.println("test, i am connected");
    }

    
}
