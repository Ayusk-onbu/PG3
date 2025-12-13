#include <Windows.h>
#include <iostream>

int main() {
	SetConsoleOutputCP(65001); // Set console code page to UTF-8
	printf("%s\n", "Hello, World! あ");

	return 0;
}