CFLAGS := -c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS := -lboost_system -lpthread
OFILES := bin/MessageEncoderDecoder.o bin/Frames.o bin/event.o bin/connectionHandler.o bin/CommandsHandler.o bin/KeyboardListener.o

all: StompWCIClient

StompWCIClient: $(OFILES)
	g++ -o bin/StompWCIClient $(OFILES) $(LDFLAGS)

bin/MessageEncoderDecoder.o: src/MessageEncoderDecoder.cpp
	g++ $(CFLAGS) -o bin/MessageEncoderDecoder.o src/MessageEncoderDecoder.cpp

bin/Frames.o: src/Frames.cpp
	g++ $(CFLAGS) -o bin/Frames.o src/Frames.cpp



# Testers
TEST_FILE_TO_RUN := bin/Testers/MessageEncoderDecoderTester

MessageEncoderDecoderTester: bin/MessageEncoderDecoder.o bin/Frames.o src/Testers/MessageEncoderDecoderTester.cpp
	g++ bin/MessageEncoderDecoder.o bin/Frames.o src/Testers/MessageEncoderDecoderTester.cpp -o bin/Testers/MessageEncoderDecoderTester $(LDFLAGS)


.PHONY: test
test: 
	./$(TEST_FILE_TO_RUN)


# Clean command
.PHONY: clean
clean:
	rm -rf bin/* bin/Testers/*

.PHONY: run
run: 
	./bin/StompWCIClient
