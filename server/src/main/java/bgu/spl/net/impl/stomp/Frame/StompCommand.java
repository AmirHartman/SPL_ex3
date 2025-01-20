package bgu.spl.net.impl.stomp.Frame;

public enum ServiceStompCommand {
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
