#include <iostream>

class IShape {
public:
	IShape() = default;
	virtual ~IShape() = default;
public:
	virtual void Size() = 0;
	virtual void Draw() = 0;
protected:
	float Area_;
};

class Circle final :
	public IShape {
public:
	Circle(float radius) : radius_(radius) {};
	~Circle() override = default;
public:
	void Size() override;
	void Draw() override;
private:
	float radius_;
};

class Rectangle final :
	public IShape {
public:
	Rectangle(float width, float height) : width_(width), height_(height) {};
	~Rectangle() override = default;
public:
	void Size() override;
	void Draw() override;
private:
	float width_;
	float height_;
};

int main(){
	Circle circle(5.0f);
	Rectangle rectangle(4.0f, 5.0f);

	circle.Size();
	rectangle.Size();

	circle.Draw();
	rectangle.Draw();

	return 0;
}

void Circle::Size() {
	Area_ = radius_ * radius_ * 3.14f;
}

void Circle::Draw() {
	std::cout << "Circle Area :" << Area_ << "cm^2" << std::endl;
}

void Rectangle::Size() {
	Area_ = width_ * height_;
}

void Rectangle::Draw() {
	std::cout << "Rectangle Area :" << Area_ << "cm^2" << std::endl;
}