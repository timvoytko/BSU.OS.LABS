#define _CRT_SECURE_NO_WARNINGS

#include<vector>
#include "string.h"
#include<sstream>
#include<iostream>
#include"signal.h"


std::string change_commas_to_spaces(std::string str) {

    for (int i = 0; i < str.size(); i++) {
        if (str[i] == ',') {
            str[i] = ' ';
        }
    }
    return str;
}

void kill_by_pid(std::string pids) {
    // so easy - we know pid and we kill by it
    pids = change_commas_to_spaces(pids);
    std::vector<int> pids_array;
    std::stringstream ss(pids);
    std::string tmp;
    while (ss >> tmp) {
        pids_array.push_back(stoi(tmp));
    }
    for (int i = 0; i < pids_array.size(); i++) {
        pid_t t1 = pids_array[i];
        kill(t1, 1);
    }
}
void kill_by_name(std::string names) {
    names = change_commas_to_spaces(names);
    std::vector<std::string> names_array;
    std::stringstream ss(names);
    std::string tmp;
    while (ss >> tmp) {
        names_array.push_back(tmp);
    }
    for (int i = 0; i < names_array.size(); i++) {
        char buf[512];
        std::string str = "pgrep " + names_array[i];
        FILE* cmd_pipe = popen(str.c_str(), "r");
        fgets(buf, 512, cmd_pipe);
        pid_t pid = strtoul(buf, NULL, 10);
        kill(pid, SIGKILL);

    }
}


int main(int argc, char* argv[]) {
    char* tmp = getenv("PROC_TO_KILL");
    std::string kill_string(tmp);
    kill_by_name(kill_string);
    if (argc < 3) return 0;
    if (strcmp(argv[1], "--id") == 0) {
        kill_by_pid(argv[2]);
    }
    if (strcmp(argv[1], "--name") == 0) {
        kill_by_name(argv[2]);
    }
}