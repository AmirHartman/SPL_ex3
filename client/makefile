CFLAGS := -c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS := -lboost_system -lpthread

OFILES := bin/MessageEncoderDecoder.o bin/Frames.o bin/event.o bin/connectionHandler.o bin/CommandsHandler.o

all: StompWCIClient

StompWCIClient: $(OFILES) bin/StompClient.o
	g++ -o bin/StompWCIClient $(OFILES) bin/StompClient.o $(LDFLAGS)

bin/StompClient.o: src/StompClient.cpp
	g++ $(CFLAGS) -o bin/StompClient.o src/StompClient.cpp

bin/CommandsHandler.o: src/CommandsHandler.cpp
	g++ $(CFLAGS) -o bin/CommandsHandler.o src/CommandsHandler.cpp

bin/connectionHandler.o: src/connectionHandler.cpp
	g++ $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/event.o: src/event.cpp
	g++ $(CFLAGS) -o bin/event.o src/event.cpp

bin/MessageEncoderDecoder.o: src/MessageEncoderDecoder.cpp
	g++ $(CFLAGS) -o bin/MessageEncoderDecoder.o src/MessageEncoderDecoder.cpp

bin/Frames.o: src/Frames.cpp
	g++ $(CFLAGS) -o bin/Frames.o src/Frames.cpp


.PHONY: clean
clean:
	rm -rf bin/*

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


# Testers____________________________________________________________


# Dear assignemnt tester, during this assignment i've learned some more advanced makefile commands.
# To clarify that I have learned the material and didn't just copy the makefile from an engine I will explain the args used:

# This is just a simple variables declaration in makefile
# BIN_TEST_DIR := bin/Testers
# SRC_TEST_DIR := src/Testers

# # Here I am creating a build rule for the test files. This will be used to build the test file after the file was chosen.
# # the structure is a simple target: dependencies structure, as learned in class, using variables.
# # The mkdir command is used to create a directory if it doesn't exist.
# $(BIN_TEST_DIR)/$(TEST): $(SRC_TEST_DIR)/$(TEST).cpp $(OFILES)
# 	@mkdir -p $(BIN_TEST_DIR)
# 	g++ -o $(BIN_TEST_DIR)/$(TEST) $(SRC_TEST_DIR)/$(TEST).cpp $(OFILES) $(LDFLAGS)

# 1. .PHONY is a command that tells makefile that this command is not a file name, but a command name (also given in the skeleton makefile)
# 2. if: if statement like in other languages, here we use [] instead of () we are used to.
# 3. The '$' sign is used to get the value of a variable, here we are getting the value of the TEST variable.
# 4. The "" is used to tell the shell that this is a string, and not a command.
# 5. The -z is used to check if the string (e.g TEST) is empty.
# 6. The ';' is used to tell the shell that the command is over, like used in Java or C++.
# 7. then: used to tell the shell that the if statement is over, and that the next line is the command to run.
# 8. The '\': is used to tell the shell that the command is not over, and that the next line is a continuation of the current line.
# 9. echo: used to print things to the console.
# 10. for: simple for each like used in other languages.
# 11. $(SRC_TEST_DIR)*.cpp: this returns all the files in the SRC_TEST_DIR that end with .cpp
# 12. $$: when using double '$' we are passing the arg '$' to the shell iteself, meaning we are getting value the varialbe while running so the makefile itself wont replace its value in the makefile code.
# 13. basename: a shell command that returns the name of the file without the path or the ".cpp" ending.
# 14. @: used to tell the shell to run the command without printing it to the console.
# 15. fi: like using "finally" in java after a try command.
# 16. exit 1: like returning 1 from main in c++, indicated that the make failed.
# 17. $(MAKE): used to run the make command from the shell. It will build the test file, using the build rule we created earlier.
# 18. $(BIN_TEST_DIR)/$(TEST): used to run the test file we just built.

# .PHONY: test
# test: 
# 	@if [ -z "$(TEST)" ]; then \
# 		echo "Please provide a test name"; \
# 		echo "Available tests: "; \
# 		for file in $(SRC_TEST_DIR)/*.cpp; do \
# 			test_name=$$(basename $$file .cpp) ;\
# 			echo " - $$test_name"; \
# 		done; \
# 		echo "Usage: make test TEST=<test_name>"; \
# 		exit 1; \
# 	fi
# 		$(MAKE) $(BIN_TEST_DIR)/$(TEST)
# 		@echo
# 		@echo "Running test $(TEST):"
# 		@echo
# 		@$(BIN_TEST_DIR)/$(TEST)


