#include <Novice.h>
#include <memory>

const char kWindowTitle[] = "LE2B_19_ハマダカズヤ_04_01";

struct Vector2 {
	float x;
	float y;
};

class InputManager {
public:
	static void Update();
	static bool GetKeys(int keyCode) {
		return keys_[keyCode];
	}
	static bool GetPreKeys(int keyCode) {
		return preKeys_[keyCode];
	}
private:
	// キー入力結果を受け取る箱
	static char keys_[256];
	static char preKeys_[256];
};

char InputManager::keys_[256] = { 0 };
char InputManager::preKeys_[256] = { 0 };

enum SCENE {
	TITLE,
	STAGE,
	CLEAR
};

class IScene {
protected:
	static int sceneNo_;
public:
	virtual ~IScene() = default;
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	int GetSceneNo() {
		return sceneNo_;
	}
};

int IScene::sceneNo_ = TITLE;

class TitleScene final :
	public IScene {
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
private:
	int backgroundTextureHandle_;
};

class StageScene final :
	public IScene {
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
private:
	// Background
	int backgroundTextureHandle_;
	float firstBackgroundScrollY_;
	float secondBackgroundScrollY_;
	// Player
	Vector2 playerPos_;
	bool isShot_;
	Vector2 bulletPos_;
	float bulletRadius_;
	// Enemy
	Vector2 enemyPos_;
	float velocityX_;
	float enemyRadius_;
	bool isAlive_;
};

class ClearScene final :
	public IScene {
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
private:
	int backgroundTextureHandle_;
};

class GameManager {
public:
	GameManager();
	~GameManager() = default;
public:
	int Run();
private:
	std::unique_ptr<IScene> sceneArr_[3];
	int currentSceneNo_;
	int prevSceneNo_;
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	GameManager gameManager;

	gameManager.Run();

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

GameManager::GameManager() {
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[STAGE] = std::make_unique<StageScene>();
	sceneArr_[CLEAR] = std::make_unique<ClearScene>();
	currentSceneNo_ = TITLE;
	prevSceneNo_ = currentSceneNo_;
}

int GameManager::Run() {
	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		InputManager::Update();

		prevSceneNo_ = currentSceneNo_;
		currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();

		if (prevSceneNo_ != currentSceneNo_) {
			sceneArr_[currentSceneNo_]->Initialize();
		}

		sceneArr_[currentSceneNo_]->Update();

		sceneArr_[currentSceneNo_]->Draw();

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (InputManager::GetKeys(DIK_SPACE) == 0 && InputManager::GetKeys(DIK_ESCAPE) != 0) {
			break;
		}
	}
	return 0;
}

void TitleScene::Initialize() {
	backgroundTextureHandle_ = Novice::LoadTexture("./NoviceResources/white1x1.png");
}

void TitleScene::Update() {
	if (InputManager::GetPreKeys(DIK_SPACE) == 0 && InputManager::GetKeys(DIK_SPACE)) {
		sceneNo_ = STAGE;
	}
}

void TitleScene::Draw() {
	Novice::DrawQuad(0, 0, 1280, 0, 0, 720, 1280, 720, 0, 0, 1280, 720, backgroundTextureHandle_, GREEN);
}

void StageScene::Initialize() {
	// Background
	backgroundTextureHandle_ = Novice::LoadTexture("./Images/bg.png");
	firstBackgroundScrollY_ = 0.0f;
	secondBackgroundScrollY_ = -720.0f;
	// Player
	playerPos_ = { 640.0f, 600.0f };
	isShot_ = false;
	bulletPos_ = { 0.0f, 0.0f };
	bulletRadius_ = 5.0f;
	// Enemy
	enemyPos_ = { 640.0f, 100.0f };
	velocityX_ = 5.0f;
	enemyRadius_ = 30.0f;
	isAlive_ = true;
}

void StageScene::Update() {
	// Background Update
	firstBackgroundScrollY_ += 2.0f;
	secondBackgroundScrollY_ += 2.0f;
	if (firstBackgroundScrollY_ >= 720.0f) {
		firstBackgroundScrollY_ = -720.0f;
	}
	if (secondBackgroundScrollY_ >= 720.0f) {
		secondBackgroundScrollY_ = -720.0f;
	}

	// Enemy Update
	if (isAlive_) {
		enemyPos_.x += velocityX_;
		if (enemyPos_.x + enemyRadius_ >= 1280.0f || enemyPos_.x - enemyRadius_ <= 0.0f) {
			velocityX_ *= -1.0f;
		}
	}
	// Player Update
	if (InputManager::GetKeys(DIK_W)) {
		playerPos_.y -= 5.0f;
	}
	if (InputManager::GetKeys(DIK_A)) {
		playerPos_.x -= 5.0f;
	}
	if (InputManager::GetKeys(DIK_S)) {
		playerPos_.y += 5.0f;
	}
	if (InputManager::GetKeys(DIK_D)) {
		playerPos_.x += 5.0f;
	}
	// Bullet Update
	if (!isShot_) {
		if (InputManager::GetKeys(DIK_SPACE)) {
			bulletPos_ = playerPos_;
			isShot_ = true;
		}
	}
	else if (isShot_) {
		bulletPos_.y -= 10.0f;
		if (bulletPos_.y + bulletRadius_ <= 0.0f) {
			isShot_ = false;
		}
	}
	// Collision Check
	if (isAlive_ && isShot_) {
		float dx = enemyPos_.x - bulletPos_.x;
		float dy = enemyPos_.y - bulletPos_.y;
		float distanceSquared = dx * dx + dy * dy;
		float radiusSum = enemyRadius_ + bulletRadius_;
		if (distanceSquared <= radiusSum * radiusSum) {
			isAlive_ = false;
			isShot_ = false;
			sceneNo_ = CLEAR;
		}
	}
}

void StageScene::Draw() {
	// Background Draw
	Novice::DrawQuad(0, static_cast<int>(firstBackgroundScrollY_), 1280, static_cast<int>(firstBackgroundScrollY_), 0, static_cast<int>(firstBackgroundScrollY_) + 720, 1280, static_cast<int>(firstBackgroundScrollY_) + 720, 0, 0, 1280, 720, backgroundTextureHandle_, WHITE);
	Novice::DrawQuad(0, static_cast<int>(secondBackgroundScrollY_), 1280, static_cast<int>(secondBackgroundScrollY_), 0, static_cast<int>(secondBackgroundScrollY_) + 720, 1280, static_cast<int>(secondBackgroundScrollY_) + 720, 0, 0, 1280, 720, backgroundTextureHandle_, WHITE);
	// Player Draw
	Novice::DrawBox(static_cast<int>(playerPos_.x - 25.0f), static_cast<int>(playerPos_.y - 25.0f), 50, 50, 0.0f, WHITE, kFillModeSolid);
	// Bullet Draw
	if (isShot_) {
		Novice::DrawEllipse(static_cast<int>(bulletPos_.x), static_cast<int>(bulletPos_.y), static_cast<int>(bulletRadius_), static_cast<int>(bulletRadius_), 0.0f, WHITE, kFillModeSolid);
	}
	// Enemy Draw
	if (isAlive_) {
		Novice::DrawEllipse(static_cast<int>(enemyPos_.x), static_cast<int>(enemyPos_.y), static_cast<int>(enemyRadius_), static_cast<int>(enemyRadius_), 0.0f, RED, kFillModeSolid);
	}
}

void ClearScene::Initialize() {
	backgroundTextureHandle_ = Novice::LoadTexture("./NoviceResources/white1x1.png");
}

void ClearScene::Update() {
	if (InputManager::GetPreKeys(DIK_SPACE) == 0 && InputManager::GetKeys(DIK_SPACE)) {
		sceneNo_ = TITLE;
	}
}

void ClearScene::Draw() {
	Novice::DrawQuad(0, 0, 1280, 0, 0, 720, 1280, 720, 0, 0, 1280, 720, backgroundTextureHandle_, 0xFFFF00FF);
}

void InputManager::Update() {
	// キー入力を受け取る
	memcpy(preKeys_, keys_, 256);
	Novice::GetHitKeyStateAll(keys_);
}