#include <iostream>

template<typename T1, typename T2>
class Compare {
public:
	// コンストラクタ
	Compare(T1 numberA, T2 numberB) 
		: numberA_(numberA), numberB_(numberB) {};
	T1 Min() {
		return static_cast<T1>(numberA_ > numberB_ ? numberB_ : numberA_);
	}
private:
	T1 numberA_;
	T2 numberB_;
};

int main() {
	Compare<int, int> cInt(10, 1);
	Compare<float, float> cFloat(10.0f, 1.0f);
	Compare<double, double> cDouble(10.0, 1.0);
	Compare<int, float> cIntFloat(10, 1.0f);
	Compare<int, double> cIntDouble(10, 1.0);
	Compare<float, double> cFloatDouble(10.0f, 1.0);

	std::cout << cInt.Min() << std::endl;
	std::cout << cFloat.Min() << std::endl;
	std::cout << cDouble.Min() << std::endl;
	std::cout << cIntFloat.Min() << std::endl;
	std::cout << cIntDouble.Min() << std::endl;
	std::cout << cFloatDouble.Min() << std::endl;

	return 0;
}