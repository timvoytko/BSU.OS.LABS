#define _CRT_SECURE_NO_WARNINGS

#include<string>
#include<iostream>
#pragma warning(disable : 4996) .
int main() {
	std::cout << "Choose what to kill: 1(Names)/ 2(PIDs)?\n";
	int task;
	std::cin >> task;
	std::string procs_to_kill="PROC_TO_KILL=1,Telegram.exe, explorer.exe, chrome.exe";
	std::string request_to_kill;
	// dont't checking bad inputs(think this task not about this)
	if (task == 1) {
		std::cout << "Enter name:\n";
		request_to_kill = " --name ";
		std::string tmp;
		std::cin >> tmp;
		request_to_kill = request_to_kill + tmp + ".exe";
	}
	else {
		std::cout << "Enter pid:\n";
		request_to_kill = " --id ";
		std::string tmp;
		std::cin >> tmp;
		request_to_kill = request_to_kill + tmp + ".exe";
	}
	// creating exe for killer and env
	_putenv(const_cast<char*>(procs_to_kill.c_str()));
	std::string text = "C:\\Users\\Timvo\\source\\repos\\ProcessKillers\\Debug\\ProcessKillers.exe" + request_to_kill;
	system(text.c_str());
	// deleting env
	_putenv("PROC_TO_KILL=");
}