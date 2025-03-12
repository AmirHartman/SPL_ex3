CFLAGS := -c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS := -lboost_system -lpthread

OFILES := bin/MessageEncoderDecoder.o bin/frame.o bin/event.o bin/connectionHandler.o bin/StompProtocol.o bin/CommandsHandler.o bin/main.o bin/SocketReader.o

all: createBin StompEMIClient

StompEMIClient: $(OFILES) 
	g++ -g -o bin/StompEMIClient $(OFILES) $(LDFLAGS)

bin/main.o: src/main.cpp
	g++ $(CFLAGS) -o bin/main.o src/main.cpp

bin/StompProtocol.o: src/StompProtocol.cpp
	g++ $(CFLAGS) -o bin/StompProtocol.o src/StompProtocol.cpp

bin/connectionHandler.o: src/connectionHandler.cpp
	g++ $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/event.o: src/object/event.cpp
	g++ $(CFLAGS) -o bin/event.o src/object/event.cpp

bin/MessageEncoderDecoder.o: src/MessageEncoderDecoder.cpp
	g++ $(CFLAGS) -o bin/MessageEncoderDecoder.o src/MessageEncoderDecoder.cpp

bin/frame.o: src/object/frame.cpp
	g++ $(CFLAGS) -o bin/frame.o src/object/frame.cpp

bin/CommandsHandler.o: src/CommandsHandler.cpp
	g++ $(CFLAGS) -o bin/CommandsHandler.o src/CommandsHandler.cpp

bin/SocketReader.o : src/SocketReader.cpp
	g++ $(CFLAGS) -o bin/SocketReader.o src/SocketReader.cpp

.PHONY: clean
clean:
	rm -rf bin/*

.PHONY: createBin
createBin:
	@mkdir -p bin

.PHONY: run
run: 
	@$(MAKE)
	@clear
	@echo
	@echo "Running StompWCIClient"
	@echo
	@if [ "$(DEBUG)" = "true" ]; then \
		echo "Launching StompWCIClient in debug mode"; \
		./bin/StompWCIClient debug; \
	else \
		./bin/StompWCIClient; \
	fi

