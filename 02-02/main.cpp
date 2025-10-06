#include "Windows.h"
#include <iostream>

void CompareRecursiveVsFixedWage(int fixedWage, int recursive, int num = 1, int fixedWageSum = 0, int recursiveSum = 0);

int main() {
	CompareRecursiveVsFixedWage(1226, 100, 10);
	return 0;
}

void CompareRecursiveVsFixedWage(int fixedWage, int recursive, int num, int fixedWageSum, int recursiveSum) {
	fixedWageSum += 1226;
	recursiveSum += recursive;

	(fixedWage >= recursive) ? printf_s("%2d hour: Fixed Wage = %d, o Recursive = %7d x ||| Fixed Wage Sum = %7d, Recursive Sum = %7d\n", fixedWageSum / 1226, fixedWage, recursive, fixedWageSum, recursiveSum):
	                           printf_s("%2d hour: Fixed Wage = %d, x Recursive = %7d o ||| Fixed Wage Sum = %7d, Recursive Sum = %7d\n", fixedWageSum / 1226, fixedWage, recursive, fixedWageSum, recursiveSum);
	
	if (num > 0) {
		CompareRecursiveVsFixedWage(fixedWage, recursive * 2 - 50, num - 1,fixedWageSum,recursiveSum);
	}
}