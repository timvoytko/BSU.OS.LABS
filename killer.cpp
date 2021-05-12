#define _CRT_SECURE_NO_WARNINGS

#include<vector>
#include<string>
#include<Windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include<sstream>

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
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, pids_array[i]);
        if (hProcess != NULL) {
            TerminateProcess(hProcess, 9);
            CloseHandle(hProcess);
        }
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
        // we need to find all proccesses with each name and we do it
        HANDLE h_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
        PROCESSENTRY32 proc_entry;
        proc_entry.dwSize = sizeof(proc_entry);
        BOOL h_res = Process32First(h_snapshot, &proc_entry);
        while (h_res) {
            wchar_t* wc = new wchar_t[names_array[i].size()+1];
            mbstowcs(wc, names_array[i].c_str(), names_array[i].size() + 1);
            if (wcscmp(proc_entry.szExeFile,wc) == 0) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,(DWORD)proc_entry.th32ProcessID);
                if (hProcess != NULL) {
                    TerminateProcess(hProcess, 9);
                    CloseHandle(hProcess);
                }
            }
            h_res = Process32Next(h_snapshot, &proc_entry);
        }
        CloseHandle(h_snapshot);
    }
}


int main(int argc, char* argv[]) {
    char* a = getenv("PROC_TO_KILL");
    std::string str(a);
    str = str.substr(1);
    // only for show that I able to parse both ways
    if (a[0] == '1') {
        kill_by_name(str);
    }
    else {
        kill_by_pid(str);
    }
    if (argc < 3) return 0;

    if (strcmp(argv[1], "--id") == 0) {
        kill_by_pid(argv[2]);
    }
    if (strcmp(argv[1], "--name") == 0) { 
        kill_by_name(argv[2]);
    }
}