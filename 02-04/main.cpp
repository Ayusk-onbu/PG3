#include "Windows.h"
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <functional>

void DelayReveal(std::function<void(int,int)>fn, unsigned int delayMs, int roll, int userGuess);

int main() {
	int isEnd = false;
	int theFace = 0;
	int userInput;
	srand((unsigned int)time(NULL));
	std::function<void(int, int)>func = [](int roll, int userGuess) {
		std::cout << "サイコロの目は " << roll << " でした" << std::endl;
		if ((roll % 2 == 0 && userGuess == 0) || (roll % 2 == 1 && userGuess == 1)) {
			std::cout << "予想が当たりました！" << std::endl;
		}
		else {
			std::cout << "予想が外れました。" << std::endl;
		}
	};
	while (!isEnd) {
		std::cout << "サイコロの出目を予想して" << std::endl;
		std::cout << "奇数なら 1, 偶数なら 0" << std::endl;
		scanf_s("%d", &userInput);
		theFace = rand() % 6 + 1;
		DelayReveal(func, 3000, theFace, userInput);
	}
	return 0;
}

void DelayReveal(std::function<void(int,int)>fn, unsigned int delayMs, int roll, int userGuess) {
	std::cout << "----------" << std::endl;
	Sleep(delayMs / 3);
	std::cout << "-----" << std::endl;
	Sleep(delayMs / 3);
	std::cout << "-" << std::endl;
	Sleep(delayMs / 3);

	fn(roll, userGuess);
}