#include<iostream>

const int N = 3;
const int STOPNUMBER = 41020913; // djtim

int main() {
	int input;
	std::cin >> input;
	while (input != STOPNUMBER) {
		std::cout << input*7 << " ";
		std::cin >> input;
	}
	std::cout << STOPNUMBER;
	return 0;
}
