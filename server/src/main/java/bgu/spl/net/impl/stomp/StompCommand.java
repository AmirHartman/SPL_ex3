package bgu.spl.net.impl.stomp.Frame;

public enum StompCommand {
    // server commands
    CONNECTED,
    MESSAGE,
    RECEIPT,
    ERROR,

    //client commands
    CONNECT,
    SUBSCRIBE,
    UNSUBSCRIBE,
    SEND,
    DISCONNECT;



}
