#include <Windows.h>
#include <iostream>

template<typename T>
T Min(T n1, T n2) {
	return (n1 < n2) ? n1 : n2;
}

int main() {
    // int型のペア
    int i1 = 10, i2 = 20;
    printf_s("Min(%d, %d) = %d\n", i1, i2, Min(i1, i2));

    // float型のペア
    float f1 = 10.5f, f2 = 20.5f;
    printf_s("Min(%.1ff, %.1ff) = %.2ff\n", f1, f2, Min(f1, f2));

    // double型のペア
    double d1 = 10.5, d2 = 20.5;
    printf_s("Min(%.1f, %.1f) = %.2f\n", d1, d2, Min(d1, d2));

	return 0;
}