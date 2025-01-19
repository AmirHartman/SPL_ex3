package bgu.spl.net.srv;

import java.util.concurrent.atomic.AtomicInteger;

public class MessageIdGenerator {
       private static final AtomicInteger idCounter = new AtomicInteger(0);

    public static int generateId() {
        return idCounter.getAndIncrement();
    }
}
 
