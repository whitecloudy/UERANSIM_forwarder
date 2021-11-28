#ifndef __STATE_MANAGER__
#define __STATE_MANAGER__

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

typedef enum State_type
{
    Listen_e,
    Action_e,
    Success_e
} State_type_t;

typedef enum Chan_type
{
    True_e,
    False_e
} Chan_type_t;

class StateManager {

public:
    void set_testcase(string filepath); // Read and analyze TestCase
    void receive_string(string input); // Receive string and discern it
    void change_state(); // If conditions are fulfilled, change state (Action <-> Listen)

    Chan_type_t get_chanUB_state() {
        return chanUB;
    };

    Chan_type_t get_chanBU_state() {
        return chanBU;
    };

    string send_act_string(); // Send action string

    State_type_t state = Listen_e;
    Chan_type_t chanUB = False_e;
    Chan_type_t chanBU = False_e;
};

#endif
