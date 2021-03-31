#include<thread>
#include"buff_chanel.h"
#include<string>
#include <chrono>
#include<iostream>
template<class T>
void f1(BufferedChannel<T>& chanel) {
	std::string words[] = { "Hello\n","I want to recommend you\n","my songs from Soundcloud:\n","https://soundcloud.com/hopeyouenjoydjtim \n" };
	for (int i = 0; i < 4; i++) {
		chanel.Send(words[i]);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
template<class T>
void f2(BufferedChannel<T>& chanel) {
	for (int i = 0; i < 4; i++) {
		std::cout << chanel.Recv().first;
	}
}
template<class T>
void f3(BufferedChannel<T>& chanel) {
	std::string words[] = { "1","2 we want to crash a size","3","4","5","6" };
	for (int i = 0; i < 6; i++) {
		chanel.Send(words[i]);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
template<class T>
void f4(BufferedChannel<T>& chanel) {
	for (int i = 0; i < 5; i++) {
		std::cout << chanel.Recv().first;
	}
}

int main() {
	BufferedChannel<std::string> chanel(2);
	std::thread t1(f1<std::string>, std::ref(chanel));
	std::thread t2(f2<std::string>, std::ref(chanel));
	t1.join();
	t2.join();


	
}


