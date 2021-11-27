#include "StateManager.hpp"

string dataset[1000];
int current_stage = 1;
int loop_idx = 0;

void
StateManager::set_testcase(string filepath) {
    cout << "===== Read and Analyze TestCase =====" << endl;
    ifstream openFile(filepath.data());
    if (openFile.is_open()) {
        size_t pos2 = 0;
        string line;
        string stage;
        int idx = 0;

        while(getline(openFile, line)) {
            size_t pos = line.find("State:");
            if (pos != string::npos) {
                cout << line << endl;
                dataset[idx] = stage;
                idx++;
                stage = "";
            }

            pos = line.find("ue_rrc_state");
            if (pos != string::npos) {
                cout << line << endl;
                pos2 = line.find("= ");
                line = line.substr(pos2 + 2);
                stage += line;
            }

            pos = line.find("bs_rrc_state");
            if (pos != string::npos) {
                cout << line << endl;
                pos2 = line.find("= ");
                line = line.substr(pos2 + 2);
                stage += line;
            }

            pos = line.find("ue_rrc_action");
            if (pos != string::npos) {
                cout << line << endl;
                pos2 = line.find("= ");
                line = line.substr(pos2 + 2);
                stage += line;
            }

            pos = line.find("bs_rrc_action");
            if (pos != string::npos) {
                cout << line << endl;
                pos2 = line.find("= ");
                line = line.substr(pos2 + 2);
                stage += line;
            }

            pos = line.find("inj_adv_act");
            if (pos != string::npos) {
                cout << line << endl;
                stage += line;
            }

            pos = line.find("Loop");
            if (pos != string::npos) {
                cout << line << endl;
                // stage += line;
                loop_idx = idx + 1;
            }
        }
        cout << line << endl;
        dataset[idx] = stage;
        openFile.close();
    }
}

void
StateManager::receive_string(string input) {
    string delimeter = "\n";
    string lines = dataset[current_stage];
    string token;
    size_t last = 0;
    size_t next = 0;

    while((next = lines.find(delimeter)) != string::npos) {
        token = lines.substr(last, next - last);

        if(token == input) {
            cout << "Condition fulfilled: " << token << endl;
            lines.erase(last, next - last + delimeter.length());
            dataset[current_stage] = lines;
            return;
        }
        last = next + delimeter.length();
    }
    cout << "Wrong condition: " << token << endl;
}

void
StateManager::change_state() {
    string lines = dataset[current_stage];
    string next_lines = dataset[current_stage + 1];
    string delimeter = "\n";
    string token;
    size_t last = 0;
    size_t next = 0;
    size_t pos = 0;

    if (StateManager::state == "Listen") {

    }

    if (StateManager::state == "Listen") { // if current state is LISTEN, check whether all conditions are fulfilled
        size_t notOver = 0;

        while((next = lines.find(delimeter)) != string::npos) { // check conditions
            token = lines.substr(last, next - last);
            last = next + delimeter.length();

            pos = token.find("ue_rrc_state");
            if (pos != string::npos) {
                cout << "More conditions remain (Listen)" << endl;
                return;
            }

            pos = token.find("bs_rrc_state");
            if (pos != string::npos) {
                cout << "More conditions remain (Listen)" << endl;
                return;
            }

            pos = token.find("null_action");
            if (pos != string::npos) {
                cout << "More conditions remain (Listen)" << endl;
                return;
            }

            pos = token.find("inj_adv_act");
            if (pos != string::npos) { // find action
                notOver = 1;
            }
        }

        cout << "State Changed (Listen -> Action)" << endl;
        StateManager::state = "Action";

        if (notOver == 0) { // No action remains (This stage only listens)
            cout << "No more actions: Next Stage (Action -> Listen)" << endl;
            StateManager::state = "Listen";
            current_stage++;

            lines = dataset[current_stage];
            if (lines.empty()) { // This stage is the last one
                cout << "Stage Done! (Listen -> Success)" << endl;
                StateManager::state = "Success";
            }
        }
    }

    else if (StateManager::state == "Action") { // if current state is ACTION, check whether all actions are sent
        while((next = lines.find(delimeter)) != string::npos) {
            token = lines.substr(last, next - last);
            last = next + delimeter.length();

            pos = token.find("inj_adv_act");
            if (pos != string::npos) {
                cout << "More actions remain (Action)" << endl;
                return;
            }
        }

        cout << "State Changed (Action -> Listen)" << endl;
        StateManager::state = "Listen";
        current_stage++;

        lines = dataset[current_stage];
        if (lines.empty()) { // This stage is the last one
            cout << "Stage Done! (Listen -> Success)" << endl;
            StateManager::state = "Success";
            return;
        }

        while((next = lines.find(delimeter)) != string::npos) { // what if next stage doesn't have any conditions? (only actions)
            token = lines.substr(last, next - last);
            last = next + delimeter.length();

            pos = token.find("ue_rrc_state");
            if (pos != string::npos) {
                return;
            }

            pos = token.find("bs_rrc_state");
            if (pos != string::npos) {
                return;
            }

            pos = token.find("null_action");
            if (pos != string::npos) {
                return;
            }
        }

        cout << "Next stage only has actions (Listen -> Action)" << endl;
        StateManager::state = "Action";
    }
}

string
StateManager::send_act_string() {
    string action;
    string delimeter = "\n";
    string lines = dataset[current_stage];
    size_t next = lines.find(delimeter);
    size_t pos = lines.find("= ");

    action = lines.substr(pos+2, next);
    lines.erase(0, next + delimeter.length());
    dataset[current_stage] = lines;

    return action;
}