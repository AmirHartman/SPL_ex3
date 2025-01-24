#include <iostream>
#include "../include/ConnectionHandler.h"

int main(int argc, char *argv[]) {

	// Getting the host and port from the command line arguments
	if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
    std::string host = argv[1];
    short port = atoi(argv[2]);
	return 0;

}