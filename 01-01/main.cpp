#include <Windows.h>
#include <iostream>
#include <list>

int main() {
	std::list <const char*> yamanoteLine;
	yamanoteLine.push_back("Tokyo");
	yamanoteLine.push_back("Kanda");
	yamanoteLine.push_back("Akihabara");
	yamanoteLine.push_back("Okachimachi");
	yamanoteLine.push_back("Ueno");
	yamanoteLine.push_back("Uguisudani");
	yamanoteLine.push_back("Nippori");
	yamanoteLine.push_back("Tabata");
	yamanoteLine.push_back("Komagome");
	yamanoteLine.push_back("Sugamo");
	yamanoteLine.push_back("Otsuka");
	yamanoteLine.push_back("Ikebukuro");
	yamanoteLine.push_back("Mejiro");
	yamanoteLine.push_back("Takadanobaba");
	yamanoteLine.push_back("Shin-Okubo");
	yamanoteLine.push_back("Shinjuku");
	yamanoteLine.push_back("Yoyogi");
	yamanoteLine.push_back("Harajuku");
	yamanoteLine.push_back("Shibuya");
	yamanoteLine.push_back("Ebisu");
	yamanoteLine.push_back("Meguro");
	yamanoteLine.push_back("Gotanda");
	yamanoteLine.push_back("Osaki");
	yamanoteLine.push_back("Shinagawa");
	yamanoteLine.push_back("Tamachi");
	yamanoteLine.push_back("Hamamatsucho");
	yamanoteLine.push_back("Shimbashi");
	yamanoteLine.push_back("Yurakucho");

	std::cout << "1970-----------------------------" << std::endl;
	for (auto* lineName : yamanoteLine) {
		std::cout << lineName << std::endl;
	}

	// 1971年に西日暮里駅が開業
	std::list <const char*>::iterator it = yamanoteLine.begin();
	std::advance(it, 7);
	yamanoteLine.insert(it, "Nishi-Nippori");

	std::cout << "2019-----------------------------" << std::endl;
	for (auto* lineName : yamanoteLine) {
		std::cout << lineName << std::endl;
	}

	// 2020年に高輪ゲートウェイ駅が開業
	it = yamanoteLine.begin();
	std::advance(it, 25);
	yamanoteLine.insert(it, "Takanawa Gateway");

	std::cout << "2022-----------------------------" << std::endl;
	for (auto* lineName : yamanoteLine) {
		std::cout << lineName << std::endl;
	}
	return 0;
}