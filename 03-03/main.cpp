#include <iostream>

class Animal {
public:
	Animal() = default;
	virtual ~Animal() = default;
public:
	virtual void Bark() = 0;
};

class Dog final :
	public Animal {
public:
	Dog() = default;
	~Dog() override = default;
public:
	void Bark() override {
		std::cout << "Bow-wow" << std::endl;
	}
};

class Cat final :
	public Animal {
public:
	Cat() = default;
	~Cat() override = default;
public:
	void Bark() override {
		std::cout << "Meow" << std::endl;
	}
};

int main() {
	Dog dog;
	Cat cat;

	dog.Bark();
	cat.Bark();

	return 0;
}