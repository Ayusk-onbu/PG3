#include <Novice.h>

struct Vector2 {
	float x;
	float y;
};

class Player
{
private:

public:
	Player();

	void Initialize();
	void Update();
	void Draw();

	void MoveRight();
	void MoveLeft();
private:
	Vector2 pos_;
	float speed_;
	int textureHandle_;
};

class ICommand
{
public:
	virtual ~ICommand() = default;
	virtual void Exec(Player& player) = 0;
};

class MoveRightCommand
	: public ICommand
{
	void Exec(Player& player)override;
};

class MoveLeftCommand
	: public ICommand
{
	void Exec(Player& player)override;
};

class InputHandler {
public:
	ICommand* HandleInput();

	void AssignMoveRightCommand2PressKeyD();
	void AssignMoveLeftCommand2PressKeyA();

private:
	ICommand* pressKeyD_;
	ICommand* pressKeyA_;
};

const char kWindowTitle[] = "LE2B_19_ハマダカズヤ_05-01";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	InputHandler* inputHandler = nullptr;
	ICommand* iCommand = nullptr;
	Player* player;

	// Initialize
	// [ Input ]
	inputHandler = new InputHandler();

	// [ Assign Command ]
	inputHandler->AssignMoveRightCommand2PressKeyD();
	inputHandler->AssignMoveLeftCommand2PressKeyA();

	// [ new Player ]
	player = new Player();
	player->Initialize();

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		iCommand = inputHandler->HandleInput();

		if (iCommand) {
			iCommand->Exec(*player);
		}

		player->Update();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		player->Draw();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

Player::Player() {
	pos_ = { 0.0f,0.0f };
	speed_ = 0.0f;
	textureHandle_ = 0;
}

void Player::Initialize() {
	pos_ = { 0.0f,40.0f };
	speed_ = 3.0f;
	textureHandle_ = Novice::LoadTexture("./NoviceResources/white1x1.png");
}

void Player::Update() {

}

void Player::Draw() {
	Novice::DrawBox(static_cast<int>(pos_.x), static_cast<int>(pos_.y), 25, 25, 0.0f, WHITE, FillMode::kFillModeSolid);
}

void Player::MoveRight() {
	this->pos_.x += this->speed_;
}

void Player::MoveLeft() {
	this->pos_.x -= this->speed_;
}

void MoveRightCommand::Exec(Player& player) {
	player.MoveRight();
}

void MoveLeftCommand::Exec(Player& player) {
	player.MoveLeft();
}

void InputHandler::AssignMoveRightCommand2PressKeyD() {
	ICommand* command = new MoveRightCommand();
	this->pressKeyD_ = command;
}

void InputHandler::AssignMoveLeftCommand2PressKeyA() {
	ICommand* command = new MoveLeftCommand();
	this->pressKeyA_ = command;
}

ICommand* InputHandler::HandleInput() {
	if (Novice::CheckHitKey(DIK_D)) {
		return pressKeyD_;
	}
	if (Novice::CheckHitKey(DIK_A)) {
		return pressKeyA_;
	}
	return nullptr;
}