#include "Windows.h"
#include <iostream>

int Min(int n1,int n2) {
	return (n1 < n2) ? n1 : n2;
}

float Min(float n1, float n2) {
	return (n1 < n2) ? n1 : n2;
}

double Min(double n1, double n2) {
	return (n1 < n2) ? n1 : n2;
}

int main() {
	printf_s("Min(10, 20) = %d\n", Min(10, 20));
	printf_s("Min(10.5f, 20.5f) = %.2f\n", Min(10.5f, 20.5f));
	printf_s("Min(10.5, 20.5) = %.2f\n", Min(10.5, 20.5));

	return 0;
}