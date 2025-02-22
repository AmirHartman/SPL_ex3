#include "StompProtocol.h"

class CommandsHandler {
public:
    CommandsHandler(StompProtocol& _stomp);
    void execute(vector<string> &command);
    void terminate();

private:
    StompProtocol& stomp;

    // Proccessing string input got by user (accept all options for input, with/without .json, with/without path)
    vector<string> handle_file_path(string& input_string);
    bool parse_json_events_file(string &input_string, names_and_events &namesAndEvents);
};