#include "Windows.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
int main() {
	std::vector<std::string> emailList;
	std::ifstream file("PG3_2025_01_02.txt");
	if (!file) {
		std::cout << "Error opening file." << std::endl;
		return 0;
	}
	std::string line;
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string email;
		while (std::getline(ss, email, ',')) {
			emailList.push_back(email);
		}
	}

	// ソート処理
	std::sort(emailList.begin(), emailList.end());

	for (auto& email : emailList) {
		std::cout << email << std::endl;
	}

	return 0;
}