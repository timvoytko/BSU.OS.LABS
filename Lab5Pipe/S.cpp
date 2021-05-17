#include<iostream>

const int N = 3;
const int STOPNUMBER = 41020913; // djtim

int main() {
	int input;
	std::cin >> input;
	int sum = 0;
	while (input != STOPNUMBER) {
		sum += input;
		std::cin >> input;
	}
	std::cout << sum;
	return 0;
}
