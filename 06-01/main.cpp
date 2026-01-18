#include <iostream>
#include <string>
#include <thread>

void PrintThread(int num) {
	std::string print = "thread" + std::to_string(num);
	std::cout << print.c_str() << std::endl;
}

int main() {
	std::thread th1(PrintThread, 1);
	th1.join();
	std::thread th2(PrintThread, 2);
	th2.join();
	std::thread th3(PrintThread, 3);
	th3.join();
	return 0;
}