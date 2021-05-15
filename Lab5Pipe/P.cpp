#include<iostream>
#include<cmath>
const int N = 3;
const int STOPNUMBER = 41020913; // djtim

int main() {
	int input;
	std::cin >> input;
	while (input != STOPNUMBER) {
		std::cout << std::pow(input,3) << " ";
		std::cin >> input;
	}
	std::cout << STOPNUMBER;
	return 0;
}
