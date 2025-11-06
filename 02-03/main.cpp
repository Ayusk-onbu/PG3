#include "Windows.h"
#include <time.h>
#include <iostream>
#include <stdio.h>

void DelayReveal(void (*fn)(int, int), unsigned int delayMs, int roll, int userGuess);

void ShowResult(int roll, int userGuess);

int main() {
	int isEnd = false;
	int theFace = 0;
	srand((unsigned int)time(NULL));
	while (!isEnd) {
		std::cout << "サイコロの出目を予想して" << std::endl;
		std::cout << "奇数なら 1, 偶数なら 0" << std::endl;
		int userInput;
		scanf_s("%d",&userInput);
		theFace = rand() % 6 + 1;
		DelayReveal(ShowResult, 3000, theFace, userInput);
	}
	return 0;
}

void DelayReveal(void (*fn)(int, int), unsigned int delayMs, int roll, int userGuess) {
	std::cout << "----------" << std::endl;
	Sleep(delayMs / 3);
	std::cout << "-----" << std::endl;
	Sleep(delayMs / 3);
	std::cout << "-" << std::endl;
	Sleep(delayMs / 3);

	fn(roll, userGuess);
}

void ShowResult(int roll, int userGuess) {
	std::cout << "サイコロの目は " << roll << " でした" << std::endl;
	if ((roll % 2 == 0 && userGuess == 0) || (roll % 2 == 1 && userGuess == 1)) {
		std::cout << "予想が当たりました！" << std::endl;
	}
	else {
		std::cout << "予想が外れました。" << std::endl;
	}
}