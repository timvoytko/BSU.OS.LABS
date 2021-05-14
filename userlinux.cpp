#define _CRT_SECURE_NO_WARNINGS

#include<string>
#include<iostream>
#pragma warning(disable : 4996) .
int main() {
    std::cout << "Choose what to kill: 1(Names)/ 2(PIDs)?\n";
    int task;
    std::cin >> task;

    std::string request_to_kill;
    // dont't checking bad inputs(think this task not about this)
    if (task == 1) {
        std::cout << "Enter name:\n";
        request_to_kill = " --name ";
        std::string tmp;
        std::cin >> tmp;
        request_to_kill = request_to_kill + tmp ;
    }
    else {
        std::cout << "Enter pid:\n";
        request_to_kill = " --id ";
        std::string tmp;
        std::cin >> tmp;
        request_to_kill = request_to_kill + tmp ;
    }
    std::string procs_to_kill = "rhythmbox";
    setenv("PROC_TO_KILL",procs_to_kill.c_str(),0);
    std::string text = "g++ /home/djtim/BSU/OS_LABS/LAB4/KIllahLinux4.cpp -o killer && ./killer" + request_to_kill;
    system(text.c_str());
    // deleting env
    unsetenv("PROC_TO_KILL");
}