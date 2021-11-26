#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class StateManager {

public:
    void set_testcase(string filepath); // Read and analyze TestCase
    void receive_string(string input); // Receive string and discern it
    void change_state(); // If conditions are fulfilled, change state (Action <-> Listen)
    string send_act_string(); // Send action string

    string state = "Listen";
};
