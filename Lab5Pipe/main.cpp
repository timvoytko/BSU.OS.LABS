#include<iostream>
#include <unistd.h>
#include<string>
#include<sys/types.h>
#include<sys/wait.h>
int main() {
  std::string ma_way_procs[] = { "./M","./a","./P","./S" };
  std::cout<<"Stop signal is 41020913\nEnter Numbers:\n";
  int p[3];
  int child = 0;
  for (int i = 0; i < 3; i++) {
    pipe(p);
    child = fork();
    if (!child) {
      dup2(p[1], 1);
      execlp(ma_way_procs[i].c_str(), ma_way_procs[i].c_str(), NULL);
      exit(0);
    }
    dup2(p[0],0);
    close(p[1]);
  }
  execlp(ma_way_procs[3].c_str(), ma_way_procs[3].c_str(), NULL);

  return 0;


}
