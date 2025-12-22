#include <iostream>

class Enemy {
public:
	// 状態
	enum class State {
		Approach,// 接近
		Shoot,   // 射撃
		Leave,   // 離脱
	};
public:
	// コンストラクタ
	Enemy() : currentState_(State::Approach) {}
	// 更新処理
	bool Update();
	// 接近
	bool Approach();
	// 射撃
	bool Shoot();
	// 離脱
	bool Leave();
private:
	State currentState_;
	// 関数ポインタのテーブル
	static bool (Enemy::* spFuncTable[])();
};

bool Enemy::Update() {
	return (this->*spFuncTable[static_cast<size_t>(currentState_)])();
}

bool Enemy::Approach() {
	std::cout << "Approach" << std::endl;
	currentState_ = State::Shoot;
	return true;
}

bool Enemy::Shoot() {
	std::cout << "Shoot" << std::endl;
	currentState_ = State::Leave;
	return true;
}

bool Enemy::Leave() {
	std::cout << "Leave" << std::endl;
	return false;
}

bool (Enemy::* Enemy::spFuncTable[])() = {
	&Enemy::Approach,
	&Enemy::Shoot,
	&Enemy::Leave
};

int main() {
	Enemy enemy;
	while (enemy.Update()) {

	}
	return 0;
}